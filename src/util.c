#include "util.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define LINE_CHAR_COUNT 16

void hexdump(const char *buffer, size_t size)
{
    const char *buffer_end = buffer + size;
    const char *line_start = buffer;

    for (;;) {
        const char *line_end = line_start + LINE_CHAR_COUNT > buffer_end
                             ? buffer_end
                             : line_start + LINE_CHAR_COUNT;

        if (line_start == line_end) {
            break;
        }   

        char output[1024];
        size_t count = 0;
        size_t blank_count = LINE_CHAR_COUNT;

        // hex part
        for (const char *p = line_start; p < line_end; ++p, --blank_count) {
            count += snprintf(output + count, sizeof(output), "%02hhx ", *p);
        }   
        for (size_t i = 0; i < blank_count; ++i) {
            count += snprintf(output + count, sizeof(output), "   ");
        }   

        // blank
        count += snprintf(output + count, sizeof(output), "    ");

        // acsii part
        for (const char *p = line_start; p < line_end; ++p) {
            count += snprintf(output + count, sizeof(output), "%c",
                              isprint(*p) ? *p : '.');
        }   

        printf("%s\n", output);

        line_start = line_end;
    }   
}

void die(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");

    exit(1);
}
