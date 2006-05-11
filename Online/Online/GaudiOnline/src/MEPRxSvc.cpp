//	============================================================
//
//	MEPRxSvc.cpp
//	------------------------------------------------------------
//
//	Package   : GaudiOnline
//
//	Author    : Niko Neufeld
//                  using code by B. Gaidioz and M. Frank
//
//	===========================================================
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <vector>
#include <utility>
#include <map>


#include "GaudiKernel/SvcFactory.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/ISvcLocator.h"
#include "Event/RawBank.h"
#include "GaudiKernel/Incident.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiOnline/MEPRxSvc.h"
#include "GaudiOnline/DimTaskFSM.h"
#include "RTL/rtl.h"
#include "WT/wt_facilities.h"
#include "MBM/MepProducer.h"
#include "GaudiOnline/MEPHdr.h"
 
#define MAX_R_PACKET (0x10000 + 20)
#define MEP_SENT  MBM_NORMAL
#define MEP_ADDED (MEP_SENT + 1)
#define IP_HEADER_LEN 20
#define IP_PROTO_HLT  0xF2
#define MEP_REQ_TTL 10
#define MEP_REQ_TOS 0xFD  /* the D is historic from the MDPs :-) */
#ifndef EVENT_TYPE_DAQBAD
#define EVENT_TYPE_DAQBAD (EVENT_TYPE_MEP + 1)
#endif
#ifndef MEP_ADD_ERROR
#define MEP_ADD_ERROR -9999
#endif
/* maximum number of sources (TELL1, RS, UKL1) */
#ifndef MAX_SRC
#define MAX_SRC 320
#endif 
#ifndef UINT
#define UINT unsigned int
#endif
#define RAWBANKSIZE (sizeof(LHCb::RawBank) - sizeof(int)) /* f*** C99 */ 
#define errmsg(x) {char errstr[64]; strerror_r(errno, errstr, 64); \
*m_log << MSG::ERROR << x << errstr << " " << __FILE__ << ":" << __LINE__ << endmsg;}
#define printnum(n, s) n << s << (n == 1 ? "" : "s")
typedef struct iphdr ip_hdr_t; 
typedef struct LHCb::MEPHdr mep_hdr_t;
typedef std::vector<LHCb::MEPRx *>::iterator RXIT;
#define HDR_LEN (IP_HEADER_LEN + sizeof(LHCb::MEPHdr)) 
static u_int8_t __hdr[HDR_LEN];

// wrapper classes for Unix System structs
struct MsgHdr: public msghdr {
  MsgHdr(struct iovec *v, int n) {
    msg_iov = v; msg_iovlen = n; msg_control = NULL; msg_controllen = 0; msg_name = NULL; msg_namelen = 0; }
};
struct InAddr: public in_addr {
  InAddr(u_int32_t a) {
    s_addr = a;
  }
};




/* A MEP request is a minimal legal Ethernet packet
 * The length of the IP payload is hence 44 bytes
 */
#define MEP_REQ_LEN 44
struct __attribute__((__packed__)) MEPReq { /* the __packed__ is *ESSENTIAL* */
  u_int8_t nmep;   /* number of meps requested */
  u_int8_t reserved[MEP_REQ_LEN - 1];
};
static MEPReq mepreq; /* yes, yes this is only differing by case, but I can't 
		       * be bothered - and it's private anyhow */
static struct iovec __mepreq_v[1] = { { &mepreq, sizeof(mepreq) }};
static struct sockaddr __mepreq_addr = { AF_INET, {0, }};
static struct msghdr mepreq_msg = { &__mepreq_addr, sizeof(struct sockaddr),
				      __mepreq_v, 1, NULL, 0, 0 };

struct LHCb::MEPRx: public MEP::Producer {
  // parameters
  int m_refCount, m_spaceSize, m_age;
  LHCb::MEPRxSvc *m_parent;
  UINT m_nSrc;
  MsgStream *m_log;
  int m_r;
  // run-time
  u_int32_t m_l0ID, m_brx, m_nrx;
  u_int16_t m_pf;
  int m_spaceRC;
  int m_eventType;
  MEPEVENT *m_e; 
  struct DAQErrorBankEntry m_eEntry[MAX_SRC];
  class  LHCb::RawBank *m_rawBufHdr;
  int m_seen[MAX_SRC]; 
  int m_maxMsForGetSpace;
  bool m_dry;

  MEPRx(const std::string& nam, int partitionID, int refcnt, 
	size_t siz, LHCb::MEPRxSvc *parent, int nsrc, MsgStream *log, int r) 
    : MEP::Producer(nam, partitionID),  m_refCount(refcnt), m_spaceSize(siz),
      m_parent(parent), m_nSrc(nsrc), m_log(log), m_r(r)
    {
      m_flags = USE_MEP_BUFFER;
      include();
      m_bmid = m_mepID->mepBuffer;
      *m_log << MSG::DEBUG << std::hex << std::showbase << "MEP buffer start: " 
	    << m_mepID->mepStart << "\nEVENT  buffer start: " 
	    << m_mepID->evtStart << "\nRESULT buffer start: " 
	    << m_mepID->resStart << endmsg;     
      *m_log << MSG::DEBUG << "Buffer space: " << m_spaceSize << " bytes" << 
	endmsg;
      m_eventType = EVENT_TYPE_MEP;
      m_rawBufHdr = (class LHCb::RawBank *) new u_int8_t[sizeof(LHCb::RawBank)]; /* don't ask me, ask Markus! */
    }
  ~MEPRx()  {
  }    
  static int spaceTimeOut(void *p) {
    MEPRx *self = (MEPRx *) p;
    if (self->m_spaceRC != -100) return 0;
    mbm_cancel_request(self->m_bmid);
    return 1;
  }
  int spaceRearm(int) {
    UINT timerID;

    m_eventType = EVENT_TYPE_MEP;
    m_brx = m_nrx = 0; memset(m_seen, 0, m_nSrc);
    if (m_maxMsForGetSpace) {
      m_spaceRC = -100;
      if (lib_rtl_set_timer(m_maxMsForGetSpace, LHCb::MEPRx::spaceTimeOut, (void *) this, 
			    &timerID)) {
	*m_log << MSG::ERROR << "Could not initialise timer!" << endmsg;
	return -1;
      }
    }
    m_spaceRC = MEP::Producer::spaceRearm(m_spaceSize);
    if (m_spaceRC == MBM_NORMAL) lib_rtl_kill_timer(timerID);
    return m_spaceRC;
  }
#define RAWBHDRSIZ (sizeof(struct LHCb::RawBank) - 4)
#define MEPHDRSIZ sizeof(struct MEPHdr)
#define MEPFHDRSIZ sizeof(struct MEPFrgHdr)  
#define DAQEERSIZ sizeof(struct DAQErrorBankEntry)

  inline int createDAQErrorBankEntries() {
    int j = 0;
    /* we create a MEP fragment with the required error bank */
    for (u_int32_t i = 0; i < m_nSrc; ++i) {
      if (m_seen[i] == 1) continue;
      m_eEntry[j].m_srcID = i;
      m_eEntry[j].m_srcIPAddr = m_parent->m_srcAddr[i];
      m_eEntry[j].m_errorType = MEPRxSvc::MissingSrc;
      m_eEntry[j].m_pktData = NULL;
      j++;
    }
    return j;
  }
  inline int setupDAQErrorBankHdr() {
    int n = createDAQErrorBankEntries();
    int size = n * DAQEERSIZ + RAWBHDRSIZ;
    int rem;

    m_rawBufHdr->setMagic();
    m_rawBufHdr->setSourceID(m_parent->m_sourceID);
    m_rawBufHdr->setVersion(DAQ_ERR_BANK_VERSION);
    m_rawBufHdr->setType(LHCb::RawBank::DAQ);
    m_rawBufHdr->setSize(size);
    if ((rem = (size % 4))) return (size + 4 - rem); else return size;
  } 
  inline int createDAQErrorMEP(u_int8_t *buf, int nEvt) {
    struct MEPHdr *meph = (struct MEPHdr *) buf;
    int banksize = setupDAQErrorBankHdr();
    meph->m_l0ID = m_l0ID;
    meph->m_totLen = MEPHDRSIZ +  nEvt * (MEPFHDRSIZ + banksize);
    meph->m_nEvt = nEvt;
    buf += MEPHDRSIZ;
    for (int i = 0; i < nEvt; ++i) {
      struct MEPFrgHdr *frgh = (struct MEPFrgHdr *) buf;
      frgh->m_l0IDlow = 0xFFFF & (m_l0ID + i);
      frgh->m_len = banksize;
      buf += MEPFHDRSIZ;
      memcpy(buf, m_rawBufHdr, RAWBHDRSIZ);
      buf += RAWBHDRSIZ;
      memcpy(buf, m_eEntry, banksize - RAWBHDRSIZ);
      buf += (banksize - RAWBHDRSIZ);
    }
    return meph->m_totLen;
  }
  inline void incompleteEvent() {
    
    u_int8_t *buf = (u_int8_t *) m_e->data + m_brx + 4; 
    m_brx += createDAQErrorMEP(buf, m_pf);
    return;
  }
  int spaceAction() { 
    static  int id = -1; 
    int status; 
     
    m_e->evID        = ++id;
    m_event.len = m_brx + sizeof(MEPEVENT);
    m_event.mask[0] = partitionID();
    m_event.mask[1] = 0;
    m_event.mask[2] = 0;
    m_event.mask[3] = 0;
    m_event.type    = m_eventType;    
    if (m_nrx != m_nSrc) incompleteEvent();
    *((int *) m_e->data) = m_brx;
    if (!m_dry) { 
      declareEvent();
      status = sendSpace();
    }
    //info("bang %d %d bytes\n", m_e->evID, m_brx);
    return status;
  }
  // Run the application in synchonous mode
  int runSynchronous() {
    int status = spaceRearm(0);
    if ( status == MBM_NORMAL )  {
      status = spaceAction();
    }
    return status;
  }
  void multipleSrc() {
    m_eventType = EVENT_TYPE_DAQBAD;
  } 
  void badPkt(MEPRxSvc::DAQError type) {
    int x = type;
    m_eventType = EVENT_TYPE_DAQBAD;
  }
  int addMEP(MEPHdr *hdr, int srcid) {
    int len = 0;
 
    if (m_seen[srcid]) multipleSrc();
    m_seen[srcid] = 1;		   
    /* here a lot of tests should be done */
    if (m_nrx == 0) {
      MBM::EventDesc& dsc = event();
      m_e = (MEPEVENT *)dsc.data;
      m_e->refCount    = m_refCount;
      m_e->evID        = hdr->m_l0ID;
      m_e->begin       = int(int(m_e)-m_mepID->mepStart);
      m_e->packing     = -1;
      m_e->valid       = 1;
      m_e->magic       = mep_magic_pattern();
      m_event.len = sizeof(MEPEVENT) - sizeof(m_e->data);
      /* m_e->data  ab da laenge und dann MEPs */	
      m_brx = 0;
      m_pf = hdr->m_nEvt;
    }
    if (m_pf != hdr->m_nEvt) badPkt(MEPRxSvc::WrongPackingFactor);
    
    //info("event 0x%08x src %d\n", hdr->l01_id, m_nrx);
    struct iovec mep_recv_vec[1];
    mep_recv_vec[0].iov_base = (char *) m_e->data + m_brx + 4; 
    mep_recv_vec[0].iov_len = MAX_R_PACKET; 	
    struct msghdr mep_recv_msg; 
    mep_recv_msg.msg_name = 0;
    mep_recv_msg.msg_namelen = 0; // sizeof(struct sockaddr_in),
    mep_recv_msg.msg_iov = mep_recv_vec;
    mep_recv_msg.msg_iovlen = 1;
    mep_recv_msg.msg_flags = 0;
    mep_recv_msg.msg_control = NULL;
    mep_recv_msg.msg_controllen = 0;
    len = recvmsg(m_r, &mep_recv_msg, MSG_DONTWAIT);
    if (len < 0) {
      errmsg("failed to receive message");
      return MEP_ADD_ERROR;
    }
    if (len != hdr->m_totLen) {
      badPkt(MEPRxSvc::ShortPkt);
    }
    m_brx += len;
    m_nrx++; 
    *(int*)m_e->data = m_brx;
//      info("\t\t\t nrx: %d", m_nrx);
//      if (m_nrx == m_nSrc) info("complete\n");
    return (m_nrx == m_nSrc) ? spaceAction() : MEP_ADDED;
  }  
    
};


DECLARE_NAMESPACE_SERVICE_FACTORY(LHCb, MEPRxSvc)

// Standard Constructor
LHCb::MEPRxSvc::MEPRxSvc(const std::string& nam, ISvcLocator* svc)
  : Service(nam, svc), m_handle(0), m_receiveEvents(false), m_incidentSvc(0)
{
  declareProperty("MEPBuffers", m_MEPBuffers);
  declareProperty("ethInterface", m_ethInterface);
  declareProperty("IPNameOdin", m_IPNameOdin = "");
  declareProperty("maxBadPktRatio", m_maxBadPktRatio);
  declareProperty("pktSamplingCount", m_pktSamplingCount);
  declareProperty("maxMsForGetSpace", m_maxMsForGetSpace);
  declareProperty("IPSrc", m_IPSrc);
  declareProperty("IPProtoIn", m_IPProtoIn = IP_PROTO_HLT);
  declareProperty("sockBuf", m_sockBuf = 0x100000);
  declareProperty("partitionID", m_partitionID);
  declareProperty("refCount",m_refCount = 2);
  declareProperty("MEPBufSize", m_MEPBufSize = -1);
  declareProperty("bufName", m_bufName = "MEPRX");
  declareProperty("ownAddress", m_ownAddress = 0xFFFFFFFF);
  m_trashVec[0].iov_base = new u_int8_t[MAX_R_PACKET];
  m_trashVec[0].iov_len = MAX_R_PACKET;
}

// Standard Destructor
LHCb::MEPRxSvc::~MEPRxSvc(){
  delete((u_int8_t *) m_trashVec[0].iov_base);
}

int 
LHCb::MEPRxSvc::exec(void* arg)  {
  LHCb::MEPRxSvc* s = (LHCb::MEPRxSvc*)arg;
  return s->execute();
}

inline bool 
LHCb::MEPRxSvc::cmpL0ID(MEPRx *r, u_int32_t id) {
  //info("cmpL0ID: m_l0ID %d id %d\n", r->m_l0id, id);
  return r->m_l0ID < id;
}

inline void
LHCb::MEPRxSvc::removePkt()
{
  struct MsgHdr msg(m_trashVec, 1);
  int len = recvmsg(m_r, &msg, MSG_DONTWAIT);
  if (len < 0) {
    if (errno != EAGAIN) 
      errmsg("recvmsg");
  }
}
/* age all workDsc and return the oldest */
RXIT
LHCb::MEPRxSvc::ageRx() {
  RXIT k, j;
  if (m_workDsc.empty()) return m_workDsc.end();
  for (k = j = m_workDsc.begin(); j != m_workDsc.end(); ++j) 
    if (--((*j)->m_age) <= 0) {
      k = j;
      (*j)->m_age = 0;
    } else if ((*k)->m_age > (*j)->m_age) k = j;
  return k;
}

inline int
LHCb::MEPRxSvc::getMyAddr(u_int32_t &addr) {
  return 0;
}

int 
LHCb::MEPRxSvc::parseAddr(const std::string &straddr, u_int32_t &addr) {
  struct in_addr a;

  int rc = inet_aton(straddr.c_str(), &a);
  addr = a.s_addr;
  return rc;
}

inline int
LHCb::MEPRxSvc::setupMEPReq(std::string odinName) {
  u_int32_t addr;

  if (odinName.empty()) {
    *m_log << MSG::INFO << "No address for ODIN. Dynamic MEP requests disabled!";
    m_dynamicMEPRequest = false;
    return 0;
  }
  if (parseAddr(odinName, addr) && addrFromName(odinName, addr)) {
    *m_log << MSG::ERROR << "invalid address for ODIN: " <<  odinName << endmsg;
    return 1; 
  } 
  struct in_addr in;
  in.s_addr = addr;
   *m_log << MSG::INFO << "Dynamic MEP requests will be sent to " << 
    inet_ntoa(in) << endmsg;
  memcpy(__mepreq_addr.sa_data, &addr, 4);
  m_dynamicMEPRequest = true;
  return 0;
}

inline int
LHCb::MEPRxSvc::sendMEPReq(int m) {
  int n;

  if (!m_dynamicMEPRequest) return 0;
  mepreq.nmep = m;
  if ((n = sendmsg(m_r, &mepreq_msg, MSG_DONTWAIT | MSG_CONFIRM)) == 
      MEP_REQ_LEN) return 0;
  if (n == -1) {
    errmsg("sendmsg");
    return 1;
  } else {
    *m_log << MSG::ERROR << "MEPRequest corrupted on send!" << endmsg;
    return 1;
  }
}
   
void 
LHCb::MEPRxSvc::freeRx() {
  MEPRx *rx;

  lib_rtl_lock(m_usedDscLock);
  if (m_usedDsc.empty()) {
    lib_rtl_unlock(m_usedDscLock);
    return;
  }
  rx = *(--m_usedDsc.end());
  m_usedDsc.pop_back();
  lib_rtl_unlock(m_usedDscLock);
  if (rx->spaceRearm(0) == MBM_NORMAL) {
    lib_rtl_lock(m_freeDscLock);
    m_freeDsc.push_back(rx);
    lib_rtl_unlock(m_freeDscLock);
    sendMEPReq(1);
    return;
  }
  *m_log << MSG::ERROR << "timeout on getting space" << endmsg;
  return;
}

void
LHCb::MEPRxSvc::forceEvents() {
}
  
int 
LHCb::MEPRxSvc::execute() {
  fd_set fds;
  iovec mep_recv_vec[1];
  RXIT rx; 
//  bool exe = true;
  int maxfd = m_r + 1;
  ip_hdr_t *iphdr = (ip_hdr_t *) __hdr;
  struct MEPHdr *mephdr = (mep_hdr_t *) &__hdr[IP_HEADER_LEN];
  mep_recv_vec->iov_base = __hdr;
  mep_recv_vec->iov_len = HDR_LEN;
  int srcid;

  FD_ZERO(&fds);
  for (;;) {
    int n;
    struct timeval timeout = {2, 0}; /* seconds */
    
    FD_SET(m_r, &fds);
    if ((n = select(maxfd, &fds, NULL, NULL, &timeout)) < 0) {
      errmsg("select");
      continue;
    }
    if (n == 0) {
      if (!m_receiveEvents) {
	forceEvents();
	return 0;
      } 
      *m_log << MSG::DEBUG << "crhhh..." <<  m_workDsc.size() << endmsg;
      continue;
    }
    if (FD_ISSET(m_r, &fds)) {
//      echohdr(hdr);
      MsgHdr msg(mep_recv_vec, 1);
      int len = recvmsg(m_r, &msg, MSG_DONTWAIT | MSG_PEEK);
      if (len < 0) {
	if (errno != EAGAIN) 
	  errmsg("recvmsg");
	continue;
      }
    } 
    if ((srcid = getSrcID(iphdr->saddr)) == - 1) {
      /* we do not expect nor want this */
      removePkt();
      continue;
    }
    //::fprintf(stdout, "\rpktin %d ", ++pktin);
    //    ::fflush(stdout);
    if (!m_workDsc.empty() && mephdr->m_l0ID == m_workDsc.back()->m_l0ID) {
      rx = --m_workDsc.end();
    } else {
      rx = lower_bound(m_workDsc.begin(), m_workDsc.end(), mephdr->m_l0ID, 
		       cmpL0ID);
      if (rx == m_workDsc.end() || (*rx)->m_l0ID != mephdr->m_l0ID) {
	/* not found - get a new descriptor*/
	RXIT oldest = ageRx();
	try {
	  if (m_freeDsc.empty()) {
	    forceEvent(oldest);
	    freeRx(); /* only if not in separate thread */
	  }
	  while (m_freeDsc.empty()) usleep(100); /* only necessary on 
						    multithreading */
	  lib_rtl_lock(m_freeDscLock);
	  MEPRx *rx = *(--m_freeDsc.end());
	  m_freeDsc.pop_back();
	  lib_rtl_unlock(m_freeDscLock);
	  RXIT j = lower_bound(m_workDsc.begin(), m_workDsc.end(), 
			       mephdr->m_l0ID, cmpL0ID);
	  m_workDsc.insert(j, rx);
	  rx->m_age = m_MEPBuffers;
	  rx->m_l0ID = mephdr->m_l0ID;
	}
	catch(std::exception& e) {
	  *m_log << MSG::ERROR << "Exception " << e.what() << endmsg;
	}
      } 
    }
    if ((*rx)->addMEP(mephdr, srcid) == MEP_SENT) {
      m_workDsc.erase(rx);
      lib_rtl_lock(m_usedDscLock);
      m_usedDsc.push_back(*rx);
      lib_rtl_unlock(m_usedDscLock);
    }		   		   
  }
//Incident incident(name(),"DAQ_ERROR");
//m_incidentSvc->fireIncident(incident);
  return 1; 
}		   

// IInterface implementation: Query interface
StatusCode 
LHCb::MEPRxSvc::queryInterface(const InterfaceID& riid,void** ppvInterface) {
  if ( riid == IID_IRunable )   {
    *ppvInterface = (IRunable*)this;
    addRef();
    return StatusCode::SUCCESS; 
  }
  // Interface is not directly availible: try a base class
  return Service::queryInterface(riid, ppvInterface);
}

int 
LHCb::MEPRxSvc::addrFromName(const std::string &hname, u_int32_t &addr) {
  struct hostent *h;

  if (!(h = gethostbyname(hname.c_str()))) {
    *m_log << MSG::ERROR << hname << " " << hstrerror(h_errno) << endmsg;
    return 1;
  }
  addr = *((u_int32_t *) h->h_addr_list[0]);    
  return 0;
}
int 
LHCb::MEPRxSvc::nameFromAddr(u_int32_t addr, std::string &hname) {
  struct hostent *h;

  if (!(h = gethostbyaddr(&addr, 4, AF_INET))) {
    MsgStream log(msgSvc(), name());
    InAddr in(addr);
    log << MSG::ERROR << inet_ntoa(in) << " " << hstrerror(h_errno) << endmsg;
    return 1;
  } 
  hname = h->h_name;
  return 0;
}
int 
LHCb::MEPRxSvc::checkProperties() {
  MsgStream log(msgSvc(),name());
   
  if (m_ethDevIn < 0) { 
    log << MSG::ERROR << "ethDevIn must be >= 0" << endmsg;
    return 1;
  }
  if (m_maxMsForGetSpace < 1) {
    log << MSG::ERROR << "maxMsForGetSpace must be > 0" << endmsg;
    return 1;
  }
  if (m_MEPBuffers < 1 || m_MEPBuffers > 256) {
    log << MSG::ERROR << "MEPBuffers must be > 0 and < 256" << endmsg;
    return 1;
  }
  if (m_pktSamplingCount < 0) {
    log << MSG::ERROR << "pktSamplingCount must be >= 0" << endmsg;
    return 1;
  }
  if (m_maxBadPktRatio <= 0) {
    log << MSG::ERROR << "maxBadPktRatio must be > 0" << endmsg;
    return 1;
  }
  if (m_sockBuf < 0x10000 || m_sockBuf > 0x4000000) {
    log << MSG::ERROR << "sockBuf must be >= 64 kB and less than 64 MB" << endmsg;
    return 1;
  }
  if (m_IPProtoIn < 0 || m_IPProtoIn > 255) {
    log << MSG::ERROR << "IPProtoIn is an unsigned 8 bit quantity" << endmsg;
    return 1;
  }
  if (parseAddr(m_IPNameOdin.c_str(), m_IPOdin)) {
    log << MSG::ERROR << "Bad address IPOdin " << m_IPOdin << endmsg;
    return 1;
  }
  if ((m_IPSrc.size() % 2) != 0) {
    log << MSG::ERROR << "Malformed source list (length is odd)" << endmsg;
    return 1;
  }
  for (UINT i = 0; i < m_IPSrc.size(); i += 2) {
    u_int32_t addr; std::string name;

    if (parseAddr(m_IPSrc[i], addr)) {
      if (addrFromName(m_IPSrc[i + 1], addr)) {
	log << MSG::ERROR << "No correct address for source " << i << endmsg;
	return 1;
      }
      name = m_IPSrc[i + 1];
    } else {
      char tmp[16];
      if (m_IPSrc[i + 1].size() == 0) {
	if (nameFromAddr(addr, name)) name = snprintf(tmp, 15, "src-%d", 
							 i / 2);
      }
    }
    m_srcAddr[addr] = i / 2; m_srcName.push_back(name);
  }
  m_nSrc = m_IPSrc.size() / 2; 
  return 0;
}

int LHCb::MEPRxSvc::allocRx() {
  for (int i = 0; i < m_MEPBuffers; ++i) {
    LHCb::MEPRx *rx = new LHCb::MEPRx(name() + char(i+'0'), m_partitionID, m_refCount, m_MEPBufSize,
				      this, m_nSrc, m_log, m_r);
    rx->setNonBlocking(WT_FACILITY_DAQ_SPACE, false);
    m_freeDsc.push_back(rx);
  }
  m_workDsc.clear();
  *m_log   << MSG::DEBUG << printnum(m_MEPBuffers, "receive-buffer") << 
    "created" << endmsg;  
  return 0;
}
	
int LHCb::MEPRxSvc::openSock() {
  char netdev_name[10];
  MsgStream log(msgSvc(),name());
  uid_t uid;

  /* get root privs - needed until SYS_CAP_RAW available */
  uid = getuid();
  if (setuid(0)) {
    errmsg("setuid");
    return 1;
  }
  if ((m_r = socket(PF_INET, SOCK_RAW, m_IPProtoIn)) < 0) {
    errmsg("socket");
    goto drop_out;
  }
  if (setsockopt(m_r, SOL_SOCKET, SO_RCVBUF, (void *)
		 &m_sockBuf, sizeof(m_sockBuf))) {
    errmsg("setsockopt SO_RCVBUF");
    goto shut_out;
  }
  sprintf(netdev_name, m_ethDevIn < 0 ? "lo" : "eth%d", m_ethDevIn);           
  if (setsockopt(m_r, SOL_SOCKET, SO_BINDTODEVICE, (void *) netdev_name,
		 1 + strlen(netdev_name))) {
    errmsg("setsockopt SO_BINDTODEVICE");
    goto shut_out;
  }
  if (m_dynamicMEPRequest) {
    int val;
    val = MEP_REQ_TTL;
    if (setsockopt(m_r, SOL_IP, IP_TTL, &val, sizeof(int))) {
      errmsg("setsockopt SOL_IP TTL");
      goto shut_out;
    }
    val = MEP_REQ_TOS;
    if (setsockopt(m_r, SOL_IP, IP_TOS, &val, sizeof(int))) {
      errmsg("setsockopt SOL");
      goto shut_out;
    }
  } 
  log << MSG::INFO << "listening on " << netdev_name << "for IP #" << 
    m_IPProtoIn <<  " socket buffer is " << m_sockBuf / 0x400 << 
    " kB" << endmsg;
  setuid(uid);
  return 0;
shut_out:
  shutdown(m_r, SHUT_RD);
drop_out:
  setuid(uid);
  return 1;
}

inline int
LHCb::MEPRxSvc::getSrcID(u_int32_t addr)
{
 std::map<u_int32_t, int>::iterator i;

  if ((i = m_srcAddr.find(addr)) == m_srcAddr.end()) {
    InAddr in(addr);
    *m_log << MSG::ERROR << "received unexpected packet from " << inet_ntoa(in) \
	<< endmsg;
    m_notReqPkt++;
    return -1;
  }
  return i->second;
}
	
void
LHCb::MEPRxSvc::publishCounters()
{
}
       
void
LHCb::MEPRxSvc::clearCounters()
{
  memset(m_rxOct,  0, m_nCnt * sizeof(u_int64_t));
  memset(m_rxPkt,  0, m_nCnt * sizeof(u_int64_t));
  memset(m_badPkt, 0, m_nCnt * sizeof(u_int32_t));
  memset(m_misPkt, 0, m_nCnt * sizeof(u_int32_t));
  m_notReqPkt = 0;
} 

void
LHCb::MEPRxSvc::setupCounters(int n) {
  m_rxOct  = new u_int64_t[n];
  m_rxPkt  = new u_int64_t[n];
  m_badPkt = new u_int32_t[n];
  m_misPkt = new u_int32_t[n];
  m_notReqPkt = 0;
  m_nCnt = n;
}

StatusCode 
LHCb::MEPRxSvc::initialize()  {
  int sc;

  if ((sc = Service::initialize()) != StatusCode::SUCCESS) return sc;
  MsgStream log(msgSvc(),name());
  log << MSG::DEBUG << "Entering initialize....." << endmsg;
  if (checkProperties() || openSock() || allocRx()) return StatusCode::FAILURE;
  
  if (service("IncidentSvc", m_incidentSvc).isSuccess())  {
    int sc = lib_rtl_start_thread(MEPRxSvc::exec, this, &m_handle);
    if (lib_rtl_is_success(sc)) {
      return StatusCode::SUCCESS;
    }
    log << MSG::ERROR << "Failed to spawn main thread." << endmsg;
    return StatusCode::FAILURE;
  }
  log << MSG::ERROR << "Failed to access incident service." << endmsg;
  return StatusCode::FAILURE;
}
StatusCode 
LHCb::MEPRxSvc::finalize()  {
  m_receiveEvents = false;
  int sc;
  if ((sc = lib_rtl_join_thread(m_handle)) != StatusCode::SUCCESS) {
    *m_log << MSG::ERROR << "Whoops: couldn't join worker thread!" << endmsg; 
  }
  if ( m_incidentSvc ) m_incidentSvc->release();
  m_incidentSvc = NULL;
  return StatusCode::SUCCESS;
}

// IRunable implementation: Run the object
StatusCode LHCb::MEPRxSvc::run() {
  m_receiveEvents = true;
  return StatusCode::SUCCESS;
}

