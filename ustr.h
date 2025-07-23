#ifndef USTR_H
#define USTR_H

#include <stdint.h>

typedef struct UStr {
    int codepoints;
    int bytes;
    uint8_t is_ascii;
    char* contents;
} UStr;

UStr new_ustr(char* contents);
void print_ustr(UStr s);
void free_ustr(UStr s);

int32_t len(UStr s);

UStr substring(UStr s, int32_t start, int32_t end);
UStr concat(UStr s1, UStr s2);
UStr removeAt(UStr s, int32_t index);
UStr reverse(UStr s);

#endif // USTR_H

