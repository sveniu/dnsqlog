#include <stdlib.h>	/* malloc */
#include <string.h>	/* strncpy, strlen, memcpy */
#include <ctype.h>	/* isalnum, tolower */
#include <arpa/inet.h>	/* ntohs */
#include "dns.h"

/* FIXME: Return pointer instead? */
dns_question_t *dns_parse_question(char *dns_msg)
{
	dns_question_t *dns_question;
	char *pos = dns_msg;

	dns_question = (dns_question_t*)malloc(sizeof(dns_question_t));
	memset(dns_question, 0, sizeof(dns_question_t));

	/* set, find end of qname */
	while(*pos != '\0')
		pos++;

	/* Skip label length field */
	strncpy(dns_question->name, dns_msg+1, strlen(dns_msg)+1);

	/* set type and class */
	dns_question->dnstype = ntohs( *(unsigned short *)(pos + 1) );
	dns_question->dnsclass = ntohs( *(unsigned short *)(pos + 3) );

	return dns_question;
}

int dns_name_format(char *qname)
{
	char *s;

	if(*qname == '\0')
		return 1;

	for(s = qname; *s; s++)
	{

		if(!isalnum(*s) && (*s != '-'))
			*s = '.';
		*s = tolower(*s);
	}

	return 0;
}
