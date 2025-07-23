#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ustr.h"
#include "pset1.h"

UStr new_ustr(char* contents) {
    UStr s;
    s.contents = NULL;

    if (contents == NULL) {
        s.codepoints = 0; s.bytes = 0; s.is_ascii = 1;
        s.contents = (char*)calloc(1, sizeof(char));
        if (s.contents == NULL) { fprintf(stderr, "Error: Failed to allocate empty UStr.\n"); }
        return s;
    }

    int32_t bytes = strlen(contents);
    int32_t codepoints = utf8_strlen(contents);
    uint8_t all_ascii = (bytes == codepoints);

    char* contents_copy = (char*)malloc(bytes + 1);
    if (contents_copy == NULL) {
        fprintf(stderr, "Error: Failed to allocate UStr contents.\n");
        s.codepoints = 0; s.bytes = 0; s.is_ascii = 1;
        s.contents = (char*)calloc(1, sizeof(char));
        return s;
    }
    strcpy(contents_copy, contents);

    s.codepoints = codepoints;
    s.bytes = bytes;
    s.is_ascii = all_ascii;
    s.contents = contents_copy;

    return s;
}

int32_t len(UStr s) {
    return s.codepoints;
}

void print_ustr(UStr s) {
    if (s.contents) { printf("Contents: \"%s\"\n", s.contents); }
    else { printf("Contents: (NULL)\n"); }
    printf("Codepoints: %d\n", s.codepoints);
    printf("Bytes: %d\n", s.bytes);
    printf("Is ASCII: %d\n", s.is_ascii);
}

void free_ustr(UStr s) {
    if (s.contents != NULL) {
        free(s.contents);
        s.contents = NULL;
    }
}

UStr substring(UStr s, int32_t start, int32_t end) {
    if (s.contents == NULL || s.codepoints == 0 || start < 0 || end <= start) {
        return new_ustr("");
    }
    if (start >= s.codepoints) {
        return new_ustr("");
    }
    if (end > s.codepoints) { end = s.codepoints; }

    char* current_byte_ptr = s.contents;
    int current_codepoint_idx_global = 0;
    int substring_start_byte_offset = 0;
    int substring_end_byte_offset = 0; // Declared here

    // Find the byte offset for the 'start' codepoint
    while (*current_byte_ptr != '\0' && current_codepoint_idx_global < start) {
        int char_len = get_codepoint_len(current_byte_ptr);
        if (char_len == 0) { current_byte_ptr++; continue; }
        current_byte_ptr += char_len;
        current_codepoint_idx_global++;
    }
    substring_start_byte_offset = current_byte_ptr - s.contents;

    char* temp_byte_ptr = current_byte_ptr;
    int sub_codepoints_count = 0;
    uint8_t sub_is_ascii = 1;

    // Iterate for the correct number of codepoints for the substring
    int target_sub_codepoints = end - start;
    while (*temp_byte_ptr != '\0' && sub_codepoints_count < target_sub_codepoints) {
        int char_len = get_codepoint_len(temp_byte_ptr);
        if (char_len == 0) { temp_byte_ptr++; continue; }
        if (char_len > 1) { sub_is_ascii = 0; }
        temp_byte_ptr += char_len;
        sub_codepoints_count++;
    }
    substring_end_byte_offset = temp_byte_ptr - s.contents;

    int sub_bytes_len = substring_end_byte_offset - substring_start_byte_offset;

    if (sub_bytes_len <= 0) { return new_ustr(""); }

    char* new_contents = (char*)malloc(sub_bytes_len + 1);
    if (new_contents == NULL) {
        fprintf(stderr, "Error: Failed to allocate substring contents.\n");
        return new_ustr("");
    }

    strncpy(new_contents, s.contents + substring_start_byte_offset, sub_bytes_len);
    new_contents[sub_bytes_len] = '\0';

    UStr sub_s = { sub_codepoints_count, sub_bytes_len, sub_is_ascii, new_contents };
    return sub_s;
}

UStr concat(UStr s1, UStr s2) {
    if (s1.contents == NULL && s2.contents == NULL) { return new_ustr(""); }
    else if (s1.contents == NULL) { return new_ustr(s2.contents); }
    else if (s2.contents == NULL) { return new_ustr(s1.contents); }

    int total_bytes = s1.bytes + s2.bytes;
    char* new_contents = (char*)malloc(total_bytes + 1);
    if (new_contents == NULL) {
        fprintf(stderr, "Error: Failed to allocate concat contents.\n");
        return new_ustr("");
    }

    strcpy(new_contents, s1.contents);
    strcat(new_contents, s2.contents);
    new_contents[total_bytes] = '\0';

    UStr new_s = { s1.codepoints + s2.codepoints, total_bytes, s1.is_ascii && s2.is_ascii, new_contents };
    return new_s;
}

UStr removeAt(UStr s, int32_t index) {
    if (s.contents == NULL || s.codepoints == 0 || index < 0 || index >= s.codepoints) {
        return new_ustr(s.contents);
    }

    char* current_byte_ptr = s.contents;
    int current_codepoint_idx = 0;
    int byte_offset_to_remove = -1;
    int bytes_to_remove_len = 0;

    while (*current_byte_ptr != '\0' && current_codepoint_idx < s.codepoints) {
        int char_len = get_codepoint_len(current_byte_ptr);
        if (current_codepoint_idx == index) {
            byte_offset_to_remove = current_byte_ptr - s.contents;
            bytes_to_remove_len = char_len;
            break;
        }
        current_byte_ptr += char_len;
        current_codepoint_idx++;
    }

    if (byte_offset_to_remove == -1) { return new_ustr(s.contents); }

    int new_bytes_len = s.bytes - bytes_to_remove_len;
    if (new_bytes_len < 0) new_bytes_len = 0;

    char* new_contents = (char*)malloc(new_bytes_len + 1);
    if (new_contents == NULL) {
        fprintf(stderr, "Error: Failed to allocate removeAt contents.\n");
        return new_ustr("");
    }

    strncpy(new_contents, s.contents, byte_offset_to_remove);
    strcpy(new_contents + byte_offset_to_remove, s.contents + byte_offset_to_remove + bytes_to_remove_len);
    new_contents[new_bytes_len] = '\0';

    uint8_t new_is_ascii = 1;
    if (!s.is_ascii) {
        char* temp_ptr = new_contents;
        while (*temp_ptr != '\0') {
            if (get_codepoint_len(temp_ptr) > 1) { new_is_ascii = 0; break; }
            temp_ptr += get_codepoint_len(temp_ptr);
        }
    } else { new_is_ascii = 1; }

    UStr new_s = { s.codepoints - 1, new_bytes_len, new_is_ascii, new_contents };
    return new_s;
}

UStr reverse(UStr s) {
    if (s.contents == NULL || s.codepoints == 0) { return new_ustr(""); }

    char* new_contents = (char*)malloc(s.bytes + 1);
    if (new_contents == NULL) {
        fprintf(stderr, "Error: Failed to allocate reverse contents.\n");
        return new_ustr("");
    }
    new_contents[s.bytes] = '\0';

    char** codepoint_starts = (char**)malloc(s.codepoints * sizeof(char*));
    if (codepoint_starts == NULL) {
        fprintf(stderr, "Error: Failed to allocate codepoint_starts.\n");
        free(new_contents);
        return new_ustr("");
    }

    char* current_byte_ptr = s.contents;
    int i = 0;
    while (*current_byte_ptr != '\0' && i < s.codepoints) {
        codepoint_starts[i] = current_byte_ptr;
        current_byte_ptr += get_codepoint_len(current_byte_ptr);
        i++;
    }

    char* write_ptr = new_contents;
    for (int j = s.codepoints - 1; j >= 0; j--) {
        char* cp_start = codepoint_starts[j];
        int cp_len = get_codepoint_len(cp_start);
        strncpy(write_ptr, cp_start, cp_len);
        write_ptr += cp_len;
    }
    *write_ptr = '\0';

    UStr reversed_s = { s.codepoints, s.bytes, s.is_ascii, new_contents };
    free(codepoint_starts);
    return reversed_s;
}

