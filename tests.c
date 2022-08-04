#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bwt_ops.h"
#include "freq_table.h"
#include "index_ops.h"

#define MAX_SIZE 528

void decode_print_series(FILE *bwt_text, F_Table ft, FILE *index);

int main(int argc, char *argv[]) {
    
    long *freqs = create_index(argv[1], argv[2]);

    FILE *ind = fopen(argv[2], "r");
    FILE *bwt_text = fopen(argv[1], "r");

    long *new_freqs = get_frequencies(ind);

    F_Table ft = create_new_cftable(ALPHABET_SIZE);
    
    load_ftable(ft, new_freqs, ALPHABET_SIZE);
    // print_table(ft);

    F_Table list = create_new_empty_table();

    sorted_insert(list, 1);
    sorted_insert(list, 12);
    sorted_insert(list, 123);
    sorted_insert(list, 303);
    sorted_insert(list, 4004);

    decode_print_lines(list, argv[1], ft, ind);

    char *search = "compiler";

    bwt_backward_search(search, argv[1], ft, ind, 0);

    exit(0);

    assert (occurence(' ', 1700, ind, bwt_text) == 403);
    assert (occurence('"', 1700, ind, bwt_text) == 223);
    assert (occurence('.', 1700, ind, bwt_text) == 135);
    assert (occurence('e', 1700, ind, bwt_text) == 188);

    // char 1700 [0-based] = 'r'
    assert (occurence('r', 1700, ind, bwt_text) == 60);
    
    assert (occurence('a', 1599, ind, bwt_text) == 39);
    
    // char 1599 [0-based] = 'h'
    assert (occurence('h', 1599, ind, bwt_text) == 20);
    
    assert (occurence('h', 1600, ind, bwt_text) == 21);
    
    assert (occurence('h', 1601, ind, bwt_text) == 21);

    
    fseek(bwt_text, 1, SEEK_SET);
    printf("Line Num: %ld\n", get_line_num(ftell(bwt_text), argv[1], ft, ind));
    decode_print_series(bwt_text, ft, ind);

    // int buf[MAX_SIZE] = {0};


    // int i = MAX_SIZE - 1;
    // buf[i] = '\n';
    // i--;
    // while (i >= 0) {
    //     buf[i] = bwt_step(bwt_text, ft, ind);
    //     i--;
    // }

    // for (int i = 0; i < MAX_SIZE; i++)
    // {
    //     putchar(buf[i]);
    // }
    
    int width = calculate_size_line_marker (bwt_text, ft, ind);
    printf("Line Marker Size: %d\n", width);
    printf("Last Line Number: %ld\n", calculate_last_line_marker (bwt_text, ft, ind, width));

    move_before_line_marker(bwt_text,  ft, ind, 144, width);
    printf("Line Num: %ld\n", get_line_num(ftell(bwt_text), argv[1], ft, ind));


    decode_print_series(bwt_text, ft, ind);



    // sorted_insert(ft, 2);
    // sorted_insert(ft, 3);
    // sorted_insert(ft, 1);
    // sorted_insert(ft, 2);
    // sorted_insert(ft, 4);
    // sorted_insert(ft, 6);
    // sorted_insert(ft, 5);
    // sorted_insert(ft, 1);
    // sorted_insert(ft, 3);
    // print_table(ft);



    // printf("Values...\n");
    // printf("Cum Freq of %d: %ld\n", 32, get_cumulative_freq(ft, 32));
    // printf("Cum Freq of %d: %ld\n", 50, get_cumulative_freq(ft, 50));
    // printf("Cum Freq of %d: %ld\n", 105, get_cumulative_freq(ft, 105));
    // printf("Cum Freq of %d: %ld\n", 115, get_cumulative_freq(ft, 115));

    // printf("Total Freq of ft: %ld\n", get_total_freq(ft));
    // printf("Pointer Freq of ft: %ld\n", get_pointer_freq(ft));
    // printf("Pointer Freq of ft: %ld\n", get_pointer_freq(ft));
    // printf("Num Entries of ft: %ld\n", get_num_entries(ft));
    


    // fseek(ind, 0, SEEK_SET);

    // long tmp;
    // fread(&tmp, sizeof(long), 1, ind);

    // printf("1st long: %ld\n", tmp);

    // long prev = read_prev_value(ind, 1598, '.');

    // printf("Prev value: %ld\n", prev);

    // for (int i = 0; i < ALPHABET_SIZE; i++)
    // {
    //     assert(freqs[i] == new_freqs[i]);

    //     // printf("Og freqs Val: %ld\n", freqs[i]);
    //     // printf("new freq Val: %ld\n", new_freqs[i]);
    // }
    

    free(freqs);
    free(new_freqs);
    
    free_ftable(ft);

    fclose(ind);

    return 0;
}

void decode_print_series(FILE *bwt_text, F_Table ft, FILE *index) {
    int buf[MAX_SIZE] = {0};

    //fseek(bwt_text, 1, SEEK_SET);

    int i = MAX_SIZE - 1;
    buf[i] = '\n';
    i--;
    while (i >= 0) {
        buf[i] = bwt_step(bwt_text, ft, index);
        i--;
    }

    for (int i = 0; i < MAX_SIZE; i++)
    {
        putchar(buf[i]);
    }
}
