TARGET = bin/htcpflow
SRCS = src/main.c
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -lpcap
BUILD_DIR = build

include mak/main.mak

