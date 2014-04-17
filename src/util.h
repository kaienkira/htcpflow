#ifndef HTCPFLOW_UTIL_H
#define HTCPFLOW_UTIL_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void hexdump(const char *buffer, size_t size);
void die(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif

