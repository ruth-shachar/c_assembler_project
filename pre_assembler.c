#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "pre_assembler.h"

int implement_macro(char file_name[],node **head) { 
    
    char *new_file1, *new_file2, *final_file, *temp_file_name1, *temp_file_name2;
    new_file1 = remove_extra_spaces_file(file_name);
    if (new_file1 == NULL) {
        return 0;
    }

    

    if (!add_macro(new_file1, head)) {
        free_list(*head);
        remove(new_file1);
        free(new_file1);
        return 0;
    }

    new_file2 = remove_mcros_decl(new_file1);
    if (new_file2 == NULL) {
        free_list(*head);
        remove(new_file1);
        free(new_file1);
        return 0;
    }

    free(new_file1);

    final_file = replace_all_mcros(new_file2, *head);
    if (final_file == NULL) {
        free_list(*head);
        remove(new_file2);
        free(new_file2);
        return 0;
    }
   
   
    temp_file_name1 = add_new_file(file_name, ".t01");
    temp_file_name2 = add_new_file(file_name, ".t02");
    remove(temp_file_name1);
    remove(temp_file_name2);

    free(temp_file_name1);
    free(temp_file_name2);
 
    free(new_file2);
    free(final_file);
    free_list(*head);

    return 1;
}