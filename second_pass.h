#include "globals.h"
#include <stdbool.h>


/**
 * @brief Performs the second pass of the assembler process on the given file.
 *
 * This function reads an assembly source file, processes each line to handle labels, instructions, and entries,
 * and writes external labels to an external file and entry labels to an entry file. It also updates the instruction
 * and data images based on label addresses.
 *
 * @param file_name Name of the source file to be processed.
 * @param label_head Pointer to the head of the linked list of labels.
 * @param list_head Pointer to the head of the linked list of instruction memory.
 * @param data_image_head Pointer to the head of the linked list of data images.
 * @return 1 if the second pass was successful, 0 otherwise.
 */
int implement_second_pass(char file_name[],label** label_head,instruction_memory **list_head,data_image **data_image_head);


/**
 * @brief Processes each operand in a line to check for labels, convert their addresses to binary, and handle them accordingly.
 *
 * This function parses each operand in `rest_of_line`, checks if it's a label, and processes it based on the label's type.
 * If the label is external, its address is written to an external file `ext_p`. For other labels, the binary address is inserted
 * into the instruction and data images.
 *
 * @param ext_p Pointer to the file stream for external labels.
 * @param rest_of_line The line containing operands, which may include labels.
 * @param line The current line number being processed.
 * @param label_head Pointer to the head of the linked list of labels.
 * @param list_head Pointer to the head of the linked list of instruction memory.
 * @param data_image_head Pointer to the head of the linked list of data images.
 */
void chek_for_label_argument(FILE *ext_p, char * rest_of_line,int line,label** label_head,instruction_memory **list_head,data_image **data_image_head);

/**
 * @brief Checks if the given operand starts with one of the characters defined in the array.
 *
 * This function verifies if the first character of the operand matches any of the predefined characters
 * in the `arr_type_of_arg` array. It is used to determine the type of addressing mode based on the operand's prefix.
 *
 * @param operand The operand string to be checked.
 * @return `true` if the operand starts with one of the predefined characters; `false` otherwise.
 */
bool check_operand_start(char *operand);


/**
 * @brief Inserts the binary address of a label into the instruction memory at a specific line.
 *
 * This function searches through the `instruction_memory` linked list for a node with the given line number.
 * If the node is found and its `binary_str` is empty, it updates the `binary_str` with the provided `address_label_binary`.
 *
 * @param head Pointer to the head of the `instruction_memory` linked list.
 * @param data_image_head Pointer to the head of the `data_image` linked list (not used in this function).
 * @param address_label_binary The binary representation of the label's address to be inserted.
 * @param line The line number where the label address should be inserted.
 */
void insert_label_address(instruction_memory *head,data_image *data_image_head,char *address_label_binary,int line);


/**
 * @brief Checks if a label is valid for use as an entry and returns its address if valid.
 *
 * This function searches for a label in the linked list of labels to verify its existence and type. 
 * If the label is found and its type is not ".external", the function returns its address. 
 * If the label is not found or its type is ".external", an error message is printed.
 *
 * @param name_of_label The name of the label to check.
 * @param label_head Pointer to the head of the `label` linked list.
 * 
 * @return The address of the label if it is valid for use as an entry; 0 otherwise.
 */
int check_valid_entry(char* name_of_label, label* label_head);

/**
 * @brief Finds the address of an instruction in memory based on its line number.
 *
 * This function searches through the `instruction_memory` linked list for a node with the specified line number.
 * If the node is found and its `binary_str` matches a specific binary pattern ("000000000000001"), the function returns the address of that instruction.
 *
 * @param head Pointer to the head of the `instruction_memory` linked list.
 * @param line The line number of the instruction whose address is to be found.
 * @return The address of the instruction if found, otherwise 0.
 */
int find_address(instruction_memory *head,int line);
