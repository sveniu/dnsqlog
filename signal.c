#include <signal.h>
#include "log.h"

/* Externs from log.c */
extern void logmsg(const int priority, const char *fmt, ...);

/* Externs from dnsqlog.c */
extern int exit_request;

/* Externs from state.c */
extern int save_state();

void sighand_all(int signum)
{
	switch(signum)
	{
		case SIGINT:
		case SIGTERM:
			logmsg(LOG_WARNING, "Received signal %d; "
					"scheduling exit.", signum);
			exit_request = 1;
			break;

		case SIGHUP:
			logmsg(LOG_WARNING, "Received signal %d; "
					"saving state.", signum);
			save_state();
			break;

		default:
			logmsg(LOG_WARNING, "Received unknown signal "
					"%d; ignoring.", signum);
			break;
	}
}
