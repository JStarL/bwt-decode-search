/*
COMP9319 A2
- Jibitesh Saha, z5280740
*/


#include <stdio.h>
#include <stdlib.h>

#include "freq_table.h"

typedef struct f_entry *F_Entry;

struct f_entry {
    int ch;
    long freq;
    F_Entry next;
};

struct f_table {

    // Total size of alphabet,
    // or total number of entries
    int t_size;

    // total count of all characters
    long total_count;

    F_Entry start;

    F_Entry ptr;

};

// *** Headers

F_Entry create_new_fentry(int ch);

void free_fentry(F_Entry fe);

// *** Functions

F_Table create_new_cftable(int alphabet_size) {
    
    F_Table tab = malloc(sizeof(*tab));

    tab->t_size = alphabet_size;
    tab->total_count = 0;
    
    tab->start = create_new_fentry('\n'); // '\n' = 10
    tab->ptr = tab->start;

    F_Entry ptr = tab->start;

    for (int i = 0, j = ' '; i < tab->t_size - 1; i++)
    {
        ptr->next = create_new_fentry(j);
        ptr = ptr->next;
        j++;
    }
    
    return tab;

}

F_Table create_new_empty_table() {

    F_Table tab = malloc(sizeof(*tab));
    
    tab->t_size = 0;
    tab->total_count = 0; // irrelevant, for row num LL
    tab->start = NULL;
    tab->ptr = NULL;

    
}

F_Entry create_new_fentry(int ch) {
    F_Entry new_entry = malloc(sizeof(*new_entry));
    new_entry->ch = ch;
    new_entry->freq = 0;
    new_entry->next = NULL;
    
    return new_entry;

}

void free_ftable(F_Table ft) {
    if (ft == NULL) {return;}

    F_Entry ptr = ft->start;
    while (ptr != NULL)
    {
        F_Entry tmp = ptr;
        ptr = ptr->next;
        free_fentry(tmp);
    }

    free(ft);

}

void free_fentry(F_Entry fe) {
    if (fe == NULL) {return;}
    free(fe);
}

void load_ftable(F_Table ft, long *frequencies, int alphabet_size) {
    long cf = 0;

    F_Entry ptr = ft->start;
    for (int i = 0; i < alphabet_size; i++)
    {
        ptr->freq = cf;
        ptr = ptr->next;
        cf += frequencies[i];
    }
    
    ft->total_count = cf;
}

long get_cumulative_freq(F_Table ft, int ch) {
    if (ft == NULL || ft->start == NULL) {return -1;}

    F_Entry ptr = ft->start;
    while (ptr != NULL) {
        if (ptr->ch == ch) {
            return ptr->freq;
        }
        ptr = ptr->next;
    }

    // Not found
    return -1;

}

long get_total_freq(F_Table ft) {
    if (ft == NULL) {return -1;}

    return ft->total_count;
}

long get_pointer_freq(F_Table ft) {
    if (ft == NULL) {return -1;}

    if (ft->ptr == NULL) {
        return -1;
    } else {
        long value = ft->ptr->freq;
        ft->ptr = ft->ptr->next;
        return value;
    }

}

int get_num_entries(F_Table t) {
    if (t == NULL) {return -1;}

    return t->t_size;
}

void sorted_insert(F_Table lns, long line_num) {
    if (lns == NULL) {return;}

    F_Entry new_entry = create_new_fentry(0);
    new_entry->freq = line_num;

    if (lns->t_size == 0) {
        lns->start = new_entry;
        lns->ptr = new_entry;

        lns->t_size++;
    } else if (line_num < lns->start->freq) {
        new_entry->next = lns->start;
        lns->start = new_entry;
        lns->ptr = new_entry;

        lns->t_size++;
    } else {
        F_Entry ptr = lns->start;
        while (ptr->next != NULL) {
            if (ptr->freq == line_num) {
                free(new_entry);
                return;
            }
            if (line_num < ptr->next->freq) {   
                new_entry->next = ptr->next;
                ptr->next = new_entry;

                lns->t_size++;
                return;
            }

            ptr = ptr->next;
        }

        // insertion at the end
        if (line_num > ptr->freq) {
            ptr->next = new_entry;

            lns->t_size++;
        } else if (line_num == ptr->freq) {
            free(new_entry);
        }
    }
}

void print_table(F_Table ft) {
    if (ft == NULL) {return;}

    printf("ft->num_entries: %d\n", ft->t_size);
    printf("ft->total_count: %ld\n", ft->total_count);
    if (ft->ptr != NULL) {
        printf("ft->ptr->value: %ld\n", ft->ptr->freq);
    }

    putchar('\n');

    if (ft->start == NULL) {return;}

    F_Entry cur = ft->start;
    while (cur != NULL) {
        printf("Char: %d, Value: %ld\n", cur->ch, cur->freq);
        cur = cur->next;
    }

    putchar('\n');

}
