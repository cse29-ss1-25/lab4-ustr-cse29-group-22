#include <stdio.h>
#include <stdlib.h>
#include "ustr.h"
#include "list.h"

int main() {
    UStr arr1_ustr_elements[] = {
        new_ustr("apple"),
        new_ustr("banana"),
        new_ustr("cherry")
    };
    List list1 = new_list_from_array(arr1_ustr_elements, 3);
    UStr sep1 = new_ustr(", ");
    UStr joined1 = join(&list1, sep1);

    printf("--- Test Case 1 ---\n");
    printf("List: [\"apple\", \"banana\", \"cherry\"]\n");
    printf("Separator: \", \"\n");
    printf("Joined: "); print_ustr(joined1);
    free_ustr(joined1);
    free_ustr(sep1);
    free_list(&list1);
    for(int i=0; i<3; i++) free_ustr(arr1_ustr_elements[i]);


    UStr arr2_ustr_elements[] = {
        new_ustr("你好"),
        new_ustr("世界"),
        new_ustr("🚀")
    };
    List list2 = new_list_from_array(arr2_ustr_elements, 3);
    UStr sep2 = new_ustr("✨");
    UStr joined2 = join(&list2, sep2);

    printf("\n--- Test Case 2 ---\n");
    printf("List: [\"你好\", \"世界\", \"🚀\"]\n");
    printf("Separator: \"✨\"\n");
    printf("Joined: "); print_ustr(joined2);
    free_ustr(joined2);
    free_ustr(sep2);
    free_list(&list2);
    for(int i=0; i<3; i++) free_ustr(arr2_ustr_elements[i]);


    List list3 = new_list(0);
    UStr sep3 = new_ustr("-");
    UStr joined3 = join(&list3, sep3);
    printf("\n--- Test Case 3 ---\n");
    printf("List: []\n");
    printf("Separator: \"-\"\n");
    printf("Joined: "); print_ustr(joined3);
    free_ustr(joined3);
    free_ustr(sep3);
    free_list(&list3);


    UStr arr4_ustr_elements[] = { new_ustr("single") };
    List list4 = new_list_from_array(arr4_ustr_elements, 1);
    UStr sep4 = new_ustr("---");
    UStr joined4 = join(&list4, sep4);
    printf("\n--- Test Case 4 ---\n");
    printf("List: [\"single\"]\n");
    printf("Separator: \"---\"\n");
    printf("Joined: "); print_ustr(joined4);
    free_ustr(joined4);
    free_ustr(sep4);
    free_list(&list4);
    free_ustr(arr4_ustr_elements[0]);


    return 0;
}

