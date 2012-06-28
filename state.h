#ifndef STATE_H
#define STATE_H

#include "hash.h"

/* Externs from hash.c */
extern record_t *records;
extern void add_record(record_t *record);

/* Externs from log.c */
extern void logmsg(const int priority, const char *fmt, ...);

/* Max DNS name length + class + type + lastseen + hitcount + etc */
#define LINEMAX 300

/* FIXME: Put somewhere non-volatile */
const char statefile[] = "dns.state";

#endif
