#include "pset1.h"
#include <stdio.h>
#include <string.h>

int get_codepoint_len(const char* utf8_char) {
    if (utf8_char == NULL) return 0;
    unsigned char c = (unsigned char)utf8_char[0];
    if (c <= 0x7F) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 0;
}

uint32_t get_codepoint_from_bytes(const char* bytes) {
    if (bytes == NULL) return 0;
    unsigned char c = (unsigned char)bytes[0];
    int len = get_codepoint_len(bytes);
    uint32_t codepoint = 0;

    switch (len) {
        case 1: codepoint = c; break;
        case 2:
            if ((bytes[1] & 0xC0) != 0x80) return 0;
            codepoint = ((uint32_t)(c & 0x1F) << 6) | ((uint32_t)(bytes[1] & 0x3F)); break;
        case 3:
            if (((bytes[1] & 0xC0) != 0x80) || ((bytes[2] & 0xC0) != 0x80)) return 0;
            codepoint = ((uint32_t)(c & 0x0F) << 12) | ((uint32_t)(bytes[1] & 0x3F) << 6) | ((uint32_t)(bytes[2] & 0x3F)); break;
        case 4:
            if (((bytes[1] & 0xC0) != 0x80) || ((bytes[2] & 0xC0) != 0x80) || ((bytes[3] & 0xC0) != 0x80)) return 0;
            codepoint = ((uint32_t)(c & 0x07) << 18) | ((uint32_t)(bytes[1] & 0x3F) << 12) | ((uint32_t)(bytes[2] & 0x3F) << 6) | ((uint32_t)(bytes[3] & 0x3F)); break;
        default: return 0;
    }
    return codepoint;
}

void get_bytes_from_codepoint(uint32_t codepoint, char* buffer) {
    if (buffer == NULL) return;
    if (codepoint <= 0x7F) { buffer[0] = (char)codepoint; }
    else if (codepoint <= 0x7FF) { buffer[0] = (char)(0xC0 | (codepoint >> 6)); buffer[1] = (char)(0x80 | (codepoint & 0x3F)); }
    else if (codepoint <= 0xFFFF) { buffer[0] = (char)(0xE0 | (codepoint >> 12)); buffer[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F)); buffer[2] = (char)(0x80 | (codepoint & 0x3F)); }
    else if (codepoint <= 0x10FFFF) { buffer[0] = (char)(0xF0 | (codepoint >> 18)); buffer[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F)); buffer[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F)); buffer[3] = (char)(0x80 | (codepoint & 0x3F)); }
}

int utf8_strlen(const char* s) {
    if (s == NULL) return 0;
    int codepoints = 0;
    const char* ptr = s;
    while (*ptr != '\0') {
        int char_len = get_codepoint_len(ptr);
        if (char_len == 0) { ptr++; }
        else { codepoints++; ptr += char_len; }
    }
    return codepoints;
}

