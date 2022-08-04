/*
COMP9319 A2
- Jibitesh Saha, z5280740
*/

#include <stdio.h>
#include <stdlib.h>

#include "index_ops.h"

long *create_index(char *bwt_text, char *index_path) {
    
    FILE *bwt = fopen(bwt_text, "r");

    FILE *index = fopen(index_path, "w");

    // initial frequency counter
    long *freqs = calloc(ALPHABET_SIZE, sizeof(long));

    int ch = -1;
    int gap_index = 0;
    while ((ch = fgetc(bwt)) != EOF) {
        gap_index++;
        
        int i = char_to_index(ch);
        freqs[i]++;
        
        if (gap_index >= OCC_GAP) {
            write_line(freqs, index);
            gap_index = 0;
        }
    }

    if (gap_index > 0) {
        write_line(freqs, index);
    }

    fflush(index);

    return freqs;

}

void write_line(long *cur_freqs, FILE *index) {
    fwrite(cur_freqs, sizeof(long), ALPHABET_SIZE, index);
}

long read_prev_value(FILE *index, long pos, int ch) {
    if (index == NULL) {return -1;}

    long block_number = (pos + 1) / OCC_GAP;
    if (block_number == 0) {return 0;}

    long index_pos = ((long)sizeof(long)) * (((block_number - 1) * ALPHABET_SIZE) + (char_to_index(ch)));

    fseek(index, index_pos, SEEK_SET);

    long prev = 0;
    fread(&prev, sizeof(long), 1, index);

    return prev;
}

long *get_frequencies(FILE *index) {
    if (index == NULL) {return NULL;}

    fseek(index, -1 * ALPHABET_SIZE * ((long) sizeof(long)), SEEK_END);

    long *freqs = calloc(ALPHABET_SIZE, sizeof(long));

    fread(freqs, sizeof(long), ALPHABET_SIZE, index);

    return freqs;
}

int char_to_index(int ch) {
    // 10 , 32 - 126
    // 0 - 95

    if (ch == '\n') { // '\n' = 10
        return 0;
    } else {
        return ch - ' ' + 1; // ' ' = 32, starting from 2nd index onwards
    }
}
