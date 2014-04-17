htcpflow
==========
A small tool print tcp flow data in hex from tcpdump pcap file.

introduction
------------
htcpflow is based on the code of tcpflow 0.21 by Jeremy Elson.

The main purpose for this small tool is that old tcpflow do not
provide a hex console output. And the new tcpflow by Simson L. Garfinkel
need a libcairo-dev which require a lot of x11 dependency on my Ubuntu server system.
I think tcpflow is a commandline tool and I do not need a pdf report really.
It is hard to compile new tcpflow and you can not disable libcairo at all.

So I wrote this tool.

compile
-------
you need install gmake gcc and libpcap-dev first, then just run
$ make release

you will get the binary at the bin dir

usage
-----
usage: htcpflow [-Ce] pcapfile
   -C do not display packet description
   -e output in alternating colors

sample
------
use tcpdump to get a pcap file
$ tcpdump tcp port 80 -w 1.pcap
use htcpflow to get a hex console output
$ htcpdump -e 1.pcap

