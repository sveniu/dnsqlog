#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include "dns.h"
#include "uthash.h"

typedef dns_question_t record_key_t;

typedef struct
{
	record_key_t key;
	time_t lastseen;
	uint64_t hitcount;
	UT_hash_handle hh;
} record_t;

/* Externs from options.c */
extern int saveunknown;

#endif
