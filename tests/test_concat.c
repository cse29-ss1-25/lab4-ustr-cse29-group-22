#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ustr.h"


int main(){
	char input[256];
	while(fgets(input, sizeof(input), stdin)){
		char str1[256];
		char str2[256];
		sscanf(input, "%255s %255s", str1, str2);
		UStr in_ustr1 = new_ustr(str1);
		UStr in_ustr2 = new_ustr(str2);
		UStr output = concat(in_ustr1, in_ustr2);
		print_ustr(output);
		printf("\n");
		free_ustr(output);
		free_ustr(in_ustr1);
		free_ustr(in_ustr2);

	}
	return 0;

}
