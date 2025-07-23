#ifndef LIST_H
#define LIST_H

#include <stdint.h>
#include "ustr.h"

typedef struct List {
    UStr* data;
    int32_t size;
    int32_t capacity;
} List;

List new_list(int32_t initial_capacity);
List new_list_from_array(struct UStr* array, int32_t size);
void free_list(List* list);
int8_t append(List* list, UStr s);

UStr join(List* list, UStr separator);
int8_t insert(List* list, UStr s, int32_t index);
int8_t listRemoveAt(List* list, int32_t index);
List split(UStr s, UStr separator);

#endif // LIST_H

