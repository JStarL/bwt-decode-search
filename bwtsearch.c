/*
COMP9319 A2
- Jibitesh Saha, z5280740
*/

#include <stdio.h>
#include <stdlib.h>

#include "bwt_ops.h"
#include "freq_table.h"
#include "index_ops.h"

int main(int argc, char *argv[]) {
    
    char *filename = NULL;
    char *index_path = NULL;
    
    /*
    0 = Usual string match, and print lines
    1 = count all pattern matches (inc. duplicates)
    2 = count all line matches (exc. duplicates)
    3 = decode bwt encoded file
    */
    int op_mode = -1;
    char *search_string = NULL;
    char *decoded_filename = NULL;

    if (argc == 5 && argv[1][0] == '-') {
        filename = argv[2];
        index_path = argv[3];
        
        if (argv[1][1] == 'm') {
            op_mode = 1;
            search_string = argv[4];
        } else if (argv[1][1] == 'n') {
            op_mode = 2;
            search_string = argv[4];
        } else if (argv[1][1] == 'o') {
            op_mode = 3;
            decoded_filename = argv[4];
        }
    } else {
        filename = argv[1];
        index_path = argv[2];
        search_string = argv[3];
        op_mode = 0;
    }

    long *frequencies = NULL;
    FILE *f = fopen(index_path, "r");
    if (f == NULL) {
        frequencies = create_index(filename, index_path);
    } else {
        frequencies = get_frequencies(f);
    }
    if (f != NULL) {fclose(f);}

    F_Table cumulative = create_new_cftable(ALPHABET_SIZE);
    load_ftable(cumulative, frequencies, ALPHABET_SIZE);
    free(frequencies);

    FILE *index = fopen(index_path, "r");

    if (op_mode == 3) {
        // decode
        bwt_decode(filename, cumulative, index, decoded_filename);
    } else {
        // search
        bwt_backward_search(search_string, filename, cumulative, index, op_mode);
    }


    free_ftable(cumulative);
    return 0;
}
