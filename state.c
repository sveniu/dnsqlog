#include <stdio.h>	/* fopen, fgets, sscanf, etc */
#include <stdlib.h>	/* malloc, memset */
#include <errno.h>	/* errno */
#include <string.h>	/* strerror */
#include "state.h"
#include "log.h"
#include "util.h"

int load_state()
{
	FILE *fp;
	char line[LINEMAX];
	record_t *r;

	if((fp = fopen(statefile, "r")) == NULL)
	{
		logmsg(LOG_WARNING, "load_state():fopen() failed: %s", strerror(errno));
		return 1;
	}

	while(fgets(line, LINEMAX, fp) != NULL)
	{
		r = (record_t*)malloc(sizeof(record_t));
		memset(r, 0, sizeof(record_t));

		/* time_t lastseen,
		 * uint64_t hitcount,
		 * uint16_t dnsclass,
		 * uint16_t dnstype,
		 * char * name
		 */
		if(sscanf(line, "%ld %llu %hu %hu %s\n",
				&r->lastseen,
				(unsigned long long *)&r->hitcount,
				(unsigned short int *)&r->key.dnsclass,
				(unsigned short int *)&r->key.dnstype,
				r->key.name) != 5)
			return 2;
		add_record(r);
	}

	fclose(fp);
	return 0;
}

int save_state()
{
	FILE *fp;
	record_t *r;

	/* FIXME: Write to tempfile, then rename */
	if((fp = fopen(statefile, "w")) == NULL)
	{
		logmsg(LOG_WARNING, "load_state():fopen() failed: %s", strerror(errno));
		return 1;
	}

	for(r = records; r != NULL; r = r->hh.next) {
		fprintf(fp, "%ld %llu %hu %hu %s\n",
				(long int)r->lastseen,
				(unsigned long long)r->hitcount,
				(unsigned short int)r->key.dnsclass,
				(unsigned short int)r->key.dnstype,
				r->key.name);
	}

	fclose(fp);
	return 0;
}
