#ifndef PCAP_H
#define PCAP_H

#include <arpa/inet.h>

/* Packet capture size */
#define SNAP_LEN 1518

/* Default data link type is Ethernet */
#define DLT_DEFAULT DLT_EN10MB

/* Header sizes */
#define HSIZE_ETHERNET	14
#define HSIZE_UDP	8
#define HSIZE_DNS	12

/* Filter string */
char *bpf = NULL;

/* IPv4 header */
/* FIXME: Where's u_char etc defined? */
typedef struct {
	u_char  ip_vhl;			/* version << 4 | header length >> 2 */
	u_char  ip_tos;			/* type of service */
	u_short ip_len;			/* total length */
	u_short ip_id;			/* identification */
	u_short ip_off;			/* fragment offset field */
	#define IP_RF 0x8000		/* reserved fragment flag */
	#define IP_DF 0x4000		/* dont fragment flag */
	#define IP_MF 0x2000		/* more fragments flag */
	#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
	u_char  ip_ttl;			/* time to live */
	u_char  ip_p;			/* protocol */
	u_short ip_sum;			/* checksum */
	struct  in_addr ip_src,ip_dst;	/* source and dest address */
	/* options */
} ipv4_header_t;

#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)

#endif
