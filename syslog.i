SYSLOG_VERSION = 1.0;

local syslog;
/* DOCUMENT syslog plugin:

   Available functions (for more info, help,function)
    syslog_open....................setup and open the syslog facility
    syslog_write...................write syslog message
    syslog_close...................close the syslog facility

 */

plug_in, "syslog";

SYSLOG_UDP_PORT=514;

LOG_EMERG=0;	   /* system is unusable */
LOG_ALERT=1;	   /* action must be taken immediately */
LOG_CRIT=2;	      /* critical conditions */
LOG_ERR=3;	      /* error conditions */
LOG_WARNING=4;	   /* warning conditions */
LOG_NOTICE=5;	   /* normal but significant condition */
LOG_INFO=6;	      /* informational */
LOG_DEBUG=7;	   /* debug-level messages */

func syslog_open(host,port=,ident=)
/* DOCUMENT syslog_open(host,port,ident)
   Setup the syslog facility
 */
{
   if (port==[]) port=SYSLOG_UDP_PORT;
   if (ident==[]) ident="yorick";

   _syslog_open,host,port,ident;

}

extern syslog_write;
/* DOCUMENT syslog_write(level,message)
   Writes a syslog message
 */

extern syslog_close;
/* DOCUMENT syslog_close()
   Close the syslog facility
 */


extern _syslog_open;

extern syslog_verbose;
 /* EXTERNAL yorick_syslog_verbose */
reshape, syslog_verbose, int;

