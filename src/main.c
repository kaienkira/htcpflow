#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <pcap/pcap.h>

void print_usage(const char *prog_name)
{
    fprintf(stderr, "usage: %s [-e] <pcap_file> \n", prog_name);
    fprintf(stderr, "   -e output in alternating colors\n");
}

int main(int argc, char *argv[])
{
    char errbuf[PCAP_ERRBUF_SIZE];
    int opt = -1;
    bool opt_color = false;

    while ((opt = getopt(argc, argv, "e")) != -1) {
        switch (opt) {
        case 'e':
            opt_color = true;
            break;
        default:
            print_usage(argv[0]);
            return -1;
        }
    }

    if (optind >= argc) {
        print_usage(argv[0]);
        return -1;
    }
    const char *pcap_file_name = argv[optind];

    pcap_t *pcap_file = pcap_open_offline(pcap_file_name, errbuf);
    if (NULL == pcap_file) {
        fprintf(stderr, "%s\n", errbuf);
        return -1;
    }

    if (opt_color) {
    }

    return 0;
}
