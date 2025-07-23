#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ustr.h"
#include "pset1.h"

/*
Initializes a new UStr with contents
*/
UStr new_ustr(char* contents) {
	int32_t bytes = strlen(contents);
	int32_t codepoints = utf8_strlen(contents);
	uint8_t all_ascii = bytes == codepoints;
	char* contents_copy = malloc(bytes + 1);
	strcpy(contents_copy, contents);
	UStr s = {
		codepoints,
		bytes,
		all_ascii,
		contents_copy
	};
	return s;
}

/*
Returns length of string
*/
int32_t len(UStr s) {
	return s.codepoints;
}

/*
Returns a substring of str, starting at index start (inclusive) 
and ending at index end (exclusive).

Returns an empty string on invalid range.
*/
UStr substring(UStr s, int32_t start, int32_t end) {
	// TODO: implement this

	if (start < 0 || end > len(s) || start >= end) { return new_ustr(""); }

	int index = 0;
        const char* p = s.contents;	
	const char* start_byte = NULL;
	const char* end_byte = NULL; 

	while (index < end && *p != '\0') {
		if (index == start) {
			start_byte = p; 
		}

		char c = *p; 
		int codepoint_len = 1; 

		if ((c & 0b11000000) == 0) { codepoint_len = 1; }
		else if ((c & 0b11100000) == 0b11000000) { codepoint_len = 2; }
		else if ((c & 0b11110000) == 0b11100000) { codepoint_len = 3; }
		else if ((c & 0b11111000) == 0b11110000) { codepoint_len = 4; }

		p += codepoint_len;
	        index++;

		if (index == end) {
			end_byte = p; 
		}	
	}

	int sub_len = end_byte - start_byte;
	char* sub_str = malloc(sub_len + 1);

	for (int i = 0; i < sub_len; i++) {
		sub_str[i] = start_byte[i]; 
	}	
       
	sub_str[sub_len] = '\0';

	UStr result = new_ustr(sub_str); 

	free(sub_str); 

	return result; 


}

/*
Given 2 strings s1 and s2, returns a string that is the result of 
concatenating s1 and s2. 
*/
UStr concat(UStr s1, UStr s2) {
	// TODO: implement this

}

/*
Given a string s and an index, return a string with the character at index 
removed from the original string. 

Returns the original string if index is out of bounds.
*/
UStr removeAt(UStr s, int32_t index) {
	// TODO: implement this

}

/*
Given a string s, return s reversed. 

Example: reverse("apples🍎 and bananas🍌") = "🍌sananab dna 🍎selppa")
*/
UStr reverse(UStr s) {
	// TODO: implement this

}


void print_ustr(UStr s) {
	printf("%s [codepoints: %d | bytes: %d]", s.contents, s.codepoints, s.bytes);
}

void free_ustr(UStr s) {
	if (s.contents != NULL) {
		free(s.contents);
		s.contents = NULL;
	}
}

