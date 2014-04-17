#ifndef HTCPFLOW_TCPIP_H
#define HTCPFLOW_TCPIP_H

#include <time.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void process_ip_packet(const struct timeval *ts,
                       const char *buffer, size_t size);
void process_tcp_packet(const struct timeval *ts,
                        u_int32_t src_addr, u_int32_t dst_addr,
                        const char *buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif

