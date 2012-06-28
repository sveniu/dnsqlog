#include <stdio.h>
#include <stdarg.h>
#include "log.h"

/* FIXME: If logging hasn't been initialized, log to stderr if there's
 * a terminal
 */

int syslog_enabled = 0;

/* from Pound */
void logmsg(const int priority, const char *fmt, ...)
{
	char buf[MAXBUF + 1];
	va_list ap;

	buf[MAXBUF] = '\0';
	va_start(ap, fmt);
	vsnprintf(buf, MAXBUF, fmt, ap);
	va_end(ap);

	if(syslog_enabled)
		syslog(LOG_FACILITY | priority, "%s", buf);
	else /* FIXME: Check if we have a terminal? */
		fprintf(stderr, "%s\n", buf);

	return;
}

void syslog_enable()
{
	syslog_enabled = 1;
}

void syslog_disable()
{
	syslog_enabled = 0;
}

/* FIXME: check return code */
int log_init(char *ident)
{
	openlog(ident, LOG_CONS | LOG_PID, LOG_FACILITY);
	return 0;
}

/* FIXME: check return code */
int log_end()
{
	closelog();
	return 0;
}

