#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pcap/pcap.h>

#include "tcpip.h"
#include "util.h"

bool g_option_display_header = true;
bool g_option_color = false;
int g_datalink_header_length = 0;

void print_usage(const char *prog_name)
{
    fprintf(stderr, "usage: %s [-Ce] <pcap_file> \n", prog_name);
    fprintf(stderr, "   -C do not display packet description\n");
    fprintf(stderr, "   -e output in alternating colors\n");
}

void packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes)
{
    if (h->len <= g_datalink_header_length) {
        die("invalid datalink packet length");
    }

    process_ip_packet(&h->ts, (const char *)bytes + g_datalink_header_length,
                      h->len - g_datalink_header_length);
}

int main(int argc, char *argv[])
{
    char errbuf[PCAP_ERRBUF_SIZE];
    int opt = -1;

    while ((opt = getopt(argc, argv, "Ce")) != -1) {
        switch (opt) {
        case 'C':
            g_option_display_header = false;
            break;
        case 'e':
            g_option_color = true;
            break;
        default:
            print_usage(argv[0]);
            exit(1);
        }
    }

    if (optind >= argc) {
        print_usage(argv[0]);
        exit(1);
    }

    // open pcap file
    const char *pcap_file_name = argv[optind];
    pcap_t *pd = pcap_open_offline(pcap_file_name, errbuf);
    if (NULL == pd) {
        die("%s", errbuf);
    }

    // get datalink heaer length
    int dlt = pcap_datalink(pd);
    if (DLT_NULL == dlt) {
        g_datalink_header_length = 4;
    } else if (DLT_RAW == dlt) {
        g_datalink_header_length = 0;
    } else if (DLT_EN10MB == dlt || DLT_IEEE802 == dlt) {
        g_datalink_header_length = 14;
    } else if (DLT_PPP == dlt) {
        g_datalink_header_length = 4;
    } else if (DLT_LINUX_SLL == dlt) {
        g_datalink_header_length = 16;
    } else {
        fprintf(stderr, "unknown datalink type\n");
        return -1;
    }

    // compile and install the filter
    struct bpf_program filter;
    if (pcap_compile(pd, &filter, "tcp", 1, 0) != 0) {
        die("%s", pcap_geterr(pd));
    }
    if (pcap_setfilter(pd, &filter) != 0) {
        die("%s", pcap_geterr(pd));
    }
    
    // start loop
    if (pcap_loop(pd, -1, packet_handler, NULL) != 0) {
        die("%s", pcap_geterr(pd));
    }

    return 0;
}
