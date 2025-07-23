#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ustr.h"

int main() {
    UStr s1 = new_ustr("hello world");
    UStr sub1 = substring(s1, 0, 5);
    printf("Original: "); print_ustr(s1);
    printf("Substring (0,5): "); print_ustr(sub1);
    free_ustr(sub1);

    UStr s2 = new_ustr("你好世界");
    UStr sub2 = substring(s2, 0, 2);
    printf("\nOriginal: "); print_ustr(s2);
    printf("Substring (0,2): "); print_ustr(sub2);
    free_ustr(sub2);

    UStr s3 = new_ustr("🚀🛰️🌌");
    UStr sub3 = substring(s3, 1, 3); // Should be "🛰️🌌" (2 codepoints, 8 bytes)
    printf("\nOriginal: "); print_ustr(s3);
    printf("Substring (1,3): "); print_ustr(sub3);
    free_ustr(sub3);

    UStr s4 = new_ustr("test");
    UStr sub4 = substring(s4, 0, 0);
    printf("\nOriginal: "); print_ustr(s4);
    printf("Substring (0,0): "); print_ustr(sub4);
    free_ustr(sub4);

    UStr s5 = new_ustr("longstring");
    UStr sub5 = substring(s5, 5, 100);
    printf("\nOriginal: "); print_ustr(s5);
    printf("Substring (5,100): "); print_ustr(sub5);
    free_ustr(sub5);

    UStr s6 = new_ustr("another test");
    UStr sub6 = substring(s6, 5, 3);
    printf("\nOriginal: "); print_ustr(s6);
    printf("Substring (5,3): "); print_ustr(sub6);
    free_ustr(sub6);

    free_ustr(s1);
    free_ustr(s2);
    free_ustr(s3);
    free_ustr(s4);
    free_ustr(s5);
    free_ustr(s6);

    return 0;
}

