/*
 * Copyright (c) 2003 Sun Microsystems, Inc.  All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistribution of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistribution in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of Sun Microsystems, Inc. or the names of
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * This software is provided "AS IS," without a warranty of any kind.
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED.
 * SUN MICROSYSTEMS, INC. ("SUN") AND ITS LICENSORS SHALL NOT BE LIABLE
 * FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.  IN NO EVENT WILL
 * SUN OR ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA,
 * OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR
 * PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF
 * LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE,
 * EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(HAVE_CONFIG_H)
# include "config.h"
#endif

//#if defined(IPMI_INTF_LAN) || defined (IPMI_INTF_LANPLUS)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <netdb.h>
//#endif
#include "ipmi_constants.h"

#include "ipmi_intf.h"
#include "log.h"
#include "helper.h"
#include "ipmi_session.h"
//#include <ipmitool/ipmi.h>
#include "ipmi_sdr.h"
//#include <ipmitool/log.h>

#define IPMI_DEFAULT_PAYLOAD_SIZE   25

//#ifdef IPMI_INTF_OPEN
//extern struct ipmi_intf ipmi_open_intf;
//#endif
//#ifdef IPMI_INTF_IMB
//extern struct ipmi_intf ipmi_imb_intf;
//#endif
//#ifdef IPMI_INTF_LIPMI
//extern struct ipmi_intf ipmi_lipmi_intf;
//#endif
//#ifdef IPMI_INTF_BMC
//extern struct ipmi_intf ipmi_bmc_intf;
//#endif
//#ifdef IPMI_INTF_LAN
//extern struct ipmi_intf ipmi_lan_intf;
//#endif
//#ifdef IPMI_INTF_LANPLUS
//extern struct ipmi_intf ipmi_lanplus_intf;
//#endif
//#ifdef IPMI_INTF_FREE
//extern struct ipmi_intf ipmi_free_intf;
//#endif
//#ifdef IPMI_INTF_SERIAL
//extern struct ipmi_intf ipmi_serial_term_intf;
//extern struct ipmi_intf ipmi_serial_bm_intf;
//#endif
//#ifdef IPMI_INTF_DUMMY
//extern struct ipmi_intf ipmi_dummy_intf;
//#endif

#if 0
struct ipmi_intf * ipmi_intf_table[] = {
#ifdef IPMI_INTF_OPEN
  &ipmi_open_intf,
#endif
#ifdef IPMI_INTF_IMB
  &ipmi_imb_intf,
#endif
#ifdef IPMI_INTF_LIPMI
  &ipmi_lipmi_intf,
#endif
#ifdef IPMI_INTF_BMC
  &ipmi_bmc_intf,
#endif
#ifdef IPMI_INTF_LAN
  &ipmi_lan_intf,
#endif
#ifdef IPMI_INTF_LANPLUS
  &ipmi_lanplus_intf,
#endif
#ifdef IPMI_INTF_FREE
  &ipmi_free_intf,
#endif
#ifdef IPMI_INTF_SERIAL
  &ipmi_serial_term_intf,
  &ipmi_serial_bm_intf,
#endif
#ifdef IPMI_INTF_DUMMY
  &ipmi_dummy_intf,
#endif
  NULL
};
#endif
//static struct ipmi_cmd ipmitool_cmd_list[] = {
//  { ipmi_chassis_main, "chassis", "Get chassis status and set power state" },
//  { NULL ,"",""},
//};
//
/* ipmi_intf::ipmi_intf_print  -  Print list of interfaces
 *
 * no meaningful return code
 */
//void ipmi_intf::ipmi_intf_print(struct ipmi_intf_support * intflist)
//{
//  ipmi_intf **intf;
//  struct ipmi_intf_support * sup;
//  int def = 1;
//  int found;
//
//  lprintf(LOG_NOTICE, "Interfaces:");
//
//  for (intf = ipmi_intf_table; intf && *intf; intf++) {
//
//    if (intflist != NULL) {
//      found = 0;
//      for (sup=intflist; sup->name != NULL; sup++) {
//        if (strncmp(sup->name, (*intf)->name, strlen(sup->name)) == 0 &&
//            strncmp(sup->name, (*intf)->name, strlen((*intf)->name)) == 0 &&
//            sup->supported == 1)
//          found = 1;
//      }
//      if (found == 0)
//        continue;
//    }
//
//    lprintf(LOG_NOTICE, "\t%-12s  %s %s",
//      (*intf)->name, (*intf)->desc,
//      def ? "[default]" : "");
//    def = 0;
//  }
//  lprintf(LOG_NOTICE, "");
//}


ipmi_intf::ipmi_intf()
{
  target_addr=  IPMI_BMC_SLAVE_ADDR;
  chassis = 0;
  session = 0;
  helper = new ipmi_helper;
  oem = 0;
}
ipmi_intf::~ipmi_intf()
{
//  ipmi_close_session_cmd(this);
  this->abort = 1;
  ipmi_lan_close(this);
  if (helper !=0) {delete helper;helper = 0;};
  if (oem !=0) {delete oem;oem = 0;};
  if (auth!=0) {delete auth;auth=0;};
  if (session!=0) {delete session;session=0;};
}

/* ipmi_intf_load  -  Load an interface from the interface table above
 *                    If no interface name is given return first entry
 *
 * @name:	interface name to try and load
 *
 * returns pointer to interface structure if found
 * returns NULL on error
 */

ipmi_intf * ipmi_intf::ipmi_intf_load(char * )
{
//  ipmi_intf ** intf;
//  ipmi_intf * i;
//
//  if (name == NULL) {
//    i = ipmi_intf_table[0];
//    if (i->setup != NULL && (i->setup(i) < 0)) {
//      lprintf(LOG_ERR, "Unable to setup "
//        "interface %s", name);
//      return NULL;
//    }
//    return i;
//  }
//
//  for (intf = ipmi_intf_table;
//      ((intf != NULL) && (*intf != NULL));
//      intf++) {
//    i = *intf;
//    if (strncmp(name, i->name, strlen(name)) == 0) {
//      if (i->setup != NULL && (i->setup(i) < 0)) {
//        lprintf(LOG_ERR, "Unable to setup "
//          "interface %s", name);
//        return NULL;
//      }
//      return i;
//    }
//  }
//
//  return NULL;
  if (session == NULL)
    session = new ipmi_session(this);
  return this;
}

void ipmi_intf::ipmi_intf_session_set_hostname( char * hostname)
{
  if (session == NULL || hostname == NULL) {
    return;
  }
  if (session->hostname != NULL) {
    free(session->hostname);
    session->hostname = NULL;
  }
  session->hostname = strdup(hostname);
}

void ipmi_intf::ipmi_intf_session_set_username( char * username)
{
  if (session == NULL)
    return;

  memset(session->username, 0, 17);

  if (username == NULL)
    return;

  memcpy(session->username, username, __min(strlen(username), 16));
}

void ipmi_intf::ipmi_intf_session_set_password( char * password)
{
  if (session == NULL)
    return;

  memset(session->authcode, 0, IPMI_AUTHCODE_BUFFER_SIZE);

  if (password == NULL) {
    session->password = 0;
    return;
  }

  session->password = 1;
  memcpy(session->authcode, password,
        __min(strlen(password), IPMI_AUTHCODE_BUFFER_SIZE));
}

void ipmi_intf::ipmi_intf_session_set_privlvl( uint8_t level)
{
  if (session == NULL)
    return;

  session->privlvl = level;
}

void ipmi_intf::ipmi_intf_session_set_lookupbit( uint8_t lookupbit)
{
  if (session == NULL)
    return;

  session->v2_data.lookupbit = lookupbit;
}

void ipmi_intf::ipmi_intf_session_set_cipher_suite_id( uint8_t cipher_suite_id)
{
  if (session == NULL)
    return;

  session->cipher_suite_id = cipher_suite_id;
}

void ipmi_intf::ipmi_intf_session_set_sol_escape_char( char sol_escape_char)
{
  if (session == NULL)
    return;

  session->sol_escape_char = sol_escape_char;
}

void ipmi_intf::ipmi_intf_session_set_kgkey( char * kgkey)
{
  if (session == NULL)
    return;

  memset(session->v2_data.kg, 0, IPMI_KG_BUFFER_SIZE);

  if (kgkey == NULL)
    return;

  memcpy(session->v2_data.kg, kgkey,
        __min(strlen(kgkey), IPMI_KG_BUFFER_SIZE));
}

void ipmi_intf::ipmi_intf_session_set_port( int port)
{
  if (session == NULL)
    return;

  session->port = port;
}

void ipmi_intf::ipmi_intf_session_set_authtype( uint8_t authtype)
{
  if (session == NULL)
    return;

  /* clear password field if authtype NONE specified */
  if (authtype == IPMI_SESSION_AUTHTYPE_NONE) {
    memset(session->authcode, 0, IPMI_AUTHCODE_BUFFER_SIZE);
    session->password = 0;
  }

  session->authtype_set = authtype;
}

void ipmi_intf::ipmi_intf_session_set_timeout( uint32_t timeout)
{
  if (session == NULL)
    return;

  session->timeout = timeout;
}

void ipmi_intf::ipmi_intf_session_set_retry( int retry)
{
  if (session == NULL)
    return;

  session->retry = retry;
}

void ipmi_intf::ipmi_intf_session_cleanup()
{
  if (session == NULL) {
    return;
  }
  if (session->hostname != NULL) {
    free(session->hostname);
    session->hostname = NULL;
  }
  free(session);
  session = NULL;
}

void ipmi_intf::ipmi_cleanup()
{
//  ipmi_sdr_list_empty(this);
}

int ipmi_intf::ipmi_intf_socket_connect()
{

  struct sockaddr_storage addr;
  struct addrinfo hints;
  struct addrinfo *rp0 = NULL, *rp;
  char service[NI_MAXSERV];
//  int rc;

  if (session == NULL)
  {
    return -1;
  }


  if (session->hostname == NULL || strlen((const char *)session->hostname) == 0) {
    lprintf(LOG_ERR, "No hostname specified!");
    return -1;
  }

  /* open port to BMC */
  memset(&addr, 0, sizeof(addr));

  sprintf(service, "%d", session->port);
  /* Obtain address(es) matching host/port */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family   = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_DGRAM;   /* Datagram socket */
  hints.ai_flags    = 0;            /* use AI_NUMERICSERV for no name resolution */
  hints.ai_protocol = IPPROTO_UDP; /*  */

  if (int sgai=getaddrinfo(session->hostname, service, &hints, &rp0) != 0) {
    lprintf(LOG_ERR, "Address lookup for %s failed, Code %d",
      session->hostname,sgai);
    return -1;
  }

  /* getaddrinfo() returns a list of address structures.
  * Try each address until we successfully connect(2).
  * If socket(2) (or connect(2)) fails, we (close the socket
  * and) try the next address.
  */

  session->ai_family = AF_UNSPEC;
  for (rp = rp0; rp != NULL; rp = rp->ai_next) {
    /* We are only interested in IPv4 and IPv6 */
    if ((rp->ai_family != AF_INET6) && (rp->ai_family != AF_INET)) {
      continue;
    }

    fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (fd == -1) {
      continue;
    }

    if (rp->ai_family == AF_INET) {
      if (connect(fd, rp->ai_addr, rp->ai_addrlen) != -1) {
        memcpy(&session->addr, rp->ai_addr, rp->ai_addrlen);
        session->addrlen = rp->ai_addrlen;
        session->ai_family = rp->ai_family;
        break;  /* Success */
      }
    }  else if (rp->ai_family == AF_INET6) {
      struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)rp->ai_addr;
      char hbuf[NI_MAXHOST];
      socklen_t len;

      /* The scope was specified on the command line e.g. with -H FE80::219:99FF:FEA0:BD95%eth0 */
      if (addr6->sin6_scope_id != 0) {
        len = sizeof(struct sockaddr_in6);
        if (getnameinfo((struct sockaddr *)addr6, len, hbuf, sizeof(hbuf), NULL, 0, NI_NUMERICHOST) == 0) {
          lprintf(LOG_DEBUG, "Trying address: %s scope=%d",
            hbuf,
            addr6->sin6_scope_id);
        }
        if (connect(fd, rp->ai_addr, rp->ai_addrlen) != -1) {
          memcpy(&session->addr, rp->ai_addr, rp->ai_addrlen);
          session->addrlen = rp->ai_addrlen;
          session->ai_family = rp->ai_family;
          break;  /* Success */
        }
      } else {
        /* No scope specified, try to get this from the list of interfaces */
        struct ifaddrs *ifaddrs = NULL;
        struct ifaddrs *ifa = NULL;

        if (getifaddrs(&ifaddrs) < 0) {
          lprintf(LOG_ERR, "Interface address lookup for %s failed",
            session->hostname);
          break;
        }

        for (ifa = ifaddrs; ifa != NULL; ifa = ifa->ifa_next) {
          if (ifa->ifa_addr == NULL) {
            continue;
          }

          if (ifa->ifa_addr->sa_family == AF_INET6) {
            struct sockaddr_in6 *tmp6 = (struct sockaddr_in6 *)ifa->ifa_addr;

            /* Skip unwanted addresses */
            if (IN6_IS_ADDR_MULTICAST(&tmp6->sin6_addr)) {
              continue;
            }
            if (IN6_IS_ADDR_LOOPBACK(&tmp6->sin6_addr)) {
              continue;
            }
            len = sizeof(struct sockaddr_in6);
            if ( getnameinfo((struct sockaddr *)tmp6, len, hbuf, sizeof(hbuf), NULL, 0, NI_NUMERICHOST) == 0) {
              lprintf(LOG_DEBUG, "Testing %s interface address: %s scope=%d",
                ifa->ifa_name != NULL ? ifa->ifa_name : "???",
                hbuf,
                tmp6->sin6_scope_id);
            }

            if (tmp6->sin6_scope_id != 0) {
              addr6->sin6_scope_id = tmp6->sin6_scope_id;
            } else {
              /*
              * No scope information in interface address information
              * On some OS'es, getifaddrs() is returning out the 'kernel' representation
              * of scoped addresses which stores the scope in the 3rd and 4th
              * byte. See also this page:
              * http://www.freebsd.org/doc/en/books/developers-handbook/ipv6.html
              */
              if (IN6_IS_ADDR_LINKLOCAL(&tmp6->sin6_addr)
                  && (tmp6->sin6_addr.s6_addr16[1] != 0)) {
                addr6->sin6_scope_id = ntohs(tmp6->sin6_addr.s6_addr16[1]);
              }
            }

            /* OK, now try to connect with the scope id from this interface address */
            if (addr6->sin6_scope_id != 0) {
              if (connect(fd, rp->ai_addr, rp->ai_addrlen) != -1) {
                memcpy(&session->addr, rp->ai_addr, rp->ai_addrlen);
                session->addrlen = rp->ai_addrlen;
                session->ai_family = rp->ai_family;
                lprintf(LOG_DEBUG, "Successful connected on %s interface with scope id %d", ifa->ifa_name, tmp6->sin6_scope_id);
                break;  /* Success */
              }
            }
          }
        }
        freeifaddrs(ifaddrs);
      }
    }
    if (session->ai_family != AF_UNSPEC) {
      break;
    }
    close(fd);
    fd = -1;
  }

  /* No longer needed */
  freeaddrinfo(rp0);

  return ((fd != -1) ? 0 : -1);
}

uint16_t ipmi_intf::ipmi_intf_get_max_request_data_size()
{
  int16_t size;

  size = max_request_data_size;

  /* check if request size is not specified */
  if (!size) {
    /*
    * The IPMB standard overall message length for �non -bridging�
    * messages is specified as 32 bytes, maximum, including slave
    * address. This sets the upper limit for typical IPMI messages.
    * With the exception of messages used for bridging messages to
    * other busses or interfaces (e.g. Master Write-Read and Send Message)
    * IPMI messages should be designed to fit within this 32-byte maximum.
    * In order to support bridging, the Master Write -Read and Send Message
    * commands are allowed to exceed the 32-byte maximum transaction on IPMB
    */

    size = IPMI_DEFAULT_PAYLOAD_SIZE;

    /* check if message is forwarded */
    if (target_addr && target_addr != my_addr) {
      /* add Send Message request size */
      size += 8;
    }
  }

  /* check if message is forwarded */
  if (target_addr && target_addr != my_addr) {
    /* subtract send message request size */
    size -= 8;

    /*
    * Check that forwarded request size is not greater
    * than the default payload size.
    */
    if (size > IPMI_DEFAULT_PAYLOAD_SIZE) {
      size = IPMI_DEFAULT_PAYLOAD_SIZE;
    }

    /* check for double bridging */
    if (transit_addr && transit_addr != target_addr) {
      /* subtract inner send message request size */
      size -= 8;
    }
  }

  /* check for underflow */
  if (size < 0) {
    return 0;
  }

  return size;
}

uint16_t ipmi_intf::ipmi_intf_get_max_response_data_size()
{
  int16_t size;

  size = max_response_data_size;

  /* check if response size is not specified */
  if (!size) {
    /*
    * The IPMB standard overall message length for �non -bridging�
    * messages is specified as 32 bytes, maximum, including slave
    * address. This sets the upper limit for typical IPMI messages.
    * With the exception of messages used for bridging messages to
    * other busses or interfaces (e.g. Master Write-Read and Send Message)
    * IPMI messages should be designed to fit within this 32-byte maximum.
    * In order to support bridging, the Master Write -Read and Send Message
    * commands are allowed to exceed the 32-byte maximum transaction on IPMB
    */

    size = IPMI_DEFAULT_PAYLOAD_SIZE; /* response length with subtracted header and checksum byte */

    /* check if message is forwarded */
    if (target_addr && target_addr != my_addr) {
      /* add Send Message header size */
      size += 7;
    }
  }

  /* check if message is forwarded */
  if (target_addr && target_addr != my_addr) {
    /*
    * Some IPMI controllers like PICMG AMC Carriers embed responses
    * to the forwarded messages into the Send Message response.
    * In order to be sure that the response is not truncated,
    * subtract the internal message header size.
    */
    size -= 8;

    /*
    * Check that forwarded response is not greater
    * than the default payload size.
    */
    if (size > IPMI_DEFAULT_PAYLOAD_SIZE) {
      size = IPMI_DEFAULT_PAYLOAD_SIZE;
    }

    /* check for double bridging */
    if (transit_addr && transit_addr != target_addr) {
      /* subtract inner send message header size */
      size -= 8;
    }
  }

  /* check for underflow */
  if (size < 0) {
    return 0;
  }

  return size;
}

void ipmi_intf::ipmi_intf_set_max_request_data_size( uint16_t size)
{
  if (size < IPMI_DEFAULT_PAYLOAD_SIZE) {
    lprintf(LOG_ERR, "Request size is too small (%d), leave default size",
        size);
    return;
  }

    max_request_data_size = size;
}

void ipmi_intf::ipmi_intf_set_max_response_data_size( uint16_t size)
{
  if (size < IPMI_DEFAULT_PAYLOAD_SIZE - 1) {
    lprintf(LOG_ERR, "Response size is too small (%d), leave default size",
        size);
    return;
  }

  max_response_data_size = size;
}
