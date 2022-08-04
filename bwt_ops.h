/*
COMP9319 A2
- Jibitesh Saha, z5280740
*/

#ifndef BWT_OPS_H
#define BWT_OPS_H

#define MAX_MATCHES 528

#include <stdio.h>
#include "freq_table.h"


/*
Performs one bwt decoding step backwards
returns the next decoded char,
i.e. the one just before in the original file
The file pointer points to the current character
After the decode step, the file pointer points to the next decoded char in the bwt file
*/
int bwt_step(FILE *f, F_Table cumulative, FILE *index);

/*
Returns the total number of digits
in the line marker of the og file:
Eg: 0024 The point is ...
Here, the line marker size is: 4
*/
int calculate_size_line_marker(FILE *f, F_Table cumulative, FILE *index);

long calculate_last_line_marker(FILE *f, F_Table cumulative, FILE *index, int width);

/*
Moves the given file pointer
to the '\n' before given line number:
Eg ...   -> \n <-  0012 ...
*/
void move_before_line_marker(FILE *f, F_Table cumulative, FILE *index, long line_num, int width);

/*
Returns the occurence of ch at position pos
Uses the index to retrieve previous value
and then searches the remaining in the bwt text

occurence = num chars of ch in any position < pos

*/
long occurence(int ch, long pos, FILE *index, FILE *bwt_text);

/*
Performs the general bwt backward search, in 3 modes:
0: NULL -> usual, print all line matches (no duplicates)
1: -m -> print count of all line matches (duplicates)
2: -n -> print count of all line matches (no duplicates)
*/
void bwt_backward_search(char *search_string, char *filename, F_Table cumulative, FILE *index, int op_mode);

/*
Given a starting position in the bwt encoded file
returns the line number of which it is a part:
Eg 0034 There once was a hor -> s <- e ...
If start_pos is the position of this 's' in the bwt file,
this returns 34
*/
long get_line_num(long start_pos, char *filename, F_Table cumulative, FILE *index);

void decode_print_lines(F_Table line_nums, char *filename, F_Table cumulative, FILE *index);

/*
Decodes the given bwt text file
*/
void bwt_decode(char *filename, F_Table cumulative, FILE *index, char *decoded_filename);



#endif
