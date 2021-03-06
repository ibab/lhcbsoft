#include "Checkpointing/MemMaps.h"
#include "Checkpointing/SysCalls.h"
#include "Checkpointing/Static.h"
#include "Checkpointing.h"
#include "Restore.h"

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <climits>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

using namespace Checkpointing;

#ifndef __STATIC__
DefineMarker(MEMMAP_BEGIN_MARKER,"MMAP");
DefineMarker(MEMMAP_END_MARKER,  "mmap");
#endif

template <class T> long default_map(const AreaHandler* ,const Area& a, const unsigned char* data, long data_len) {
  return T::do_map(a, data, data_len);
}

template <class T> long default_handle(const AreaHandler* p,int which, const Area& a) {
  return ((T*)p)->handle(which,a);
}

STATIC(long) checkpointing_area_skip(const Area& a) {
  if ( m_strcmp(a.name,"/usr/lib/locale/locale-archive") == 0 ) {
    checkpointing_area_print(&a,MTCP_INFO,"*** WARNING: Skip locale-archive:");
    return 1;
  }
  else if ( m_strcmp(a.name,"/usr/lib64/gconv/gconv-modules.cache") == 0 ) {
    checkpointing_area_print(&a,MTCP_INFO,"*** WARNING: Skip  gconv-modules.cache:");
    return 1;
  }
  else if ( m_strcmp(a.name,"/var/db/nscd/passwd") == 0 ) {
    checkpointing_area_print(&a,MTCP_INFO,"*** WARNING: Skip  /var/db/nscd/passwd:");
    return 1;
  }
  else if ( m_strcmp(a.name,"/var/db/nscd/hosts") == 0 ) { // since slc6
    checkpointing_area_print(&a,MTCP_INFO,"*** WARNING: Skip  /var/db/nscd/passwd:");
    return 1;
  }
  else if ( m_strncmp(a.name,"/var/run/nscd/",14) == 0 ) { // since slc5 (somehow new)
    checkpointing_area_print(&a,MTCP_INFO,"*** WARNING: Skip  /var/run/nscd/");
    return 1;
  }
  return 0;
}

/// Initializing constructor
AreaBaseHandler::AreaBaseHandler() : m_bytes(0), m_count(0)   {
  f_map     = default_map<AreaBaseHandler>;
  f_handle  = default_handle<AreaBaseHandler>;
}

int AreaBaseHandler::updateCounts(const Area& a)   {
  AreaBaseHandler* p = const_cast<AreaBaseHandler*>(this);
  long pg = chkpt_sys.pageSize;
  long sz = pg - 1;
  p->m_bytes += size_t((a.size+sz)/pg)*pg;
  ++p->m_count;
  return 1;
}

long AreaBaseHandler::mapArea(const Area& a, const unsigned char* in, long data_len)  {
  return checkpointing_area_map(a,0,in,data_len);
}

/// Handler callback
long AreaBaseHandler::handle(int, const Area& a)    {
  return updateCounts(a);
}

/// Initializing constructor
AreaPrintHandler::AreaPrintHandler() : AreaBaseHandler()  {
  f_handle = default_handle<AreaPrintHandler>;
}

/// Handler callback
long AreaPrintHandler::handle(int, const Area& a)    {
  checkpointing_area_print(&a,MTCP_INFO,"");
  return updateCounts(a);
}

/// Initializing constructor
AreaLibHandler::AreaLibHandler(int fd) : AreaBaseHandler(), m_fd(fd)
{
  f_handle = default_handle<AreaLibHandler>;
  m_numLibs = 0;
  m_len = 1000;
  m_libs = (char**)::malloc(m_len*sizeof(char*));
  for(char** p=m_libs; p<m_libs+m_len; ++p) *p=0;
}

AreaLibHandler::~AreaLibHandler()  {
  this->release();
}

void AreaLibHandler::release()  {
  if ( m_libs )  {
    for(char** p=m_libs; *p; ++p)
      if ( *p ) ::free(*p);
    ::free(m_libs);
    m_libs = 0;
  }
}

/// Handler callback
long AreaLibHandler::handle(int, const Area& a)    {
  // Skip images, which are no files
  if ( a.name[0] != '/' ) {
    return 0;
  }
  // Skip already saved images
  for(int i=0; i<m_numLibs; ++i)  {
    if ( m_libs[i] && m_strcmp(m_libs[i],a.name) == 0 )   {
      return 0;
    }
  }
  if ( checkpointing_area_skip(a) ) {
    return 0;
  }
  // This one we now save!
  int bytes = checkpointing_library_fwrite(m_fd, &a);
  if ( bytes > 0 ) {
    if ( m_numLibs+1 > m_len ) {
      m_libs = (char**)::realloc(m_libs,(1000+m_len)*sizeof(char*));
      for(char** p=m_libs+m_len; p<m_libs+(1000+m_len)*sizeof(char); ++p) *p=0;
      m_len += 1000;
    }
    mtcp_output(MTCP_INFO,"AreaLibHandler: Added image file[%d]: %s %d bytes\n",m_numLibs,a.name,bytes);
    m_libs[m_numLibs] = (char*)::malloc(a.name_len+1);
    m_memcpy(m_libs[m_numLibs], a.name, a.name_len+1);
    m_bytes += bytes;
    m_numLibs++;
  }
  return bytes;
}

/// Initializing constructor
AreaInfoHandler::AreaInfoHandler() {
  stack[0]           = stack[1]      = 0;
  vdso[0]            = vdso[1]       = 0;
  vsyscall[0]        = vsyscall[1]   = 0;
  imageAddr[0]       = ULONG_MAX;
  imageAddr[1]       = 0;
  checkpointAddr[0]  = ULONG_MAX;
  checkpointAddr[1]  = 0;
  highAddr           = 0;
  image[0]           = 0;
  m_prev             = false;
  f_handle = default_handle<AreaInfoHandler>;
}

/// Handler callback
long AreaInfoHandler::handle(int, const Area& a)  {
  //if ( m_prev && a.name[0] != 0 ) m_prev = false;
  if ( a.name[0]=='[' ) {
    if ( m_intcheck(a.name,"[stack]") ) {
      stack[0] = a.low;
      stack[1] = a.high;
    }
    else if ( m_intcheck(a.name,"[vdso]") ) {
      vdso[0] = a.low;
      vdso[1] = a.high;
    }
    else if ( m_intcheck(a.name,"[vsyscall]") ) {
      vsyscall[0] = a.low;
      vsyscall[1] = a.high;
    }
  }
  else if ( a.name[0] == '/' ) {
    if ( strstr(a.name,chkpt_sys.checkpointFile) ) {
      if ( checkpointAddr[0] > a.low )  checkpointAddr[0] = a.low;
      if ( checkpointAddr[1] < a.high ) checkpointAddr[1] = a.high;
    }
    else if ( strstr(a.name,"libCheckpointing.so") ) {
      m_prev = true;
      if ( !image[0] ) m_memcpy(image,a.name,sizeof(image));
      if ( imageAddr[0] > a.low )  imageAddr[0] = a.low;
      if ( imageAddr[1] < a.high ) imageAddr[1] = a.high;
    }
    else if ( strstr(a.name,"libChkpt.so") ) {
      m_prev = true;
      if ( !image[0] ) m_memcpy(image,a.name,sizeof(image));
      if ( imageAddr[0] > a.low )  imageAddr[0] = a.low;
      if ( imageAddr[1] < a.high ) imageAddr[1] = a.high;
    }
  }
  else if ( m_prev ) {
    m_prev = false;
    if ( imageAddr[0] > a.low )  imageAddr[0] = a.low;
    if ( imageAddr[1] < a.high ) imageAddr[1] = a.high;
  }
  if ( a.high > highAddr ) highAddr = a.high;
  return updateCounts(a);
}

/// Initializing constructor
AreaWriteHandler::AreaWriteHandler(int fd) : m_fd(fd)
{
  f_handle = default_handle<AreaWriteHandler>;
}

/// Handler callback
long AreaWriteHandler::handle(int, const Area& a)    {
  // Skip if the memory region is where we actually write!
  long rc;
  if ( a.high == chkpt_sys.addrEnd ) {
    checkpointing_area_print(&a,MTCP_INFO,"*** WARNING: SKIP IMAGE area:");
    return 0;
  }  
  else if ( a.low >= chkpt_sys.addrStart && a.high <= chkpt_sys.addrEnd ) {
    checkpointing_area_print(&a,MTCP_INFO,"*** WARNING: SKIP IMAGE area:");
    return 0;
  }  
  else if( m_intcheck(a.name,"[vdso]") && chkpt_sys.vsyscallStart == 0 ) {
    checkpointing_area_print(&a,MTCP_INFO,"*** WARNING: SKIP [vdso] area:");
    return 0;
  }
  else if( m_longcheck(a.name,"[vsyscall]") ) {
    checkpointing_area_print(&a,MTCP_INFO,"*** WARNING: SKIP [vsyscall] area:");
    return 0;
  }
  else if ( checkpointing_area_skip(a) ) {
    return 0;
  }
  rc = a.write(m_fd);
  if ( rc > 0 )  {
    updateCounts(a);
    m_bytes += rc;
  }
  return rc;
}

/// Initializing constructor
AreaChkptWriteHandler::AreaChkptWriteHandler(int fd) : AreaWriteHandler(fd) {
  f_handle = default_handle<AreaChkptWriteHandler>;
  m_prev = false;
}

/// Handler callback
long AreaChkptWriteHandler::handle(int, const Area& a)    {
  bool is_image = m_strcmp(a.name,chkpt_sys.checkpointImage) == 0;
  if ( m_prev || is_image ) {
    long rc = 0;
    if ( a.prot[0]=='r' ) {
      rc = m_writemem(m_fd,(void*)a.low,a.size);
    }
    else {
      rc = m_writeset(m_fd,0,a.size);
    }
    if ( rc > 0 ) m_bytes += rc;
    checkpointing_area_print(&a,MTCP_INFO,"Write raw image:");
    m_prev = is_image ? true : false;
    return rc;
  }
  m_prev = false;
  return 0;
}

/// Initializing constructor
AreaMapper::AreaMapper()   {
  f_map    = default_map<AreaMapper>;
  f_handle = default_handle<AreaMapper>;
}

/// Number of memory mapped areas/files
int MemMaps::numLines()  const {
  int fd = mtcp_sys_open("/proc/self/maps",O_RDONLY,0);
  if ( fd>0 ) {
    char p;
    int num = 0;
    for(int n=1; n>0;)   {
      n = mtcp_sys_read(fd,&p,1);
      if      ( n == 1 && p!='\n' ) continue;
      else if ( n == 1 ) ++num;
      else if ( n == 0 ) break;
    }
    mtcp_sys_close(fd);
    return num;
  }
  return -1;
}

/// Collect in process information about the memory mappings
int MemMaps::scan(AreaHandler& handler) {
  return checkpointing_memory_scan(&handler);
}

/// Dump descriptor information about the memory mappings
void MemMaps::dump() {
  AreaPrintHandler h;
  scan(h);
  mtcp_output(MTCP_INFO,"--------------- Total memory size:%ld Bytes\n",h.bytes());
}

/// Write descriptor information and data from the memory mappings to file
long MemMaps::write(int fd) {
  return checkpointing_memory_write(fd);
}

/// Read descriptor information and data from the memory mappings from file
long MemMaps::read(const void* address, AreaHandler& handler) {
  const_Pointer in = (const_Pointer)address;
  if ( in ) {
    int numArea = 0;
    in += checkMarker(in,MEMMAP_BEGIN_MARKER);
    in += getInt(in,&numArea);
    Area a;
    for(int i=0; i<numArea; ++i) {
      in += a.read(in,handler);
      handler.f_handle(&handler,i,a);
    }
    in += checkMarker(in,MEMMAP_END_MARKER);
    mtcp_output(MTCP_INFO,"Read %ld bytes of mmap area data\n",addr_diff(in,address));
    return addr_diff(in,address);
  }
  return -1;
}
