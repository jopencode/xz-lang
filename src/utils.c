#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

char *readFile(const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(f);
        return NULL;
    }
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);
    return buffer;
}

// TODO: support negative values
int parseInt(const char *start, int length) {
    int result = 0;
    int sign = 1;
    const char *p = start;
    const char *end = start + length;

    while (p < end) {
        result = result * 10 + (*p - '0');
        p++;
    }

    return result * sign;
}
