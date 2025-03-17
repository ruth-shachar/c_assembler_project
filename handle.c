#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "first_pass.h"
int check_valid_data_comma(char* rest_of_line) {
	int i = 0;
	int length = strlen(rest_of_line);

	/* Check if the first character is a comma*/
	if (rest_of_line[i] == ',') {
		return 0; /*Invalid format*/
	}

	/* Iterate through the rest of the string*/
	for (i = 0; i < length; i++) {
		/*Check if the current character is a comma*/
		if (rest_of_line[i] == ',') {
			/* Check if the next character is also a comma or if the next character doesn't exist*/
			if (i == length - 1 || rest_of_line[i + 1] == ',') {
				return 0; /*Invalid format*/
			}
		}
	}
	/*Check if the last character is a comma*/
	if (rest_of_line[length - 1] == ',') {
		return 0; /*Invalid format*/
	}

	return 1; /*Valid format*/
}
/* Function to check if a string is a valid integer (positive or negative) */
int is_valid_integer(char* rest_of_line) {
    /*Handle empty strings or strings with only non-digit characters*/
    if (*rest_of_line == '\0') return 0;

    /*Skip optional leading '+' or '-'*/
    if (*rest_of_line == '+' || *rest_of_line == '-') rest_of_line++;

    /*Ensure there are digits left after the optional sign*/
    if (*rest_of_line == '\0') return 0; /* Must have at least one digit*/

    /*Check if the remaining string is composed only of digits*/
    while (*rest_of_line) {
        if (!isdigit(*rest_of_line)) return 0; /*If it's not a digit, invalid*/
        rest_of_line++;
    }

    return 1;
}
int starts_and_ends_with_quote(char* rest_of_line) {
	int length = strlen(rest_of_line);

	/* אם המחרוזת ריקה, היא נחשבת תקינה*/
	if (length == 0) {
		return 1; /*תקינה*/
	}

	/*וק אם התו הראשון והאחרון הם תו כפול*/
	if (rest_of_line[0] == '"' && rest_of_line[length - 1] == '"') {
		return 1; /*תקינה*/
	}

	return 0; /*לא תקינה*/
}

char* trim_whitespace(char* str) {
	char* end;

	/* Trim leading space */
	while (isspace((unsigned char)*str)) str++;

	/* All spaces? */
	if (*str == 0)
		return str;

	/* Trim trailing space */
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) end--;

	/* Write new null terminator character */
	end[1] = '\0';

	return str;
}

/* Function to check if a line is empty */
int is_empty_line(char* line) {
	char* trimmed_line = trim_whitespace(line);
	return strlen(trimmed_line) == 0;
}

void save_data_line(char* name_of_file, short num_of_line, char* content_of_line, line_data** line_data_head) {
	line_data* new_line = (line_data*)malloc(sizeof(line_data));
	if (new_line == NULL) {
		perror("Failed to allocate memory for new line");
		exit(EXIT_FAILURE);
	}

	new_line->file_name = duplicate(name_of_file);  /* Copy the string */
	new_line->number = num_of_line;
	new_line->data = duplicate(content_of_line);  /* Copy the string */
	new_line->next = NULL;

	if (*line_data_head == NULL) {
		*line_data_head = new_line;
	}
	else {
		line_data* current = *line_data_head;  /* Correctly initialize current to point to the head of the list */
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_line;
	}
}
