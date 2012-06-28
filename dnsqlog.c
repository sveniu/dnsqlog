#include <stdlib.h>	/* memset, malloc */
#include <stdio.h>	/* printf */
#include <string.h>	/* strncpy, strerror */
#include <time.h>	/* time */
#include <unistd.h>	/* daemon */
#include <pcap.h>	/* u_char */
#include <signal.h>	/* signal */
#include <poll.h>	/* poll */
#include <errno.h>	/* errno */
#include "dnsqlog.h"
#include "hash.h"
#include "log.h"
#include "util.h"

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	(void) args; /* FIXME: (void) in function definition instead? */
	(void) header;
	char *dns_payload;
	dns_question_t *dns_question; /* FIXME: pointer? */

	if((dns_payload = get_dns_payload(packet)) == NULL)
	{
		/* FIXME: Anything else? */
		return;
	}

	if((dns_question = dns_parse_question(dns_payload)) == NULL)
	{
		/* FIXME: Anything else? */
		return;
	}

	/* In-place reformatting */
	if((dns_name_format(dns_question->name)) != 0)
	{
		/* FIXME: Log "Malformed DNS question QNAME" */
		/* FIXME: Store this in the struct key. */
		return;
	}

	if(dns_question->dnsclass != DNS_QCLASS_IN)
	{
		/* FIXME: Log "Non-Internet DNS class" */
		return;
	}

	add_or_update(dns_question);
	free(dns_question);
}

int main(int argc, char *argv[])
{
	/* For poll() */
	int nfds;
	struct pollfd *pfd;

	/* Parse options */
	if(getcmdline(argc, argv) != 0)
	{
		logmsg(LOG_CRIT, "getcmdline() failed");
		exit(EXIT_FAILURE);
	}

	/* Set up logging */
	if(log_init(progname) != 0)
	{
		/* FIXME: Log something */
		exit(EXIT_FAILURE);
	}

	/* Load previous state, if any */
	if(load_state() != 0)
		logmsg(LOG_NOTICE, "load_state() failed");

	/* Set up pcap */
	if(pcap_init() != 0)
	{
		/* FIXME: Log something */
		logmsg(LOG_CRIT, "pcap_init() failed");
		exit(EXIT_FAILURE);
	}

	/* Handle signals */
	signal(SIGTERM, sighand_all);
	signal(SIGINT, sighand_all);
	signal(SIGHUP, sighand_all);

	/* Daemonize and enable syslog */
	if (daemonize)
	{
		if(!daemon(1, 0))
			syslog_enable();
		else
			logmsg(LOG_INFO, "daemon() failed: %s\n", strerror(errno));
	}

	/* Stuff to poll */
	pfd = (struct pollfd *)malloc(sizeof(struct pollfd));
	pfd->fd = pcap_fn();
	pfd->events = POLLIN;
	pfd->revents = 0;

	/* FIXME: aggregating thread? */

	logmsg(LOG_NOTICE, "Starting with bpf: `%s'", bpf);

	/* Main loop */
	for(;;)
	{
		nfds = poll(pfd, 1, interval);
		if (nfds == -1 || (pfd->revents & (POLLERR|POLLHUP|POLLNVAL))) {
			if (errno != EINTR)
			{
				logmsg(LOG_ERR, "Poll error: %s", strerror(errno));
				exit_request = 1;
			}
		}

		/* Handle packet arrivals */
		if (pfd->revents) {
			(void)pcap_dispatch(descr, -1, (pcap_handler)got_packet, NULL);
		}

		if (exit_request)
		{
			logmsg(LOG_WARNING, "Exiting on system/user request.");
			break;
		}
	}

	free(pfd);
	logmsg(LOG_DEBUG, "Closing packet capture device.");
	pcap_end();
	logmsg(LOG_DEBUG, "Saving state.");
	save_state();
	logmsg(LOG_DEBUG, "Freeing hash list.");
	delete_all();	/* Delete hash */
	log_end();
	exit(EXIT_SUCCESS);
	return 0;
}
