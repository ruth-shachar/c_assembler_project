#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "globals.h"
#include "first_pass.h"
#include "pre_assembler.h"

int label_process(char* first_word, int* p_address, label** label_head,char *type_of_label) {
	size_t len = strlen(first_word);
	int found;
	if (len > 0 && first_word[len - 1] == ':') {
		first_word[len - 1] = '\0';  /* Remove the column */
	}
	if (len > MAX_LABEL_LENGTH) {
		printf("The label is too long\n");
		return 0;
	}

	search_label_on_list(*label_head, first_word, &found);

	if (!found) {
		if (is_valid_label(first_word)) {
			add_label_to_list(first_word, p_address, label_head,type_of_label);
			return 1;
		}
	}

	return 0;
}

char* duplicate(const char* str)
{
	char* new_str = (char*)handle_malloc((strlen(str) + 1) * sizeof(char));
	strcpy(new_str, str);
	return new_str;
}

/* Function to add a new label to the list */
void add_label_to_list(char* name_of_label, int* address_of_label, label** head,char *type_of_label) {
	label* new_label = (label*)handle_malloc(sizeof(label));
	if (new_label == NULL) {
		perror("Failed to allocate memory for new label");
		exit(EXIT_FAILURE);
	}

	new_label->name_of_label = duplicate(name_of_label);  /* Copy the string */
	if (address_of_label != NULL)
	{
		new_label->address_of_label = *address_of_label;
	}
	else
	{
		new_label->address_of_label = 0;
	}
	new_label->type_of_label = NULL;  /* Initialize or set appropriately */
	new_label->next = NULL;
	update_label_type(new_label,type_of_label);

	if (*head == NULL) {
		*head = new_label;
	}
	else {
		label* current = *head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_label;
	}

}


/* Function to search for a label in the list */
label* search_label_on_list(label* head, char* name_of_label, int* found) {
	label* current = head;
	*found = 0;

	while (current != NULL) {
		if (strcmp(name_of_label, current->name_of_label) == 0) {
			*found = 1;
			return current;
		}
		current = current->next;
	}

	return NULL;
}

/* Function to check if a label is valid */
int is_valid_label(char* name_of_label) {
	return (!instr_detection(name_of_label) &&
		!opcode_detection(name_of_label) &&
		!reg_detection(name_of_label) &&
		!extra_char_detection(name_of_label) &&
		isalpha(*name_of_label))&&
		is_alphanumeric_string(name_of_label);
}


int instruction_data_process(char *str,char* first_word, char* rest_of_line, int* DC, int line, data_image** data_image_head) {
	if (!instr_data_detection(str,first_word, rest_of_line, DC, line, data_image_head)) {
		printf("undefinde instruchion in line: %d\n",line);
		return 0;
	}
	return 1;
}

void check_valid_data(char* rest_of_line, int* DC, int line, data_image** data_image_head) {
	char* token;
	
	if (check_valid_data_comma(rest_of_line)) {

		char rest_of_line_copy[MAX_LINE_LENGTH];
		strncpy(rest_of_line_copy, rest_of_line, MAX_LINE_LENGTH);
		token = strtok(rest_of_line_copy, ",");
		while (token != NULL) {
			if (is_valid_integer(token)) {
				
				add_to_data_image(atoi(token),*DC, data_image_head,line);
				(*DC)++;
			}
			else {
				
				printf("One or more numbers are invalid");
				break;
			}
			token = strtok(NULL, ",");
		}
		
	}
	else {
		printf("Invalid data format in line: %d\n", line);
	}
}

void check_valid_string(char* rest_of_line, int* DC, int line, data_image** data_image_head) {
	int i;
	if (starts_and_ends_with_quote(rest_of_line)) {
		
		for (i = 1; i < (int)strlen(rest_of_line) - 1; i++) { 
			add_to_data_image((int)rest_of_line[i],*DC, data_image_head,line);
			(*DC)++;
		}
		
		add_to_data_image((int)'\0',*DC, data_image_head,line);
		(*DC)++;
	}
	else {
		printf("Invalid string in line: %d\n", line);
	}
}


void update_data_label(label *label_head,int IC){
	label* current = label_head;
	while (current != NULL) {
		if (strcmp(current->type_of_label,".data") == 0) {
			current->address_of_label += IC;
		}
		current = current->next;
	}
	
}





void add_to_data_image(int value, int address, data_image** data_image_head, int line) {
    

    int i;
    char binary_value[16]; 
    data_image* new_node;
    binary_value[15] = '\0';
    

  
    for (i = 14; i >= 0; i--) {
        binary_value[i] = (value & 1) ? '1' : '0';
        value >>= 1;
    }

    new_node = (data_image*)handle_malloc(sizeof(data_image));
    
    new_node->binary_value = duplicate(binary_value);
    new_node->address = address;
    new_node->line = line;
    new_node->next = NULL;
    

    if (*data_image_head == NULL) {
        *data_image_head = new_node;
    } else {
        data_image* current = *data_image_head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
	
}




void add_to_instruction_memory(int line,int address,char* binary_str , instruction_memory** instruction_memory_head) {
	instruction_memory* new_instruction = (instruction_memory*)handle_malloc(sizeof(instruction_memory));
	new_instruction->address = address;
	new_instruction->line = line;
	new_instruction->binary_str = duplicate(binary_str);
	new_instruction->next = NULL;

	if (*instruction_memory_head == NULL) {
		*instruction_memory_head = new_instruction;
	}
	else {
		instruction_memory* current = *instruction_memory_head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_instruction;
	}
}
label* find_label_by_name(const char* name_of_label, label* label_head) {
	label* current = label_head;
	while (current != NULL) {
		if (strcmp(current->name_of_label, name_of_label) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}
void update_label_type(label* lbl, const char* new_type) {
	if (lbl->type_of_label != NULL) {
		free(lbl->type_of_label);
	}
	lbl->type_of_label = duplicate(new_type);
}
void create_entry_file(const char* filename, const char* rest_of_line, label* label_head) {
	FILE* file;
	label* lbl = find_label_by_name(rest_of_line, label_head);
	if (lbl == NULL) {
		printf("Label %s not found\n", rest_of_line);
		return;
	}

	file = fopen(filename, "w");
	if (file == NULL) {
		perror("Error opening file");
		return;
	}

	fprintf(file, "%s %d\n", rest_of_line, lbl->address_of_label);

	fclose(file);
}

void convert_str_to_binary(int length, char *str, char *ARE) {
    char *token;
    char binary[WORD_LEN];
    char result[FILED_SIZE] = "";
	if(str[0]=='\0' ||!strcmp(str,"NULL")){/*if its label or empty argument*/
		return;
	}
	else{
		token = strtok(str, ",");
		while (token != NULL) {
			int num = atoi(token);
			decimal_to_binary(length, num, binary);
			strcat(result, binary);
			token = strtok(NULL, ",");
		}

		strcat(result, ARE);
		strcpy(str, result);
	}
		
}


void convert_first_word_to_binary(int length, char* str, char* ARE) {

	char* token;
	char binary[WORD_LEN];
	int first_number = 1;
	char result[FILED_SIZE] = "";
	
	token = strtok(str, ",");
	while (token != NULL) {
		int num = atoi(token);
		if (first_number && (num >= 0)) {
			decimal_to_binary(length, num, binary);
			first_number = 0;
		} else {
			convert_to_binary(length, num, binary);
		}
		strcat(result, binary);
		token = strtok(NULL, ",");
	}
	
	strcat(result, ARE);
	strcpy(str, result);
}

void decimal_to_binary(int length, int decimal, char binary[])
{
	int i;
	int is_negative = 0;
	char temp;
	/* Handle negative numbers */
	if (decimal < 0)
	{
		is_negative = 1;
		decimal = -decimal;
	}
	/* Handle special case for 0 */
	if (decimal == 0)
	{
		memset(binary, '0', length);
		binary[length] = '\0';
		return;
	}
	/* Convert the decimal number to binary */
	i = 0;
	while (decimal > 0)
	{
		binary[i++] = (decimal % 2) ? '1' : '0';
		decimal /= 2;
	}
	/* Pad the binary representation with '0's to ensure it has the correct length */
	while (i < length)
		binary[i++] = '0';

	/* Null-terminate the string */
	binary[i] = '\0';
	/* Reverse the binary array to get the correct representation */
	for (i = 0; i < length / 2; i++)
	{
		temp = binary[i];
		binary[i] = binary[length - i - 1];
		binary[length - i - 1] = temp;
	}
	/* If number was negative, compute two's complement */
	if (is_negative)
	{
		for (i = 0; i < length; i++)
		{
			binary[i] = (binary[i] == '0') ? '1' : '0'; /*Invert bits */
		}
		/* Add 1 to the binary number */
		for (i = length - 1; i >= 0; i--)
		{
			if (binary[i] == '0')
			{
				binary[i] = '1';
				break;
			}
			else
			{
				binary[i] = '0';
			}
		}
	}
}

void convert_to_binary(int length, int decimal, char binary[]) {

	memset(binary, '0', length);

	if (decimal >= 0 && decimal < length) {
		binary[length - decimal - 1] = '1';
	}

	binary[length] = '\0';
}



int opcode_process(char* first_word, char* rest_of_line, int* IC,int line,instruction_memory **instruction_memory_head) {
	
	int detected_label_on_first_pass = 0;
	int num_of_opcode = 0;
	int fieldBitSize1 = 0, fieldBitSize2 = 0, fieldBitSize3 = 0;
	char* first_word_to_binary = (char*)calloc(16, sizeof(char)); /* Allocate memory for the binary string */
	char* second_word_to_binary = (char*)calloc(16, sizeof(char)); /* Allocate memory for the binary string */
	char* third_word_to_binary = (char*)calloc(16, sizeof(char)); /* Allocate memory for the binary string */

	if (first_word_to_binary == NULL || second_word_to_binary == NULL || third_word_to_binary == NULL) {
        fprintf(stderr, "Error: malloc failed\n");
        free(first_word_to_binary);
        free(second_word_to_binary);
        free(third_word_to_binary);
        return 0;
    }
	
	if (valid_num_argument(first_word, rest_of_line, &num_of_opcode)) {
		if(!parsing_arg(first_word, rest_of_line, first_word_to_binary, second_word_to_binary, third_word_to_binary,
		&fieldBitSize1, &fieldBitSize2, &fieldBitSize3, &detected_label_on_first_pass))
		{
			printf("invalid argument in line: %d\n",line);
			return 0;
		}
		
		if (first_word_to_binary != NULL) {
			convert_first_word_to_binary(fieldBitSize1, first_word_to_binary, "100");
			convert_str_to_binary(fieldBitSize2, second_word_to_binary, "100");
			convert_str_to_binary(fieldBitSize3, third_word_to_binary, "100");
		
		}
		
			add_to_instruction_memory(line,*IC,first_word_to_binary,instruction_memory_head);
			(*IC)++;

			
			if (strcmp(second_word_to_binary,"NULL") != 0) {/*if its not empty*/
				add_to_instruction_memory(line,*IC,second_word_to_binary,instruction_memory_head);
				(*IC)++;
			}
			
			
			if (strcmp(third_word_to_binary,"NULL") != 0) {
				add_to_instruction_memory(line,*IC,third_word_to_binary,instruction_memory_head);
				(*IC)++;
			}
		
			free(first_word_to_binary);
			free(second_word_to_binary);
			free(third_word_to_binary);
			return 1;
		}
	
	free(first_word_to_binary);
	free(second_word_to_binary);
	free(third_word_to_binary);
	return 0;
}


void free_label_list(label* head) {
	label* current = head;
	label* next;

	while (current != NULL) {
		next = current->next;
		free(current->name_of_label);
		free(current->type_of_label);
		free(current);
		current = next;
	}
}

void free_instruction_memory(instruction_memory* head) {
	instruction_memory* current = head;
	instruction_memory* next;

	while (current != NULL) {
		next = current->next;
		free(current->binary_str);
		free(current);
		current = next;
	}
}

void free_data_image(data_image* data_image_head) {
	data_image* current = data_image_head;
	data_image* next_node;

	while (current != NULL) {
		next_node = current->next;
		free(current);
		current = next_node;
	}

	data_image_head = NULL;
}


unsigned int binaryToOctal(const char *binary) {
    unsigned int octal = 0;
    unsigned int power = 1;
    int len = strlen(binary);
    int i;

    for (i = len - 1; i >= 0; i--) {
        if (binary[i] == '1') {
            octal += power;
        }
        power *= 2;
    }

    return octal;
}

void copyAndConvertFile(char *sourceFile, char *destinationFile) {
    FILE *src = fopen(sourceFile, "r");
    FILE *dest = fopen(destinationFile, "w");
    char buffer[256];

    if (!src || !dest) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    


    if (fgets(buffer, sizeof(buffer), src)) {
        fputs(buffer, dest);
    }


    while (fgets(buffer, sizeof(buffer), src)) {
        unsigned int address;
        char binary[128] = "";


        int result = sscanf(buffer, "%u\t%127[^\n]", &address, binary);


        if (result == 1 || strlen(binary) == 0) {

            fprintf(dest, "%04u 00001\n", address);
        } else if (result == 2) {

            unsigned int octal = binaryToOctal(binary);


            fprintf(dest, "%04u %05o\n", address, octal);
        } else {
            fprintf(stderr, "Failed to parse line: %s", buffer);
        }
    }

    fclose(src);
    fclose(dest);
}
