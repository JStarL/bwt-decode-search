/*
COMP9319 A2
- Jibitesh Saha, z5280740
*/

#ifndef INDEX_OPS_H
#define INDEX_OPS_H

/*
The alphabet is:
10 ('\n'), and 32 - 126 (inclusive)
So: 1 + (126 - 32 + 1) = 96
*/
#define ALPHABET_SIZE 96
#define OCC_GAP 1600

#include <stdio.h>

/*
Given the bwt text as input,
creates a new index with given path

The index contains:
long (8 bytes) arrays of ALPHABET_SIZE, each entry is a frequency
each array is formed from the previous array,
updated with the occurrences in the last OCC_GAP num. of chars in the bwt text

Returns a malloc'ed array of frequencies of each char of the alphabet, to be freed
*/
long *create_index(char *bwt_text, char *index_path);

/*
Appends the array of character occurences for given pos
at the end of the given index file
*/
void write_line(long *cur_freqs, FILE *index);

/*
Reads a value from the index, using:
1) position of current char
2) ASCII value of current char
Returns the occurrence of ch at (pos - (pos % OCC_GAP))
*/
long read_prev_value(FILE *index, long pos, int ch);

/*
Returns an array of frequencies
of size ALPHABET_SIZE
from the last line of the index
*/
long *get_frequencies(FILE *index);

/*
Maps a character to the index in the corresponding occurence array
*/
int char_to_index(int ch);

/*
Maps an index in the occurence array to the corresponding character
*/
int index_to_char(int index);


#endif
