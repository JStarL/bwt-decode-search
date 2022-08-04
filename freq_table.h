/*
COMP9319 A2
- Jibitesh Saha, z5280740
*/

#ifndef FREQ_TABLE_H
#define FREQ_TABLE_H

#include <stdio.h>

typedef struct f_table *F_Table;

F_Table create_new_cftable(int alphabet_size);

F_Table create_new_empty_table();

void free_ftable(F_Table ft);

void load_ftable(F_Table ft, long *frequencies, int alphabet_size);

long get_cumulative_freq(F_Table ft, int ch);

long get_total_freq(F_Table ft);

long get_pointer_freq(F_Table ft);

int get_num_entries(F_Table t);

void sorted_insert(F_Table lns, long line_num);

void print_table(F_Table ft);

#endif
