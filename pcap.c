#include <pcap/pcap.h>
#include <stdlib.h>	/* FIXME: remove this */
#include "pcap.h"
#include "dns.h"

/* Extern from options.c */
extern char *iface;

pcap_t *descr;

/* FIXME:
 * use logging instead of fprintf
 * return -1 instead of exit
 */

/* FIXME: iface as argument? */
int pcap_init()
{
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program fp;

	*errbuf = '\0';
	/* FIXME: Disable promisc? */
	descr = pcap_open_live(iface, SNAP_LEN, 1, 0, errbuf);
	if (descr == NULL) {
		fprintf(stderr, "pcap_open_live failed (%s): %s\n",
				iface, errbuf);
		exit(EXIT_FAILURE);
	}
	if (*errbuf)
		fprintf(stderr, "pcap_open_live warning (%s): %s\n",
				iface, errbuf);

	if (pcap_datalink(descr) != DLT_DEFAULT) {
		fprintf(stderr, "Unsupported data link type `%s'. "
				"Use `%s' instead.\n",
				pcap_datalink_val_to_description(pcap_datalink(descr)),
				pcap_datalink_val_to_description(DLT_DEFAULT));
		exit(EXIT_FAILURE);
	}

	if (pcap_compile(descr, &fp, bpf, 0, 0) == -1) {
		fprintf(stderr, "pcap_compile failed (%s): %s\n",
				iface, pcap_geterr(descr));
		exit(EXIT_FAILURE);
	}

	if (pcap_setfilter(descr, &fp) == -1) {
		fprintf(stderr, "pcap_setfilter failed (%s): %s\n",
				iface, pcap_geterr(descr));
		exit(EXIT_FAILURE);
	}
	pcap_freecode(&fp);

	return 0;
}

int pcap_fn()
{
	return pcap_fileno(descr);
}

void pcap_end()
{
	/* FIXME: Handle errors? */
	pcap_close(descr);
	free(bpf);
}

char *get_dns_payload(const u_char *packet)
{
	char *dns_payload;
	int size_ipv4;
	const ipv4_header_t *ipv4;
	const dns_header_t *dns;

	/* Compute IPv4 header size */
	ipv4 = (ipv4_header_t*)(packet + HSIZE_ETHERNET);
	size_ipv4 = IP_HL(ipv4)*4;
	if (size_ipv4 < 20)
		return NULL; /* FIXME: Log first */

	if(ipv4->ip_p != IPPROTO_UDP)
		return NULL; /* FIXME: Log first */

	/* Point DNS header struct to the correct place */
	dns = (dns_header_t *)(packet + HSIZE_ETHERNET + size_ipv4 +
			HSIZE_UDP);
	dns_payload = (char *)(packet + HSIZE_ETHERNET + size_ipv4 +
			HSIZE_UDP + HSIZE_DNS);

	/* Skip response packets */
	if((dns->flags & DNS_FLAG_QR) == DNS_FLAG_QR)
		return NULL; /* FIXME: Log first */

	return dns_payload;
}
