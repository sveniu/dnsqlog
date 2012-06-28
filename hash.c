#include <stdlib.h>	/* memset, malloc */
#include <stdio.h>	/* printf */
#include <string.h>	/* strncpy */
#include <time.h>	/* time */
#include "uthash.h"
#include "hash.h"
#include "util.h"

record_t *records = NULL;
//void add_or_update(record_key_t *record);
//void print_records();
//void delete_record(record_key_t *k);
//void delete_all();

void add_or_update(record_key_t *record_key)
{
	record_t l, *p;

	memset(&l, 0, sizeof(record_t)); /* zero fill! */
	memcpy(&l.key, record_key, sizeof(record_key_t));
	dp("Looking for name: \"%s\"\n", l.key.name);
	dp("Looking for type: %d\n", l.key.dnstype);
	dp("Looking for class: %d\n", l.key.dnsclass);
	HASH_FIND(hh, records, &l.key, sizeof(record_key_t), p);

	if(p)
	{
		p->hitcount++;
		p->lastseen = time(NULL);
		dp("Found! %llu hits, lastseen %d.\n", p->hitcount, (int)p->lastseen);
	} else if(saveunknown)
	{
		//printf("Entry not found. Adding\n");
		p = (record_t*)malloc( sizeof(record_t) );
		memset(p, 0, sizeof(record_t));
		memcpy(&p->key, record_key, sizeof(record_key_t));
		HASH_ADD(hh, records, key, sizeof(record_key_t), p);
	}
}

/* Called from state.c:load_state() */
void add_record(record_t *record)
{
	HASH_ADD(hh, records, key, sizeof(record_key_t), record);
}

void print_records(int ignoretime) {
	record_t *r;
	time_t now = time(NULL);

	printf("Hash table:\n");
	for(r=records; r != NULL; r=r->hh.next) {
		if(now - r->lastseen >= ignoretime)
			printf(" * name \"%s\": %llu hits, %d lastseen\n",
					r->key.name,
					(unsigned long long)r->hitcount,
					(int)r->lastseen);
	}
	printf("\n");
}

void delete_record(record_key_t *k) {
	record_t *r;

	HASH_FIND(hh, records, k, sizeof(record_key_t), r);
	if(r != NULL)
	{
		HASH_DEL(records, r);
		free(r);
	}
}

void delete_all() {
	record_t *p, *tmp;

	HASH_ITER(hh, records, p, tmp) {
		HASH_DEL(records, p);
		free(p);
	}
}
