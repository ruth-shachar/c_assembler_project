#include "globals.h"
#include <stdbool.h>

/**
 * @brief Implements macro substitution in a given file.
 *
 * This function performs the following steps to process macros in the file:
 * 1. Removes extra spaces from the file.
 * 2. Adds macro definitions to a linked list.
 * 3. Removes macro declarations from the file.
 * 4. Replaces macro occurrences with their definitions.
 * 5. Cleans up temporary files and memory.
 *
 * @param file_name The name of the input file to process.
 * @return int Returns 1 if the macro implementation is successful, otherwise returns 0.
 */
int implement_macro(char file_name[],node **head);


/**
 * This function removes all extra unnecessary white spaces from the file
 * @param file_name string of the input file name
 * @return string the name of the new file after white spaces removed
 */
char* remove_extra_spaces_file(char file_name[]);


/**
 * This function removes extra unnecessary white-spaces from a string
 * @param str string to change
 */
void remove_extra_spaces_str(char *str);


/**
 * This function checks of a character is a ' ' or '\t'
 * @param c character to check
 * @return 1 if the character is either ' ' or '\t', 0 if not
 */
int is_space_or_tab(char c);


/**
 * This function removes white-spaces next to a comma
 * @param str string to change
 */
void remove_spaces_next_to_comma(char *str);


/**
 * This function saves a new name for a file. It deletes the content of the name after the '.' if one exists
 * and adds a new ending
 * @param name_file string of the beginning of the new file name
 * @param ending string of the ending of the new file name
 * @return a string with the new file name
 */
char * add_new_file(char * name_file,char * ending);


/**
 * @brief Checks if a macro name is valid.
 *
 * This function determines if the given macro name is valid by checking if it does not match
 * any known instruction, opcode, register, or extra characters. A macro name is considered valid
 * if it is not recognized as any of these categories.
 *
 * @param name_macr A pointer to the string representing the macro name to be checked.
 * @return 1 if the macro name is valid, or 0 if it matches an instruction, opcode, register, or has extra characters.
 */
int is_valid_macro_name(char *as_file );


/**
 * @brief Checks if a string matches a known instruction.
 *
 * This function compares the given string to a list of known instructions. If a match is found,
 * the function returns 1. If no match is found or if the input string is NULL, the function returns 0.
 *
 * @param str A pointer to the string to be checked.
 * @return 1 if the string matches a known instruction, or 0 if the string is not a valid instruction or is NULL.
 */
int instr_detection(char *str);


/**
 * @brief Checks if a string matches a known opcode.
 *
 * This function compares the given string to a list of known opcodes. If a match is found,
 * the function returns 1. If no match is found or if the input string is NULL, the function returns 0.
 *
 * @param str A pointer to the string to be checked.
 * @return 1 if the string matches a known opcode, or 0 if the string is not a valid opcode or is NULL.
 */
int opcode_detection(char *str);


/**
 * @brief Detects if a string corresponds to a known register and returns its number.
 *
 * This function checks if the provided string matches one of the known register names.
 * If the string matches a register, the function returns the register number. It returns
 * `0` if the string does not correspond to a valid register.
 *
 * The function compares the input string with a list of known registers. If a match is found,
 * the register number associated with the matched register is returned. If no match is found,
 * the function returns `0`.
 *
 * @param str The string to be checked.
 * @return The number of the register if a match is found, `0` otherwise.
 */
int reg_detection(char *str);

/**
 * @brief Checks if a string contains more than one token when split by spaces.
 *
 * This function examines a string to determine if it contains more than one token when split
 * by spaces. It returns `1` if an extra token is found (i.e., more than one token), and `0` if
 * only one token is present or if the string is empty.
 *
 * The function allocates memory to create a copy of the input string for processing. If memory 
 * allocation fails, it prints an error message and terminates the program.
 *
 * @param str The string to be checked.
 * @return `1` if more than one token is found, `0` otherwise.
 */
int extra_char_detection(char *str);


/**
 * @brief Checks if a given string ends with a colon (`:`), indicating it might be a label.
 *
 * This function examines the last character of a string to determine if it is a colon (`:`).
 * If the string is empty, the function returns `false`. Otherwise, it returns `true` if the
 * last character is a colon, and `false` otherwise.
 *
 * @param str The string to be checked.
 * @return `true` if the string ends with a colon, `false` otherwise.
 */
bool endsWithColon(const char *str);

/**
 * This function allocates new memory and handles the errors that might occur
 * @param size the amount of memory to allocate
 * @return a void pointer of the allocated memory, or NULL if the allocation failed
 */
void *handle_malloc(size_t size);


/**
 * @brief Creates a new node with the given macro name, content, and line number.
 *
 * This function allocates memory for a new node, initializes its fields with the provided
 * macro name, content, and line number, and returns a pointer to the newly created node.
 *
 * @param name The name of the macro to be stored in the node.
 * @param content The content associated with the macro.
 * @param line_num The line number where the macro is defined.
 * @return A pointer to the newly created node, or NULL if memory allocation fails.
 */
node *make_node(char *name, char *content, int line_num);


/**
 * @brief Searches for a macro name in a linked list and checks if it's already defined.
 *
 * This function recursively searches through a linked list to find if a given macro name
 * is already present and defined within a line that contains the keyword "macr".
 * If found, the function sets the `found` flag to 1 and returns a pointer to the node.
 * If the macro name is not found, the function returns a pointer to the last node in the list.
 *
 * @param head A pointer to the head node of the linked list.
 * @param name The name of the macro to search for in the list.
 * @param line The current line being processed, used to check for the keyword "macr".
 * @param found A pointer to an integer flag that is set to 1 if the macro name is found.
 * @return A pointer to the node where the macro name is found, or the last node in the list if not found.
 */
node *search_list(node *head, char *name,char *line, int *found);


/**
 * @brief Adds a node to a linked list.
 *
 * This function adds a new node of a macro to the end of a linked list
 * @param head a pointer to head of a linked list where the macros were saved
 * @param name a string with the name of the new macro
 * @param content a string with the content of the new macro
 * @param line_num the line number in the source file where the macro was defined
 * @param temp a pointer to the last node in the linked list (or NULL if the list is empty).
 */
void add_macro_to_list(node **head, char *name, char *content, int line_num,node *temp);

/**
 * @brief Frees a node in the linked list.
 *
 * This function frees the memory allocated for a node in the linked list of macros.
 * It releases the memory for the macro's name, the macro's content, and the node itself.
 *
 * @param node1 Pointer to the node to be freed.
 */
void free_node(node *node1);

/**
 * @brief Frees all nodes in a linked list.
 *
 * This function iterates through a linked list and frees the memory
 * allocated for each node and its contents.
 *
 * @param head Pointer to the head of the linked list to be freed.
 */
void free_list(node *head);


/**
 * @brief Adds macros from a file to a linked list.
 *
 * This function reads a file line by line, identifies macros defined in the file, 
 * and adds them to a linked list. 
 *
 * @param file_name The name of the file to read macros from.
 * @param head A pointer to the head of the linked list where the macros will be added.
 * @return Returns 1 if macros were added successfully, or -1 if there was an error opening the file.
 */
int add_macro(char *file_name, node **head);


/**
 * @brief Removes macro declarations from a file.
 *
 * This function processes an input file line by line, removes any lines 
 * that are part of a macro declaration (from "macr" to "endmacr"), and writes 
 * the remaining content to a new file with a ".t02" extension.
 *
 * @param file_name The name of the input file to process.
 * @return A pointer to a string containing the name of the new file without macro declarations, or NULL if an error occurs.
 */
char *remove_mcros_decl(char file_name[]);


/**
 * @brief Replaces all macro occurrences in a file with their corresponding content.
 *
 * This function reads an input file line by line, checks each line for a macro name,
 * and replaces the macro with its content if found. The resulting content is written
 * to a new file with a ".am" extension.
 *
 * @param file_name The name of the input file to process.
 * @param head A pointer to the head of the linked list containing macro names and their content.
 * @return A pointer to a string containing the name of the new file with macros replaced, or NULL if an error occurs.
 */
char * replace_all_mcros(char *file_name, node *head);
/**
 * @brief Checks if the string is valid.
 *
 * This function checks whether the string `str` is considered valid
 * based on the following criteria:
 * 1. If the string is empty ( it contains no characters), it is considered valid.
 * 2. If the string contains only whitespace characters (spaces, tabs, etc.), it is considered valid.
 * 3. If the string contains any non-whitespace characters, it is considered invalid.
 *
 * @param str A pointer to the string to be checked.
 * @return int Returns 1 if the string is valid (empty or contains only whitespace),
 *             and 0 if the string is invalid (contains any non-whitespace characters).
 */
int is_rest_of_line_valid(char *str);



void printlist(node *head);
