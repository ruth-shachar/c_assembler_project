#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "first_pass.h"
#include "pre_assembler.h"
#include "second_pass.h"
#define stop_with_error()                                 \
    do                                                    \
    {                                                     \
        free_label_list(label_head);                      \
        free_instruction_memory(instruction_memory_head); \
        return 0;                                         \
    } while (0)

int implement_first_pass(char file_name[],node **head)
{
    
    int is_valid_file = 1;
    /* string to save the current line */
    char str[MAX_LINE_LENGTH] = {0};
    /* strings to divide the line into sections */
    char first_word[MAX_LINE_LENGTH] = {0};
    char second_word[MAX_LINE_LENGTH] = {0};
    char rest_of_line[MAX_LINE_LENGTH] = {0};
    /* strings to save the lines to write into entry and extern files */
    /* string to handle the name of files */
    char *temp_ob_file, *ob_file;

    FILE *fp, *temp_ob_p;

    /* Initialize IC and DC pointers */
    int IC = IC_INIT_VALUE;
    int IC_CURRENT = IC_INIT_VALUE;
    int extern_address = 0;
    int DC = 0;
    int DC_CURRENT = 0;
    instruction_memory *instruction_memory_head = NULL;
    data_image *data_image_head = NULL;
    label *label_head = NULL;

    int line = 0;

    /* opening the am file to read */
    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf("Failed to open file: %s\n", file_name);
        return 0;
    }

    temp_ob_file = add_new_file(file_name, ".tob");
    temp_ob_p = fopen(temp_ob_file, "w");
    if (temp_ob_p == NULL)
    {
        printf("Failed to open file: %s\n", temp_ob_file);
        fclose(fp); /* Close the opened file before returning */
        return 0;
    }

    ob_file = add_new_file(file_name, ".ob");

    /* read line from file and parsing it */
    while (fgets(str, MAX_LINE_LENGTH, fp))
    {
        line++;
        IC_CURRENT = IC; /*save the current ic before change*/
        DC_CURRENT = DC;
        memset(first_word, 0, MAX_LINE_LENGTH);
        memset(second_word, 0, MAX_LINE_LENGTH);
        memset(rest_of_line, 0, MAX_LINE_LENGTH);

        sscanf(str, "%s %s %s", first_word, second_word, rest_of_line);
        if (*str == ';')
        { /* if its a comment, ignor. */
            continue;
        }
        if (endsWithColon(first_word))
        { /* optional label */
            if (is_instruction(second_word))
            {
                if (!strcmp(first_word, ".entry") || !strcmp(first_word, ".extern"))
                {
                    continue; /*ignor from defination label on .entry/.extern */
                }
                else if (instruction_data_process(str, second_word, rest_of_line, &DC, line, &data_image_head))
                {
                    if (!label_process(first_word, &DC_CURRENT, &label_head, ".data"))
                    {
                        is_valid_file = 0; /*or label or instruction wrong*/
                    }
                }
                else
                {
                    printf("invalid instruction in line: %d\n", line);
                    is_valid_file = 0; /*or label or instruction wrong*/
                }
            }
            else if (opcode_detection(second_word))
            {
                if (opcode_process(second_word, rest_of_line, &IC, line, &instruction_memory_head))
                {
                    if (!label_process(first_word, &IC_CURRENT, &label_head, ".code"))
                    {
                        printf("invalid label in line: %d\n", line);
                        is_valid_file = 0;
                        ; /*invalid label */
                    }
                }
                else
                {
                    printf("invalid code line: %d\n", line);
                    is_valid_file = 0; /*invalid opcode*/
                }
            }
        }
        else if (is_instruction(first_word))
        {
            if (!strcmp(first_word, ".entry"))
            {

                continue;
            }
            else if (!strcmp(first_word, ".extern"))
            {
                if (!label_process(second_word, &extern_address, &label_head, ".external"))
                    is_valid_file = 0;
            }
            else if (!instruction_data_process(str, first_word, second_word, &DC, line, &data_image_head))
            {
                is_valid_file = 0;
            }
        }
        else if (opcode_detection(first_word))
        {
            if (!opcode_process(first_word, second_word, &IC, line, &instruction_memory_head))
            {
                is_valid_file = 0;
            }
        }
        else
        {
            if (is_empty_line(str))
            {
                continue;
            }
            else
            {
                printf("Error: Unrecognized line format in line %d: %s\n", line, str);
                is_valid_file = 0;
            }
        }
    }
    update_data_label(label_head, IC);
    /* end first pass and parsing the line without entry  */

    if (!is_valid_file)
    {
        return 0;
    }
    if (!implement_second_pass(file_name, &label_head, &instruction_memory_head, &data_image_head))
    {
        printf("second pass failed\n");
        is_valid_file = 0;
    }
    printf("File closed: %s\n", file_name);
    print_memory(instruction_memory_head, data_image_head, IC, DC, temp_ob_p);
    fclose(temp_ob_p);
    copyAndConvertFile(temp_ob_file, ob_file);
    remove(temp_ob_file);
    fclose(fp);

    free_label_list(label_head);
    free_instruction_memory(instruction_memory_head);
    return is_valid_file;
}

void printlist_label(label *head)
{
    while (head)
    {
        printf("The name of label is: %s\n", head->name_of_label);
        printf("The address of label: %d\n", head->address_of_label);
        printf("The type of label: %s\n", head->type_of_label);
        head = head->next;
    }
}

void print_memory(instruction_memory *instruction_head, data_image *data_head, int IC, int DC, FILE *fp)
{
    fprintf(fp, "   %d %d\n", IC - IC_INIT_VALUE, DC);
    while (instruction_head)
    {
        fprintf(fp, "%d          %s\n", instruction_head->address, instruction_head->binary_str);
        instruction_head = instruction_head->next;
    }
    while (data_head)
    {
        data_head->address += IC;
        fprintf(fp, "%d          %s\n", data_head->address, data_head->binary_value);
        data_head = data_head->next;
    }
}
