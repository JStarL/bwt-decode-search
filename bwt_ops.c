/*
COMP9319 A2
- Jibitesh Saha, z5280740
*/

#define LAST_CHAR 126
#define MAX_NUM 528
#define MAX_LINE_NUM_WIDTH 7

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include "freq_table.h"
#include "index_ops.h"
#include "bwt_ops.h"



int bwt_step(FILE *f, F_Table cumulative, FILE *index) {
    if (f == NULL) {return 0;}

    long old_pos = ftell(f);

    int ch = fgetc(f);

    long new_pos = get_cumulative_freq(cumulative, ch) + occurence(ch, old_pos, index, f);

    fseek(f, new_pos, SEEK_SET);

    int decoded_ch = fgetc(f);

    fseek(f, -1, SEEK_CUR);

    return decoded_ch;
    
}


int calculate_size_line_marker(FILE *f, F_Table cumulative, FILE *index) {
    if (f == NULL) {return 0;}

    // 0 = get_cumulative_freq(cumulative, '\n')
    fseek(f, 0, SEEK_SET);

    int width = 0;
    int ch = bwt_step(f, cumulative, index);
    while (ch != '\n') {
        if (ch >= '0' && ch <= '9') {
            width++;
        } else {
            width = 0;
        }

        ch = bwt_step(f, cumulative, index);
    }

    return width;
}

void move_before_line_marker(FILE *f, F_Table cumulative, FILE *index, long line_num, int width) {
    if (f == NULL) {return;}

    long curr = line_num;
    int digit = ((int)(curr % 10)) + '0';
    curr = curr / 10;

    long first = get_cumulative_freq(cumulative, digit);
    long second = get_cumulative_freq(cumulative, digit + 1) - 1;

    for (int i = 0; i < width - 1; i++)
    {
        digit = ((int)(curr % 10)) + '0';
        curr = curr / 10;

        first = get_cumulative_freq(cumulative, digit) + occurence(digit, first, index, f);
        second = get_cumulative_freq(cumulative, digit) + occurence(digit, second + 1, index, f) - 1;

        if (second < first) {
        // line_num not found
        return;
        }

    }
    
    int c = -1;
    fseek(f, first, SEEK_SET);
    while (ftell(f) <= second) {
        c = fgetc(f);
        if (c == '\n') {
            fseek(f, -1, SEEK_CUR);
            // printf("[new-line found]"); // DEBUG
            return;
        }
    }
    // printf("['\\n' not found]"); // DEBUG

}

long calculate_last_line_marker(FILE *f, F_Table cumulative, FILE *index, int width) {
    if (f == NULL) {return -1;}

    move_before_line_marker(f, cumulative, index, 1, width);

    int c = -1;
    long last_line = 0;
    long multiplier = 1;
    while (c != '\n') {
        if (c >= '0' && c <= '9') {
            last_line = ((c - '0') * multiplier) + last_line;
            multiplier *= 10;
        } else {
            last_line = 0;
            multiplier = 1;
        }

        c = bwt_step(f, cumulative, index);
    }

    return last_line;
}

long occurence(int ch, long pos, FILE *index, FILE *bwt_text) {
    if (bwt_text == NULL || index == NULL) {return -1;}

    long prev = read_prev_value(index, pos, ch);
    long rank = prev;
    if ((pos + 1) % OCC_GAP == 0) {
        fseek(bwt_text, pos, SEEK_SET);
        int c = fgetc(bwt_text);
        if (c == ch) {
            rank--;
        }
    } else {
        long start_pos = ((pos + 1) / OCC_GAP) * OCC_GAP;

        fseek(bwt_text, start_pos, SEEK_SET);

        int c = -1;
        while (ftell(bwt_text) < pos) {
            c = fgetc(bwt_text);
            if (c == ch) {
                rank++;
            }
        }

        
    }

    return rank;
}

void bwt_backward_search(char *search_string, char *filename, F_Table cumulative, FILE *index, int op_mode) {

    int i = 0;
    while (search_string[i] != '\0') {
        i++;
    }

    i--;

    char ch = search_string[i];
    long first = get_cumulative_freq(cumulative, ch);
    long second;
    if (ch < LAST_CHAR) {
        second = get_cumulative_freq(cumulative, ch + 1) - 1;
    } else {
        second = get_total_freq(cumulative) - 1;
    }

    FILE *f = fopen(filename, "r");

    while ((i > 0) && (first <= second)) {
        ch = search_string[i-1];
        first = get_cumulative_freq(cumulative, ch) + occurence(ch, first, index, f);
        second = get_cumulative_freq(cumulative, ch) + occurence(ch, second + 1, index, f) - 1;
        i--;
    }

    fclose(f);

    if (second < first) {
        // no matches
        if (op_mode == 1 || op_mode == 2) {
            printf("0\n");
        }
        return;
    }

    if (op_mode == 1) {
        // -m
        printf("%ld\n", second - first + 1);
        return;
    }

    F_Table line_nums = create_new_empty_table();
    for (long i = first; i <= second; i++)
    {
        long line_num = get_line_num(i, filename, cumulative, index);
        sorted_insert(line_nums, line_num);
    }
    
    if (op_mode == 2) {
        // -n
        printf("%d\n", get_num_entries(line_nums));
        return;
    }

    // Print all matching lines
    // print_table(line_nums); // DEBUG
    decode_print_lines(line_nums, filename, cumulative, index);

}

long get_line_num(long start_pos, char *filename, F_Table cumulative, FILE *index) {
    if (filename == NULL) {return -1;}

    FILE *f = fopen(filename, "r");

    fseek(f, start_pos, SEEK_SET);

    int ch = -1;
    long line_num = 0;
    long multiplier = 1;
    while (ch != '\n') {
        if (ch >= '0' && ch <= '9') {
            line_num = ((ch - '0') * multiplier) + line_num;
            multiplier *= 10;
        } else {
            line_num = 0;
            multiplier = 1;
        }

        ch = bwt_step(f, cumulative, index);
    }

    fclose(f);

    return line_num;
}

void print_buffer(int *buffer, int index) {
    for (int i = index; i < MAX_NUM; i++)
    {
        putchar(buffer[i]);
    }
    
}

void decode_print_lines(F_Table line_nums, char *filename, F_Table cumulative, FILE *index) {
    if (filename == NULL) {return;}

    FILE *f = fopen(filename, "r");
    int width = calculate_size_line_marker(f, cumulative, index);
    long last_line = calculate_last_line_marker(f, cumulative, index, width);


    int buffer[MAX_NUM];
    long line_num = get_pointer_freq(line_nums);
    while (line_num != -1)
    {

        // printf("Line Num: %ld\n", line_num); // DEBUG

        // for each line num
        long search_num;
        if (line_num != last_line) {
            search_num = line_num + 1;
        } else {
            search_num = 1;
        }

        // printf("Search Num: %ld\n", search_num); // DEBUG

        move_before_line_marker(f, cumulative, index, search_num, width);

        buffer[MAX_NUM - 1] = '\n';

        int i = MAX_NUM - 1;
        int c = -1;
        while (c != '\n' && i > 0) {
            c = bwt_step(f, cumulative, index);
            i--;
            buffer[i] = c;
        }
        i++; // pre-'\n' written

        print_buffer(buffer, i);

        line_num = get_pointer_freq(line_nums);
    }
    
    fclose(f);
}

void bwt_decode(char *filename, F_Table cumulative, FILE *index, char *decoded_filename) {
    if (filename == NULL) {return;}

    FILE *bwt_text = fopen(filename, "r");

    if (bwt_text == NULL) {return;}

    FILE *decode = fopen(decoded_filename, "w");

    struct stat s;
    stat(filename, &s);

    long bwt_text_size = (long) s.st_size;

    fseek(decode, bwt_text_size - 1, SEEK_SET);

    fputc('\n', decode);
    fseek(decode, -2, SEEK_CUR);

    int width = calculate_size_line_marker(bwt_text, cumulative, index);
    move_before_line_marker(bwt_text, cumulative, index, 1, width);

    long starting_pos = ftell(bwt_text);

    int ch = bwt_step(bwt_text, cumulative, index);

    while (ftell(bwt_text) != starting_pos) {
        fputc(ch, decode);
        fseek(decode, -2, SEEK_CUR);

        ch = bwt_step(bwt_text, cumulative, index);
    }

    fclose(bwt_text);
    fclose(decode);

}


