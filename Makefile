TARGET = bin/htcpflow
SRCS = src/main.c \
       src/tcpip.c \
       src/util.c
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -lpcap
BUILD_DIR = build

include mak/main.mak

