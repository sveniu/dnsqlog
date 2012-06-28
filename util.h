#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>	/* fprintf */

#ifndef DEBUG
#define DEBUG 0
#endif

#define dp(fmt, ...) \
	do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
			__LINE__, __func__, ## __VA_ARGS__); } while (0)

#endif
