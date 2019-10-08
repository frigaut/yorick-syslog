// rlwrap yorick

require, "syslog.i"

//syslog_open,"myst",ident="myapp";
syslog_open,"myst.cl.gemini.edu",ident="myapp";
//syslog_open,"172.16.65.199",ident="myapp";
syslog_write,LOG_INFO,"hello world from francois";
syslog_write,LOG_NOTICE,"some notice from the same";
//syslog_write,LOG_DEBUG,"foo";
syslog_close;

