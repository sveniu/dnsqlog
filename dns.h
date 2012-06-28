#ifndef DNS_H
#define DNS_H

#include <stdint.h>

/* DNS header */
typedef struct
{
	uint16_t xid;
	uint16_t flags;
	uint16_t qdcount;
	uint16_t ancount;
	uint16_t nscount;
	uint16_t arcount;
} dns_header_t;

#define DNS_FLAG_QR 0x8000

typedef struct /* this structure will be our key */
{
	char name[255];
	uint16_t dnstype;
	uint16_t dnsclass;
} dns_question_t;

#define DNS_QCLASS_IN 1

#endif
