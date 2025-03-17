#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "globals.h"
#include "first_pass.h"
char* INSTRUCTION[] = { ".data",".string",".extern",".entry" };
Register REGISTERS[] = {
	{"r0",1},
	{"r1",2},
	{"r2",3},
	{"r3",4},
	{"r4",5},
	{"r5",6},
	{"r6",7},
	{"r7",8}
};

op_code OPCODES[] = {/*insert -1 when should be null and -2 when should be empty argument*/
		{"mov", 0,  2,{1,2,3,-1},{0,1,2,3}},
		{"cmp", 1,  2,{0,1,2,3},{0,1,2,3}},
		{"add", 2,  2,{1,2,3,-1},{0,1,2,3}},
		{"sub", 3,  2,{1,2,3,-1},{0,1,2,3}},
		{"lea", 4,  2,{1,2,3,-1},{1,-2,-2,-2}},
		{"clr", 5,  1,{1,2,3,-1},{-2,-2,-2,-2}},
		{"not", 6,  1,{1,2,3,-1},{-2,-2,-2,-2}},
		{"inc", 7,  1,{1,2,3,-1},{-2,-2,-2,-2}},
		{"dec", 8,  1,{1,2,3,-1},{-2,-2,-2,-2}},
		{"jmp", 9,  1,{1,2,-1,-1},{-2,-2,-2,-2}},
		{"bne", 10,  1,{1,2,-1,-1},{-2,-2,-2,-2}},
		{"red", 11,  1,{1,2,3,-1},{-2,-2,-2,-2}},
		{"prn", 12,  1,{0,1,2,3},{-2,-2,-2,-2}},
		{"jsr", 13,  1,{1,2,-1,-1},{-2,-2,-2,-2}},
		{"rts", 14,  0,{-2,-2,-2,-2},{-2,-2,-2,-2}},
		{"stop" ,15, 0,{-2,-2,-2,-2},{-2,-2,-2,-2}}
};

type_of_argument arr_type_of_arg[] = {
	{'#',0},
	{'*',2},
	{'r',3}
};

int instr_detection(char* str) {
	int i;

	/* Return 0 if the string is NULL */
	if (str == NULL) {
		return 0;
	}

	/* Iterate through the list of known instructions and compare the string with each instruction in the list */
	for (i = 0; i < INSTRUCTIONS_COUNT; i++) {
		if (strcmp(str, INSTRUCTION[i]) == 0) {
			return 1; /* Return 1 if the string matches an instruction */
		}
	}
	return 0; /* Return 0 if the string is not a valid instruction */
}

int instr_data_detection(char *str,char* first_word, char* rest_of_line, int* DC, int line, data_image** data_image_head) {
    /* Return 0 if the string is NULL */
    if (first_word == NULL) {
        return 0;
    }
    if (strcmp(first_word, ".data") == 0) {
        check_valid_data(rest_of_line, DC, line, data_image_head);
        return 1;
    }
    else if (strcmp(first_word, ".string") == 0) {
        char *start_of_string = strstr(str, ".string");

        if (start_of_string) {

            start_of_string += strlen(".string"); /* Move the pointer to the end of the .string keyword */

            while (*start_of_string == ' ' || *start_of_string == '\t') { /* Skip over any spaces or tabs following .string */
                start_of_string++;
            }
            strcpy(rest_of_line, start_of_string); /* Copy the rest of the line after .string into rest_of_line */

            rest_of_line[strcspn(rest_of_line, "\n")] = '\0'; /* Remove the newline character from rest_of_line */
        }
        check_valid_string(rest_of_line, DC, line, data_image_head);
        return 1;
    }

    return 0;
}
int opcode_detection(char* str) {
	int i;

	/* Return 0 if the string is NULL */
	if (str == NULL) {
		return 0;
	}

	/* Iterate through the list of known opcode and compare the string with each opcode in the list */
	for (i = 0; i < OPCODES_COUNT; i++) {
		if (!strcmp(str, OPCODES[i].name_of_opcode)) {
			return 1; /* Return 1 if the string matches an opcode */
		}
	}
	return 0; /* Return 0 if the string is not a valid opcode */
}

int reg_detection(char* str) {
	int i;
	/* Return 0 if the string is NULL */
	if (str == NULL) {
		return 0;
	}

	/* Iterate through the list of known register and compare the string with each register in the list */
	for (i = 0; i < REG_COUNT; i++) {
		if (!strcmp(str, REGISTERS[i].name_of_register)) {
			return REGISTERS[i].reg_num; /* Return the number of reg (but +1 to avoid return false for r0) */
		}
	}
	return 0; /* Return 0 if the string is not a valid register */
}

bool endsWithColon(const char* str) {/*check if the string in the begining of the line is label*/
	int len = strlen(str);
	if (len == 0) {
		return false; /* String is empty*/
	}
	return str[len - 1] == ':';
}

int extra_char_detection(char* str) {
	char* token;
	/* Allocate memory for the copy of the string*/
	char* rest_of_line = (char*)malloc((strlen(str) + 1) * sizeof(char));
	if (rest_of_line == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}

	/* Copy the original string to avoid modifying it*/
	strcpy(rest_of_line, str);

	/*Use strtok to tokenize the string*/
	token = strtok(rest_of_line, " ");
	if (token == NULL) {
		free(rest_of_line);
		return 0; /* No tokens found */
	}

	/* Check if there's another token after the first*/
	token = strtok(NULL, " ");
	if (token != NULL) {
		free(rest_of_line);
		return 1; /* Extra character found*/
	}

	free(rest_of_line);
	return 0; /*No extra character*/
}
/* if the string start with point its should be instruction */
int is_instruction(char* str) {
	return(*str == '.');
}

int valid_num_argument(char* str, char* rest_of_line, int* num_of_opcode) {
	int i;
	int count = 0;
	char* ptr;
	if (!check_valid_data_comma(rest_of_line)) {
		printf("invalid comma");
		return 0;
	}
	else {
		if (rest_of_line[0] != '\0'){
			ptr = rest_of_line;
			while (*ptr != '\0') {
				if (*ptr == ',') {
					count++;
				}
				ptr++;
			}
		count++;
		}	
		
	}
	for (i = 0; i < OPCODES_COUNT; i++) {
		if (strcmp(str, OPCODES[i].name_of_opcode) == 0 && OPCODES[i].arg_num == count) {
			*num_of_opcode = OPCODES[i].num_of_opcode;
			return 1; /* Return 1 if the string matches an opcode */
		}
	}
	return 0;
}

int parsing_arg(char* name_of_opcode, char* arguments, char* first_word_to_binary, char* second_word_to_binary, char* third_word_to_binary,
                 int* fieldBitSize1, int* fieldBitSize2, int* fieldBitSize3,int *detected_label_on_first_pass) {
    int num_arg_target = 0, num_arg_source = 0;
	
    int type_of_source_arg = -3, type_of_target_arg = -3; /* Invalid by default */
    int num_of_opcode; 
    char target_arg[50] = {0};
    char source_arg[50] = {0};

    if (arguments[0] == '\0') {
        type_of_source_arg = -2;/*-2 if its empty argument*/
        type_of_target_arg = -2;
        *second_word_to_binary = '\0';
        *third_word_to_binary = '\0';
    } 
	else {
        char* comma_ptr = strchr(arguments, ',');
        if (comma_ptr != NULL) { /* Two arguments case */
            strncpy(source_arg, arguments, comma_ptr - arguments);
            strcpy(target_arg, comma_ptr + 1);
        } else { /* Single argument case, just target exist */
            strcpy(target_arg, arguments);
			type_of_source_arg = -2;
        }
	}
    identifyAddressingMode(source_arg, target_arg, &type_of_source_arg, &type_of_target_arg);

        if (type_of_source_arg != -3 && type_of_source_arg != -2) {/*if its valid type*/
            num_arg_source = detection_argument(source_arg + 1); /* Without the first character */
            
        }

        if (type_of_target_arg != -3 && type_of_target_arg != -2) {
            num_arg_target = detection_argument(target_arg + 1); /* Without the first character */
          
        }

		if(num_arg_source == -1 ){
			if(type_of_source_arg == 1){/*this is label*/
				(*detected_label_on_first_pass) ++;
			}
			else{
				return 0;
			}
		}
		if(num_arg_target == -1){
			if(type_of_target_arg == 1){
				(*detected_label_on_first_pass) ++;
			}
			else{
				return 0;
			}			
		}
    

    if (validateParameters(name_of_opcode, &num_of_opcode, type_of_target_arg, type_of_source_arg)) {
        
sprintf(first_word_to_binary, "%d,%d,%d", num_of_opcode, type_of_source_arg, type_of_target_arg);
*fieldBitSize1 = 4;


if (type_of_source_arg == -2) {
   
    if (type_of_target_arg == 0) {
        
        sprintf(second_word_to_binary, "%d", num_arg_target);
        *fieldBitSize2 = 12;
    } else if (type_of_target_arg > 1) {
       
        sprintf(second_word_to_binary, "0,0,0,%d", num_arg_target);
        *fieldBitSize2 = 3;
    } else if (type_of_target_arg == 1) {
        
        strcpy(second_word_to_binary, "");
        *fieldBitSize2 = 0;
    } else if(type_of_target_arg == -2){
		sprintf(second_word_to_binary, "NULL");/*both empty*/
    	*fieldBitSize3 = 0;
	}
   
    sprintf(third_word_to_binary, "NULL");
    *fieldBitSize3 = 0;
} else {
 
     if (type_of_source_arg == 0) {
        sprintf(second_word_to_binary, "%d", num_arg_source);
        *fieldBitSize2 = 12;
    } else if (type_of_source_arg > 1) {
        sprintf(second_word_to_binary, "0,0,%d,0", num_arg_source);
        *fieldBitSize2 = 3;
    } else if (type_of_source_arg == 1) {
        strcpy(second_word_to_binary, "");
        *fieldBitSize2 = 0;
    }

    if (type_of_target_arg == 0) {
        sprintf(third_word_to_binary, "%d", num_arg_target);
        *fieldBitSize3 = 12;
    } else if (type_of_target_arg > 1) {
        sprintf(third_word_to_binary, "0,0,0,%d", num_arg_target);
        *fieldBitSize3 = 3;
    } else if (type_of_target_arg == 1) {
        strcpy(third_word_to_binary, ""); 
        *fieldBitSize3 = 0;
    }
}

if (type_of_source_arg > 1 && type_of_target_arg > 1) {
    sprintf(second_word_to_binary, "0,0,%d,%d", num_arg_source, num_arg_target);
    *fieldBitSize2 = 3;
    
    sprintf(third_word_to_binary, "NULL");
    *fieldBitSize3 = 0;
}

		return 1;
    }
	return 0;
}

void identifyAddressingMode(char* source_arg, char* target_arg, int* type_of_source_arg, int* type_of_target_arg) {
    int i;
    char first_char_of_source = *source_arg;
    char first_char_of_target = *target_arg;

    /* Determine source and target argument types */
    for (i = 0; i < 3; i++) {
        if (arr_type_of_arg[i].first_char == first_char_of_source) {
            *type_of_source_arg = arr_type_of_arg[i].num_of_addressing_method;
        }
        if (arr_type_of_arg[i].first_char == first_char_of_target) {
            *type_of_target_arg = arr_type_of_arg[i].num_of_addressing_method;
        }
    }
    if ((*type_of_source_arg == -3) && is_valid_label(source_arg)) {/*if the argument not null but not found ,check if could be label*/
        *type_of_source_arg = 1;
    }
    if ((*type_of_target_arg == -3) && is_valid_label(target_arg)) {
        *type_of_target_arg = 1;
    }
    
}


int detection_argument(char* argument) {
	if (is_valid_integer(argument)) {
		return atoi(argument);
	}
	else if (reg_detection(argument)) {
		return reg_detection(argument) - 1;
	}
	return -1;
}

int validateParameters(char* name_of_opcode, int* num_of_opcode, int type_of_target_arg, int type_of_source_arg) {
	int i, j, k;
	int valid_source = 0, valid_target = 0;
	for (i = 0; i < OPCODES_COUNT; i++) {
		if (strcmp(name_of_opcode, OPCODES[i].name_of_opcode) == 0) {
			*num_of_opcode = OPCODES[i].num_of_opcode;
			for (j = 0; j < 4; j++) {
				/* Check for valid target type */
				if (type_of_target_arg == OPCODES[i].target_type[j]) {
					valid_target = 1;
					for (k = 0; k < 4; k++) {
						/* Check for valid source type */
						if (type_of_source_arg == OPCODES[i].source_type[k]) {
							valid_source = 1;
						}
					}
				}
			}
		}
	}
	return (valid_source && valid_target);
}
bool check_operand_start(char *operand) {
    int i;
    for (i = 0; i < 3; i++) {
        if (operand[0] == arr_type_of_arg[i].first_char) {
            return true;  
        }
    }
    return false;  
}

int is_rest_of_line_valid(char *str) {  
    
    if (*str == '\0') {
        return 1;  /*valid*/ 
    }

    while (*str) {

        if (!isspace(*str)) {
            return 0;/*not valid*/
        }
        str++;
    }

    return 1;  /*valid*/   
}

bool is_alphanumeric_string(const char* str) {
    while (*str) {
        if (!isalnum((unsigned char)*str)) {
            return false;  /*Found a non-alphanumeric character*/ 
        }
        str++;
    }
    return true;  /*All characters are alphanumeric*/ 
}
