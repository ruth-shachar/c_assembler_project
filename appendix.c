#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pre_assembler.h"
#include "globals.h"

/* this function relate to main and add the ending to files */
char* add_new_file(char* file_name, char* ending) {
	char* c, * new_file_name;
	new_file_name = handle_malloc(MAX_LINE_LENGTH * sizeof(char));
	strcpy(new_file_name, file_name);
	/* deleting the file name if a '.' exists and forth */
	if ((c = strchr(new_file_name, '.')) != NULL) {
		*c = '\0';
	}
	/* adds the ending of the new file name */
	strcat(new_file_name, ending);

	return new_file_name;
}
/*this functions cleaning the files from extra space*/
void remove_extra_spaces_str(char* str) {
	int i, j;
	char str_temp[MAX_LINE_LENGTH];
	i = j = 0;
	/* eliminating white-spaces in the beginning of the line */
	while (is_space_or_tab(*(str + i))) {
		i++;
	}
	while (*(str + i) != '\0') {
		/* copying character */
		while (!is_space_or_tab(*(str + i)) && *(str + i) != '\0') {
			*(str_temp + j) = *(str + i);
			i++;
			j++;
		}
		/* if loop stopped because end of line char */
		if (*(str + i) == '\0') {
			break;
		}
		/* if loop stopped because of a white-space skipping them until another character is encountered*/
		while (is_space_or_tab(*(str + i))) {
			i++;
		}
		/* if stopped not because of end of line char then copy one space for all the others that were skipped */
		if (!(*(str + i) == '\n' || *(str + i) == '\0')) {
			*(str_temp + j) = ' ';
			j++;
		}
	}
	*(str_temp + j) = *(str + i);
	*(str_temp + j + 1) = '\0';
	remove_spaces_next_to_comma(str_temp);
	strcpy(str, str_temp);
}

int is_space_or_tab(char c) {
	/* Check if the char is a space or a tab */
	return (isspace(c) && c != '\n');
}

void remove_spaces_next_to_comma(char* str) {
	char* ptr = str;
	/* If the line starts with ',' avoiding accessing outside the str */
	if (*ptr == ',') {
		return;
	}
	while ((ptr = strchr(ptr, ',')) != NULL) {
		/* Space before the comma */
		if (*(ptr - 1) == ' ') {
			memmove(ptr - 1, ptr, strlen(ptr) + 1);
			if (*(ptr) == ' ') {
				/* Also space after the comma */
				memmove(ptr, ptr + 1, strlen(ptr + 1) + 1);
			}
		}
		else if (*(ptr + 1) == ' ') {
			/* Only space after the comma */
			memmove(ptr + 1, ptr + 2, strlen(ptr + 2) + 1);
			ptr++;
		}
		else {
			ptr++;
		}
	}
}

void* handle_malloc(size_t size) {
	void* ptr = malloc(size);
	if (ptr == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		exit(EXIT_FAILURE);
	}
	return ptr;
}
node* make_node(char* name, char* content, int line_num) {
	node* temp;

	/* Check if memory allocation for the node succeeded */
	temp = handle_malloc(sizeof(node));

	temp->macro_name = name;        /* Set the name of the node */
	temp->macro_content = content;  /* Set the content string of the node */
	temp->macro_line = line_num;    /* Set the line number associated with the content */
	temp->next = NULL;        /* Initialize the next pointer to NULL */
	return temp;  /* Return a pointer to the newly created node */
}
node* search_list(node* head, char* name, char* line, int* found) {
	*found = 0;

	/* If the list is empty */
	if (head == NULL) {
		return NULL;
	}

	/* fix to check if this is defination*/
	if ((strcmp(name, head->macro_name) == 0) && (strstr(line, "macr") != NULL)) {
		*found = 1;
		printf("Node %s already exists in the list\n", name);
		return head;
	}

	/* If the end of the list is reached */
	if (head->next == NULL) {
		return head;
	}

	/* Recursively search the rest of the list */
	return search_list(head->next, name, line, found);
}
int is_valid_macro_name(char* name_macr) {

	return(!instr_detection(name_macr) && !opcode_detection(name_macr) && !reg_detection(name_macr));
}

void add_macro_to_list(node** head, char* name, char* content, int line_num, node* temp) {
	int found = 0;
	node* new_node;

	/* Create a copy of the content to store in the new node */
	char* content_copy = handle_malloc(strlen(content) + 1);
	strcpy(content_copy, content);

	if (found) {
		if (strcmp(temp->macro_content, content_copy) != 0) {
			printf("ERROR_CODE_13\n");
			free(name);
			free(content_copy);
			return;
		}
	}
	else {
		new_node = make_node(name, content_copy, line_num);

		if (temp == NULL) {
			*head = new_node;
		}
		else {
			temp->next = new_node;
		}
	}
}


char* remove_mcros_decl(char file_name[]) {
	char* token, * new_file;
	char str[MAX_LINE_LENGTH];
	char str_copy[MAX_LINE_LENGTH];
	FILE* inFile, * outFile;

	/* Open the input file for reading */
	inFile = fopen(file_name, "r");
	if (inFile == NULL) {
		printf("Failed to open file for reading");
		return NULL;
	}

	/* Create a new filename with the ".t02" extension */
	new_file = add_new_file(file_name, ".t02");

	/* Open the new file for writing */
	outFile = fopen(new_file, "w");
	if (outFile == NULL) {
		printf("Failed to open new file for writing");
		free(inFile);
		remove(new_file);
		free(new_file);
		return NULL;
	}

	/* Process the input file line by line */
	while (fgets(str, MAX_LINE_LENGTH, inFile)) {
		/* copying the line so we can manipulate one copy */
		strcpy(str_copy, str);/*str_copyמעתיק את שורת המחרוזת המקורית ל*/
		token = strtok(str, " \n");/* מצביע לתחילת המחרוזת str token */

		/* blank line */
		if (token == NULL) {/* בודק אם השורה ריקה */
			fprintf(outFile, "\n");/*כותה את השורה הריקה לקובץ הפלט וממשיך ללולאה הבאה */
			continue;
		}

		/* mcro was found */
		if (strcmp(token, "macr") == 0) {
			/* Skip lines until the "endmcro" marker is found */
			while (strcmp(token, "endmacr") != 0) {
				fprintf(outFile, "\n");
				fgets(str, MAX_LINE_LENGTH, inFile);
				token = strtok(str, " \n");

				/* blank lines */
				while (token == NULL) {
					fprintf(outFile, "\n");
					fgets(str, MAX_LINE_LENGTH, inFile);
					token = strtok(str, " \n");
				}
			}
			fprintf(outFile, "\n");
		}
		else {
			/* Write the line to the new file if it's not part of a macro declaration */
			fprintf(outFile, "%s", str_copy);
		}
	}
	/*Close file ptr*/
	fclose(inFile);
	fclose(outFile);

	/* Return the name of the new file without macros */
	return new_file;
}


char* replace_all_mcros(char* file_name, node* head) {
	FILE* inFile, * outFile;
	char* new_file;
	char str[MAX_LINE_LENGTH];
	char strcopy[MAX_LINE_LENGTH];
	char *first_token;

	inFile = fopen(file_name, "r");
	if (inFile == NULL) {
		perror("Unable to open file");
		return NULL;
	}

	new_file = add_new_file(file_name, ".am");

	/* Open the new file for writing */
	outFile = fopen(new_file, "w");
	if (outFile == NULL) {
		perror("Unable to open new file");
		fclose(inFile);
		free(new_file);
		return NULL;
	}

	while (fgets(str, MAX_LINE_LENGTH, inFile)) {
		strcpy(strcopy, str);
		first_token = strtok(strcopy, " ");

		if (first_token != NULL && strtok(NULL, " ") == NULL && !is_space_or_tab(*str)) {
			node* current = head;
			int found = 0;
			while (current) {
				if (strncmp(current->macro_name, str, strlen(current->macro_name)) == 0) {
					fprintf(outFile, "%s\n", current->macro_content);
					found = 1;
					break;
				}
				current = current->next;
			}
			if (!found) {
				fprintf(outFile, "%s", str);
			}
		}
		else {
			fprintf(outFile, "%s", str);
		}
	}

	fclose(inFile);
	fclose(outFile);

	return new_file;
}

void free_node(node* node1) {
	/* Free memory allocated for the name, content and node */
	free(node1->macro_name);
	free(node1->macro_content);
	free(node1);
}

void free_list(node* head) {
	/* Free memory for the current node and its contents while storing the current node in a temporary pointer */
	while (head != NULL) {
		node* temp = head;
		head = head->next;
		free_node(temp);
	}
}

