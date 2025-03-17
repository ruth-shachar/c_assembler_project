#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "globals.h"
#include "first_pass.h"
#include "second_pass.h"
#include "pre_assembler.h"


void chek_for_label_argument(FILE *ext_p, char * rest_of_line, int line, label** label_head,instruction_memory **list_head,data_image **data_image_head ) {
    label  *label;
    
    int address;
    int found;
    char *token;
    
    
    char address_label[MAX_LINE_LENGTH]; 
    char address_label_binary[MAX_LINE_LENGTH];
    char rest_of_line_copy[MAX_LINE_LENGTH] = {0};


    strncpy(rest_of_line_copy, rest_of_line, MAX_LINE_LENGTH);
    
    token = strtok(rest_of_line_copy, ",");
    
    while (token != NULL) {
        
        if (!check_operand_start(token)) { 
            label = search_label_on_list(*label_head, token, &found);
            if (label) {
                sprintf(address_label, "%d", label->address_of_label);
                sprintf(address_label_binary, "%d", label->address_of_label);
               
                if (strcmp(".external", label->type_of_label) == 0) {
                    
                    convert_str_to_binary(12, address_label_binary, "001");
                    
                    insert_label_address(*list_head,*data_image_head, address_label_binary,line);
                    address = find_address( *list_head,line);
                        
                        fprintf(ext_p, "%s          %d\n",label->name_of_label, address);
                    
                } else {
                    
                    convert_str_to_binary(12, address_label_binary, "010");
                    
                    insert_label_address(*list_head,*data_image_head, address_label_binary,line);
                }
           
        }
        
    }
    token = strtok(NULL, ","); 
}
}

void insert_label_address(instruction_memory *head,data_image *data_image_head,char *address_label_binary,int line) {
    
    instruction_memory *current1 = head;
    
    while (current1 != NULL) {
        
       if (current1->line == line && current1->binary_str[0]=='\0'){
        
            current1->binary_str = duplicate(address_label_binary);
            return;
       }
        current1 = current1->next;
    }  

}

int check_valid_entry(char* name_of_label, label* label_head) {
	label* lbl = find_label_by_name(name_of_label, label_head);
	if (lbl != NULL) {
        if(strcmp(lbl->type_of_label,".external") == 0){
            printf("A label cannot be defined as external and entry in the same file\n");
        }

		return lbl->address_of_label;/*return the address*/
	}
	else {
		printf("Label %s not found in current file and can't defined as entry\n", name_of_label);
		return 0;
	}
}



int find_address(instruction_memory *head,int line) {
    
    instruction_memory *current1 = head;
    
    
    while (current1 != NULL) {
        
       if (current1->line == line && !strcmp(current1->binary_str,"000000000000001")){/*if there differenc extern?*/
        return current1->address;
            
       }
        current1 = current1->next;
    } 
    return 0; 

}

