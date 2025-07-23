#include "list.h"
#include "ustr.h"
#include "pset1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

List new_list(int32_t initial_capacity) {
    List list;
    list.size = 0;
    list.capacity = initial_capacity > 0 ? initial_capacity : 1;
    list.data = (UStr*)malloc(list.capacity * sizeof(UStr));
    if (list.data == NULL) { fprintf(stderr, "Error: Failed to allocate new list array.\n"); list.capacity = 0; }
    return list;
}

List new_list_from_array(UStr* array, int32_t size) {
    List list = new_list(size);
    if (list.data == NULL) { return list; }

    for (int32_t i = 0; i < size; i++) {
        list.data[i] = new_ustr(array[i].contents);
        if (list.data[i].contents == NULL) {
            for (int32_t j = 0; j < i; j++) { free_ustr(list.data[j]); }
            free(list.data); list.data = NULL; list.size = 0; list.capacity = 0;
            fprintf(stderr, "Error: Failed to copy UStr into new list from array.\n");
            return list;
        }
        list.size++;
    }
    return list;
}

void free_list(List* list) {
    if (list == NULL || list->data == NULL) { return; }
    for (int32_t i = 0; i < list->size; i++) { free_ustr(list->data[i]); }
    free(list->data);
    list->data = NULL; list->size = 0; list->capacity = 0;
}

int8_t append(List* list, UStr s) {
    if (list == NULL) return 0;
    if (list->size == list->capacity) {
        int32_t new_capacity = list->capacity * 2;
        if (new_capacity == 0) new_capacity = 1;
        UStr* new_array = (UStr*)realloc(list->data, new_capacity * sizeof(UStr));
        if (new_array == NULL) { fprintf(stderr, "Error: Failed to reallocate list for append.\n"); return 0; }
        list->data = new_array;
        list->capacity = new_capacity;
    }
    list->data[list->size] = new_ustr(s.contents);
    if (list->data[list->size].contents == NULL) { fprintf(stderr, "Error: Failed to copy UStr for append.\n"); return 0; }
    list->size++;
    return 1;
}

UStr join(List* list, UStr separator) {
    UStr joined_s;
    joined_s.codepoints = 0; joined_s.bytes = 0; joined_s.is_ascii = 1; joined_s.contents = NULL;

    if (list == NULL || list->size == 0) { return new_ustr(""); }

    int total_bytes = 0;
    for (int32_t i = 0; i < list->size; i++) {
        total_bytes += list->data[i].bytes;
        if (!list->data[i].is_ascii) { joined_s.is_ascii = 0; }
    }
    if (list->size > 1) {
        total_bytes += (list->size - 1) * separator.bytes;
        if (!separator.is_ascii) { joined_s.is_ascii = 0; }
    }

    joined_s.contents = (char*)malloc(total_bytes + 1);
    if (joined_s.contents == NULL) { fprintf(stderr, "Error: Failed to allocate memory for joined string.\n"); return new_ustr(""); }
    joined_s.contents[0] = '\0';

    for (int32_t i = 0; i < list->size; i++) {
        strcat(joined_s.contents, list->data[i].contents);
        if (i < list->size - 1) { strcat(joined_s.contents, separator.contents); }
    }
    joined_s.contents[total_bytes] = '\0';

    joined_s.bytes = total_bytes; // Explicitly set the bytes count

    UStr temp_ustr_for_recalc = new_ustr(joined_s.contents);
    joined_s.codepoints = temp_ustr_for_recalc.codepoints;
    joined_s.is_ascii = temp_ustr_for_recalc.is_ascii;
    free_ustr(temp_ustr_for_recalc);

    return joined_s;
}

int8_t insert(List* list, UStr s, int32_t index) {
    if (list == NULL || index < 0 || index > list->size) { return 0; }
    if (list->size == list->capacity) {
        int32_t new_capacity = list->capacity * 2;
        if (new_capacity == 0) new_capacity = 1;
        UStr* new_array = (UStr*)realloc(list->data, new_capacity * sizeof(UStr));
        if (new_array == NULL) { fprintf(stderr, "Error: Failed to reallocate list for insert.\n"); return 0; }
        list->data = new_array;
        list->capacity = new_capacity;
    }

    for (int32_t i = list->size; i > index; i--) { list->data[i] = list->data[i - 1]; }

    list->data[index] = new_ustr(s.contents);
    if (list->data[index].contents == NULL) {
        fprintf(stderr, "Error: Failed to copy UStr for insert.\n");
        for (int32_t i = index; i < list->size; i++) { list->data[i] = list->data[i + 1]; }
        return 0;
    }
    list->size++;
    return 1;
}

int8_t listRemoveAt(List* list, int32_t index) {
    if (list == NULL || list->data == NULL || index < 0 || index >= list->size) { return 0; }
    free_ustr(list->data[index]);
    for (int32_t i = index; i < list->size - 1; i++) { list->data[i] = list->data[i + 1]; }
    list->size--;
    return 1;
}

List split(UStr s, UStr separator) {
    List result_list = new_list(10);
    if (result_list.data == NULL) { return result_list; }

    if (s.contents == NULL || s.codepoints == 0) {
        UStr empty_ustr = new_ustr("");
        if (empty_ustr.contents != NULL) { append(&result_list, empty_ustr); free_ustr(empty_ustr); }
        return result_list;
    }

    if (separator.contents == NULL || separator.codepoints == 0) {
        if (!append(&result_list, s)) { free_list(&result_list); return new_list(0); }
        return result_list;
    }

    char* current_pos = s.contents;
    char* found_sep;

    while ((found_sep = strstr(current_pos, separator.contents)) != NULL) {
        int segment_bytes_len = found_sep - current_pos;
        char* temp_segment_str = (char*)malloc(segment_bytes_len + 1);
        if (temp_segment_str == NULL) { fprintf(stderr, "Error: Failed to allocate split segment.\n"); free_list(&result_list); return new_list(0); }
        strncpy(temp_segment_str, current_pos, segment_bytes_len);
        temp_segment_str[segment_bytes_len] = '\0';

        UStr segment_ustr = new_ustr(temp_segment_str);
        free(temp_segment_str);
        if (segment_ustr.contents == NULL) { fprintf(stderr, "Error: Failed to create UStr for split segment.\n"); free_list(&result_list); return new_list(0); }

        if (!append(&result_list, segment_ustr)) { fprintf(stderr, "Error: Failed to append UStr during split.\n"); free_ustr(segment_ustr); free_list(&result_list); return new_list(0); }
        free_ustr(segment_ustr);

        current_pos = found_sep + separator.bytes;
    }

    if (*current_pos != '\0') {
        UStr remaining_ustr = new_ustr(current_pos);
        if (remaining_ustr.contents == NULL) { fprintf(stderr, "Error: Failed to create UStr for remaining split part.\n"); free_list(&result_list); return new_list(0); }
        if (!append(&result_list, remaining_ustr)) { fprintf(stderr, "Error: Failed to append remaining UStr during split.\n"); free_ustr(remaining_ustr); free_list(&result_list); return new_list(0); }
        free_ustr(remaining_ustr);
    } else if (result_list.size == 0 && s.codepoints > 0) {
        UStr whole_string_ustr = new_ustr(s.contents);
        if (whole_string_ustr.contents != NULL) { append(&result_list, whole_string_ustr); free_ustr(whole_string_ustr); }
    }

    return result_list;
}

