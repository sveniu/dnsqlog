#ifndef DNSQLOG_H
#define DNSQLOG_H

#include <stdarg.h>
#include <stdint.h>
#include "hash.h"

/* Externs from options.c */
extern int getcmdline(int argc, char **argv);
extern uint64_t interval;
extern int daemonize;
extern int ignoretime;

/* Externs from hash.c */
extern record_t *records;
extern void add_or_update(record_key_t *record); /* FIXME: Ptr? */
extern void print_records();
extern void delete_record(record_key_t *k);
extern void delete_all();

/* Externs from dns.c */
extern dns_question_t *dns_parse_question(char *dns_msg);
extern int dns_name_format(char *qname);

/* Externs from pcap.c */
extern char *get_dns_payload(const u_char *packet);
extern int pcap_init();
extern int pcap_end();
extern pcap_t *descr;
extern char *bpf;
extern int pcap_fn();

/* Externs from log.c */
extern void logmsg(const int priority, const char *fmt, ...);
extern int log_init(char *ident);
extern int log_end();
extern void syslog_enable();
extern void syslog_disable();
extern int syslog_enabled;

/* Externs from signal.c */
extern void sighand_all(int signum);

/* Externs from state.c */
extern int save_state();
extern int load_state();

static char progname[20] = "dnsqlog";
int exit_request = 0;

#endif
