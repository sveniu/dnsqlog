#include <stdio.h>	/* printf, fprintf */
#include <stdlib.h>	/* malloc, strtoll */
#include <string.h>	/* strlen */
#include <unistd.h>	/* getopt */
#include <ctype.h>	/* isprint */
#include <stdint.h>	/* uint64_t */
#include "options.h"

/* Externs from pcap.c */
extern char *bpf;

char *iface = NULL;
int daemonize = 1;
uint64_t interval = 60000ULL; /* milliseconds */
int ignoretime = 60; /* ignore entries newer than n seconds */
int saveunknown = 0; /* store unknown qnames */

static void usage(char* prog)
{
	printf("Usage: %s -i <iface> [options] [bpf]\n", prog);
	printf("Options:\n");
	printf("  -w n   Status file write interval (sec).\n");
	printf("  -D     Run in foreground. Log to stderr.\n");
	printf("  -s     Save unknown qnames.\n");
}

/* from tcpdump.c */
static char *copy_argv(register char **argv)
{
	register char **p;
	register u_int len = 0;
	char *buf;
	char *src, *dst;

	p = argv;
	if (*p == 0)
		return 0;

	while (*p)
		len += strlen(*p++) + 1;

	buf = (char *)malloc(len);
	if (buf == NULL)
		fprintf(stderr, "copy_argv: malloc\n");

	p = argv;
	dst = buf;
	while ((src = *p++) != NULL) {
		while ((*dst++ = *src++) != '\0')
			;
		dst[-1] = ' ';
	}
	dst[-1] = '\0';

	return buf;
}

/* from
 * http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
 */
int getcmdline(int argc, char **argv)
{
	/* FIXME: Return -1 instead of exit? */
	int c;

	opterr = 0;

	while((c = getopt(argc, argv, "hi:w:l:sD")) != -1)
		switch (c)
		{
			case 'h':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
			case 'i':
				iface = optarg;
				break;
			case 'w':
				/* milliseconds */
				interval = strtoll(optarg, NULL, 10);
				if(interval < INTV_MIN)
				{
					fprintf(stderr, "Fatal: Interval must be >= %lld ms\n", INTV_MIN);
					usage(argv[0]);
					exit(EXIT_FAILURE);
				}
				break;
			case 'l':
				ignoretime = atoi(optarg);
				break;
			case 'D':
				daemonize = 0;
				break;
			case 's':
				saveunknown = 1;
				break;
			case '?':
				if(optopt == 'i' || optopt == 'w')
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if(isprint (optopt))
					fprintf(stderr, "Fatal: Unknown option `-%c'.\n", optopt);
				else
					fprintf(stderr, "Fatal: Unknown option character `\\x%x'.\n", optopt);
				usage(argv[0]);
				exit(EXIT_FAILURE);
			default:
				usage(argv[0]);
				exit(EXIT_FAILURE);
		}

	/* iface arg is required */
	if(iface == NULL)
	{
		fprintf(stderr, "Fatal: No interface specified.\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	/* the rest is the bpf, even if empty */
	bpf = copy_argv(&argv[optind]);

	return 0;
}
