// test.cpp : Defines the entry point for the console application.
//
#ifdef WIN32
#include <windows.h>
#endif
#include "oci.h"
#include "dis.hxx"
#include "dic.hxx"
#include "CONFDB.h"
#include "CONFDBEXCEPTION.h"
#include "stdio.h"
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

void printtime(void)
{
  struct tm *newtime;
  char tim[25];
  time_t aclock;
  time( &aclock );   // Get time in seconds
  newtime = localtime( &aclock );   // Convert time to struct tm form 

  /* Print local time as a string */
  strncpy(tim,asctime( newtime ),24);
  tim[24]=0;
  printf( "%s: ", tim );
}
class connection
{
public:
  char src[16];
  int srcport;
  char dest[16];
  int destport;
  void connection::print()
  {
    printf("src: %s source port %d destination: %s destination port %d\n",src,srcport,dest,destport);
  }
  char *connection::appenddest(char *buf)
  {
    int ret;
    ret = sprintf(buf,"%s %d",dest,srcport);
    return buf+ret;
  }
  char *connection::appendsrc(char *buf)
  {
    int ret;
    ret = sprintf(buf,"%s",src);
    return buf+ret;
  }
};

//DimService *portserv;

class HuginRPC : public DimRpc
{
public:
  CONFDB *db;
  HuginRPC(CONFDB *, char *, char *, char*);
  virtual ~HuginRPC();
  void rpcHandler();
};
HuginRPC::HuginRPC(CONFDB *dbase, char *n, char *f_in, char *f_out) : DimRpc(n, f_in, f_out)
{
  db = dbase;
}
HuginRPC::~HuginRPC(void)
{
}

void HuginRPC::rpcHandler()
{
  char toksep[] = "|";
  char tells[1<<16];
  int len;
  int clid;
  void *p;
  int msk;

  char *srcdev;
  char *desttyp;

  memset(tells,0,sizeof(tells));
  clid  = DimServer::getClientId();
  p = getData();
  len = getSize();
  char *pp=(char*)p;

  srcdev = strtok(pp,toksep);
  desttyp = strtok(NULL,toksep);
  for ( unsigned int jj=0;jj<strlen(srcdev);jj++) srcdev[jj]=toupper(srcdev[jj]);
  string ssrc(srcdev);
  string sdtyp(desttyp);
  printtime();
  printf("%s %s\n",srcdev,desttyp);

  std::vector<string> result;
  msk = 0;
  int idev;
  try
  {
    idev  =db->PyGetDeviceID_devicename(ssrc);
    if (idev == -1)
    {
      printtime();
      printf("Source device does not exist...\n");
      tells[0] = 0;
      setData(tells,1);
      return;
    }
  }
  catch(CONFDBEXCEPTION e)
  {
    printtime();
    printf("Source device does not exist... %s\n",e.getconfDBErrString().c_str());
    tells[0] = 0;
    setData(tells,1);
    return;
  }
  try
  {
    result = db->PyGetDetailedConnBetweenDeviceDevType(ssrc,sdtyp,1,1,1);
    /*
    return is a vector of strings with the following structure
    'TTHUGIN04|TTTELL01|2|THROTTLE_data|4|THROTTLE_data|1|2|1|0|Throttle Link'
    */
  }
  catch(CONFDBEXCEPTION e)
  {
    printtime();
    printf("Exception during processing...%s\n",e.getconfDBErrString().c_str());
    tells[0] = 0;
    setData(tells,1);
    return;
  }
  len = result.size();
  connection *conns = (connection *)malloc(len*sizeof(connection));
  connection **oconns = (connection **)malloc(len*sizeof(connection*));
  connection **oconns1 = (connection **)malloc(len*sizeof(connection*));
  int i;
  for (i=0;i<len;i++)
  {
    char *lin = (char*)(result[i].c_str());
    char *hug,*rob,*hport,*robport;
    hug = strtok(lin,toksep);
    if (hug == NULL) break;
    rob   = strtok( NULL, toksep );
    hport = strtok( NULL, toksep);
    robport = strtok( NULL, toksep );
    robport = strtok( NULL, toksep );
    strcpy(conns[i].src,hug);
    strcpy(conns[i].dest,rob);
    sscanf(hport,"%d",&conns[i].srcport);
    sscanf(robport,"%d",&conns[i].destport);
  }
  int ocidx = 0;
  int j;
  for (i=0;i<len;i++)
  {
    if (conns[i].src[0]!= 0)
    {
      oconns[ocidx] = &conns[i];
      ocidx++;
      for (j=i+1;j<len;j++)
      {
        if ((strcmp(conns[i].src,conns[j].src)== 0)&&(strcmp(conns[i].dest,conns[j].dest)== 0))
        {
          memset(&conns[j],0,sizeof(connection));
        }
      }
    }
  }
  int cidx=0;
  for (i=0;i<ocidx;i++)
  {
    if (oconns[i] != 0)
    {
      oconns1[cidx] = oconns[i];
      cidx++;
      for (j=i+1;j<ocidx;j++)
      {
        if(oconns[j] != 0)
        {
          if (strcmp(oconns[i]->src,oconns[j]->src) == 0)
          {
            oconns1[cidx] = oconns[j];
            cidx++;
            oconns[j] = 0;
          }
        }
      }
    }
  }
  char obuff[1<<16];
  memset(obuff,0,sizeof(obuff));
  char *cptr = obuff;
  cptr  = oconns1[0]->appendsrc(cptr);
  *cptr = '|';
  cptr++;
  cptr  = oconns1[0]->appenddest(cptr);
  //oconns1[0]->print();
  for (i=1;i<ocidx;i++)
  {
    if (strcmp(oconns1[i-1]->src,oconns1[i]->src) != 0)
    {
      *cptr  = '\\';
      cptr++;
      cptr  = oconns1[i]->appendsrc(cptr);
      *cptr = '|';
      cptr++;
      cptr  = oconns1[i]->appenddest(cptr);
    }
    else
    {
      *cptr = '|';
      cptr++;
      cptr  = oconns1[i]->appenddest(cptr);
    }
    //oconns1[i]->print();
  }
  *cptr++ = 0;
  setData(obuff,strlen(obuff)+1);
  printtime();
  printf("end of processing\n");
  free (conns);
  free (oconns);
  free (oconns1);
}

#include "errno.h"
int main(/*int argc, char* argv[]*/)
{
  CONFDB *db;
//  int dum[21];
  std::vector<string> result;

  char *cern=getenv("ATCERN");
  char  *pdbase,*puser,*ppassw;
  if (cern != 0)
  {
    /*string*/ pdbase="devdb";
    /*string*/ puser="lhcb_configuration_dev";
    /*string*/ ppassw="T_75_op_LbDB009";
  }
  else
  {
    /*string*/ pdbase="confdb";
    /*string*/ puser="conf_con_writer";
    /*string*/ ppassw="wr1ter";
  }
  string dbase(pdbase);
  string user(puser);
  string passw(ppassw);
  db = new CONFDB((char*)dbase.c_str(),(char*)user.c_str(),(char*)passw.c_str());
  int success=0;
  while (success==0)
  {
    try
    {
      db->PyDBConnexion();
      success=1;
    }
    catch(...)
    {
      printtime();
      printf("Cannot connect to database. retrying...\n");
      sleep(10);
    }
  }
  HuginRPC *rpc;
  rpc = new HuginRPC(db,"ConfDBHuginTells","C","C");
  DimServer *srvr  = new DimServer();
  srvr->autoStartOn();
  srvr->start("ConfDBHugin");
  while (1)
  {
    pause();
    if ((errno == EINTR))
      {
	      printf("Errno = EINTR Exiting...\n");
	      break;
      }
  }
  return 0;
}
