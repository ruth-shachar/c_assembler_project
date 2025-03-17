#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "first_pass.h"
#include "second_pass.h"
#include "pre_assembler.h"

int implement_second_pass(char file_name[],label** label_head,instruction_memory **list_head,data_image **data_image_head){
    char str[MAX_LINE_LENGTH] = {0};
    char first_word[MAX_LINE_LENGTH] = {0};
    char second_word[MAX_LINE_LENGTH] = {0};
    char rest_of_line[MAX_LINE_LENGTH] = {0};
    char *ent_file, *ext_file;
    FILE *fp, *ent_p, *ext_p;
    int address_of_ent_label = 0;
    int line = 0;
    long fileSize1;
    long fileSize2;
    fp=fopen(file_name, "r");
    if(fp == NULL){
        printf("Failed to open file: %s\n", file_name);
        return 0;
    }

    

    ent_file = add_new_file(file_name, ".ent");
    ent_p = fopen(ent_file, "w");

    if (ent_p == NULL) {
        printf("Failed to open file: %s\n", ent_file);
        fclose(fp);  
        return 0;
    }

    ext_file = add_new_file(file_name, ".ext");
    ext_p = fopen(ext_file, "w");
    if (ext_p == NULL) {
        printf("Failed to open file: %s\n", ext_file);
        fclose(fp);  
        free(ext_file);  
        return 0;
    }
    fseek(fp, 0, SEEK_SET);
    while (fgets(str,MAX_LINE_LENGTH, fp))
    {
        line++;
        memset(first_word, 0, MAX_LINE_LENGTH);
        memset(second_word, 0, MAX_LINE_LENGTH);
        memset(rest_of_line, 0, MAX_LINE_LENGTH);
        
         sscanf(str, " %s %s %[^\n]", first_word, second_word, rest_of_line);
        if(is_empty_line(str)){     
             continue;
        }
        if(endsWithColon(first_word)){
            if (is_instruction(second_word)){/*becous it's can't be .entry */
                continue;
            }
            else{            
                chek_for_label_argument(ext_p,rest_of_line,line,label_head,list_head,data_image_head);
            }
        }
        else if(opcode_detection(first_word)){
            chek_for_label_argument(ext_p,second_word,line,label_head,list_head,data_image_head);
        }
        else if(strcmp(first_word,".entry") == 0){
            address_of_ent_label = check_valid_entry(second_word,*label_head);/*if the addres is 0 its fail*/
            fprintf(ent_p,"%s           %d\n",second_word,address_of_ent_label);
        }
        
    }    
    fclose(fp);

  fseek(ext_p, 0, SEEK_END);
    fseek(ent_p, 0, SEEK_END);

    fileSize1 = ftell(ent_p);
    fileSize2 = ftell(ext_p);

    fclose(ext_p);
    fclose(ent_p);


    if (fileSize1 == 0){
        remove(ent_file);
        
    }
    if(fileSize2 == 0){
        remove(ext_file);
    }
    free(ent_file);
    free(ext_file);
return 1;
}
