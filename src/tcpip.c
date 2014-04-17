#include "tcpip.h"

#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include "util.h"

extern bool g_option_display_header;
extern bool g_option_color;

void process_ip_packet(const struct timeval *ts,
                       const char *buffer, size_t size)
{
    if (size < sizeof(struct ip)) {
        die("invalid ip packet length");
    }

    const struct ip *ip_header = (struct ip *)buffer;

    size_t ip_len = ntohs(ip_header->ip_len);
    size_t ip_header_len = ip_header->ip_hl * 4;

    if (size < ip_len || ip_len < ip_header_len) {
        die("invalid ip packet length");
    }

    process_tcp_packet(ts,
                       ntohl(ip_header->ip_src.s_addr),
                       ntohl(ip_header->ip_dst.s_addr),
                       buffer + ip_header_len,
                       ip_len - ip_header_len);
}

void process_tcp_packet(const struct timeval *ts,
                        u_int32_t src_addr, u_int32_t dst_addr,
                        const char *buffer, size_t size)
{
    static const char *colors[] = {"\033[0;34m", "\033[0;31m"};
    static int current_color = 0;
    static u_int32_t last_src_addr = 0;
    static int last_src_port = 0;
    static u_int32_t last_dst_addr = 0;
    static int last_dst_port = 0;

    if (size < sizeof(struct tcphdr)) {
        die("invalid tcp packet length");
    }

    struct tcphdr *tcp_header = (struct tcphdr *)buffer;
    size_t tcp_header_len = tcp_header->doff * 4;
    int src_port = ntohs(tcp_header->source);
    int dst_port = ntohs(tcp_header->dest);

    // empty tcp packet
    if (tcp_header_len >= size) {
        return;
    }

    if (g_option_color) {
        if (src_addr != last_src_addr || dst_addr != last_dst_addr ||
            src_port != last_src_port || dst_port != last_dst_port) {
            last_src_addr = src_addr;
            last_dst_addr = dst_addr;
            last_src_port = src_port;
            last_dst_port = dst_port;
            current_color = !current_color;
        }
        printf("%s", colors[current_color]);
    }

    // print packet description
    if (g_option_display_header) {
        struct tm tm;
        localtime_r(&ts->tv_sec, &tm);
        char format_time[1024];
        strftime(format_time, sizeof(format_time), "%Y-%m-%d %H:%M:%S", &tm);
        printf("%s.%ld %d.%d.%d.%d:%d => %d.%d.%d.%d:%d\n",
               format_time, ts->tv_usec / 1000,
               (src_addr & 0xff000000) >> 24,
               (src_addr & 0x00ff0000) >> 16,
               (src_addr & 0x0000ff00) >> 8,
               src_addr & 0x000000ff,
               src_port,
               (dst_addr & 0xff000000) >> 24,
               (dst_addr & 0x00ff0000) >> 16,
               (dst_addr & 0x0000ff00) >> 8,
               dst_addr & 0x000000ff,
               dst_port);
    }

    // print hex data
    hexdump(buffer + tcp_header_len, size - tcp_header_len);

    if (g_option_color) {
        printf("%s", "\033[0m");
    }

    printf("\n");
}

