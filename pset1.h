#ifndef PSET1_H
#define PSET1_H

#include <stdint.h>

int get_codepoint_len(const char* utf8_char);
uint32_t get_codepoint_from_bytes(const char* bytes);
void get_bytes_from_codepoint(uint32_t codepoint, char* buffer);
int utf8_strlen(const char* s);

#endif // PSET1_H

