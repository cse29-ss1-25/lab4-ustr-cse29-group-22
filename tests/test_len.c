#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ustr.h"

int main() {
    char input[1024];

    while (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;

        UStr s = new_ustr(input);
        printf("Length: %d\n", len(s));

        free_ustr(s);
    }

    return 0;
}

