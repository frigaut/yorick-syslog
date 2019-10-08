/*************************************************************************\
 * yorick-syslog.c
 *
 * Matthieu Bec, 12Jan2010.
 * 
\*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <syslog.h>

#include "ydata.h"
#include "pstdlib.h"


#define SYSLOG_MSG_LENGTH       1024    /* Max bytes in a single message    */
static struct sockaddr_in server_addr;
static int msg_socket = 0;

char this_identity[128] = { '\0' };
char this_host[128] = { '\0' };
int this_port;

int yorick_syslog_verbose = 0;

/*****************************************************************\
 * private
\*****************************************************************/

static int get_server_addr(char *host, int port) {
   
   memset((char *)&server_addr, 0, sizeof(struct sockaddr_in));
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = port;
   if ((server_addr.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE) {
      struct hostent *he;
      if ((he = gethostbyname(host)) == NULL) {
         if (yorick_syslog_verbose) fprintf(stderr,"syslog: unable to resolve %s:%d\n",host,port);
         return -1;
      }
    /* server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)he->h_addr_list[0])); */
    server_addr.sin_addr.s_addr = *(struct in_addr **)he->h_addr_list[0];
   }
   
   return 0;
}

static int get_client_socket() {
   if ( (msg_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {
       if (yorick_syslog_verbose) fprintf(stderr,"syslog: unable to get socket\n");
       msg_socket = 0;
       return -1;
   }
   return 0;
}

static int logmsg(int level, char *msg) {
   char level_str[8];
   char buffer[SYSLOG_MSG_LENGTH+1];
   int facility = LOG_USER; /* pre defined */
   
   switch (level) {
      case LOG_EMERG:   strcpy(level_str,"EMERG");   break;
      case LOG_ALERT:   strcpy(level_str,"ALERT");   break;
      case LOG_CRIT:    strcpy(level_str,"CRIT");    break;
      case LOG_ERR:     strcpy(level_str,"ERR");     break;   
      case LOG_WARNING: strcpy(level_str,"WARN");    break;
      case LOG_NOTICE:  strcpy(level_str,"NOTICE");  break;
      case LOG_INFO:    strcpy(level_str,"INFO");    break;
      case LOG_DEBUG:   strcpy(level_str,"DEBUG");   break;
      default:          strcpy(level_str,"DEFAULT"); break;
   }
   
   snprintf(buffer,sizeof(buffer), "<%d>%s: %s ", facility|level, this_identity, level_str);
   strncat(buffer,msg,sizeof(buffer));
   
   if (sendto(msg_socket, buffer, strlen(buffer), MSG_DONTWAIT,(struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) < 0 ) {
      return -1;
   }
   
   return 0;
}



/*****************************************************************\
 * Y__syslog_open
\*****************************************************************/

void Y__syslog_open(int nArgs)
{
   char *host = YGetString(sp - nArgs + 1);
   int port = YGetInteger(sp - nArgs + 2);
   char *identity = YGetString(sp - nArgs + 3);
   
   if (get_server_addr(host,port))
      return;
   
   if (get_client_socket())
      return;

   /* update globals */
   strncpy(this_identity,identity,sizeof(this_identity));
   strncpy(this_host,host,sizeof(this_host));
   this_port=port;
   
}

/*****************************************************************\
 * Y_syslog_write
\*****************************************************************/

void Y_syslog_write(int nArgs)
{
   
   int level = YGetInteger(sp - nArgs + 1);
   char *msg = YGetString(sp - nArgs + 2);
   
   if (!strlen(this_identity)) {
      if (yorick_syslog_verbose) fprintf(stderr,"syslog: syslog_open needs to be called first!\n");
      return;
   }
   
   if ( logmsg(level,msg) ) {
      /* try a new socket */
      if (get_client_socket()) {
         if (yorick_syslog_verbose) fprintf(stderr,"syslog: unable to send [%d] %s\n",level,msg);
         return;
      }
      if ( logmsg(level,msg) ) {
         if (yorick_syslog_verbose) fprintf(stderr,"syslog: unable to send [%d] %s\n",level,msg);
      }
   }
}

/*****************************************************************\
 * Y_syslog_close
\*****************************************************************/

void Y_syslog_close(int nArgs)
{
   close(msg_socket);
   *this_identity = 0;
}
