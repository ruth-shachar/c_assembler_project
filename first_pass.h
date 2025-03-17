#include "globals.h"
#include <stdbool.h>

/**
 * @brief Performs the first pass on the assembly file.
 *
 * This function processes an assembly file in the first pass, which includes:
 * 1. Opening the source file and corresponding .ent and .ext files for writing.
 * 2. Parsing each line of the file to detect labels, instructions, and directives.
 * 3. Storing labels and their associated addresses in a linked list.
 * 4. Handling .entry and .extern directives.
 * 5. Processing data and instruction memory storage for later use in the second pass.
 * 6. Handling errors related to invalid labels, instructions, or opcodes.
 * 7. Finalizing the first pass by updating label addresses and preparing for the second pass.
 * 8. Printing memory and label data for verification.
 *
 * @param file_name The name of the file to be processed in the first pass.
 * @param head A pointer to the head of the linked list for storing macro definitions.
 * @return Returns 0 if the first pass was completed successfully, or an error code if a failure occurred.
 */
int implement_first_pass(char file_name[],node **head);  


/**
 * @brief Processes a label in the assembly code.
 *
 * This function handles the detection and validation of labels in the assembly code, performing the following steps:
 * 1. Removes the colon (`:`) at the end of the label if present.
 * 2. Checks if the label length exceeds the maximum allowed length.
 * 3. Searches for the label in the existing linked list of labels.
 * 4. If the label is not found and is valid, it adds the label to the linked list with its corresponding address.
 *
 * @param first_word The label to be processed (e.g., "LOOP:").
 * @param p_address A pointer to the address associated with the label (e.g., instruction or data address).
 * @param line The current line number being processed in the assembly file.
 * @param label_head A pointer to the head of the linked list where labels are stored.
 * @param type_of_label A string indicating the type of the label (e.g., ".code", ".data", ".external").
 * @param head A pointer to the head of the linked list for storing macro definitions.
 * @return Returns 1 if the label was processed and added successfully, 0 if the label was invalid or already existed.
 */

int label_process(char* first_word, int* p_address, label** label_head,char *type_of_label);

/**
 * @brief Processes an instruction related to data in the assembly code.
 *
 * This function is responsible for handling data-related instructions in the assembly code. 
 * It performs the following steps:
 * 1. Detects and processes data instructions using the `instr_data_detection` function.
 * 2. If the instruction is not recognized, it prints an error message indicating an undefined instruction.
 *
 * @param str The current line of assembly code being processed.
 * @param first_word The first word in the line, typically representing the instruction.
 * @param rest_of_line The remaining part of the line after the first word, containing the data or operands.
 * @param DC A pointer to the Data Counter (DC), which tracks the memory address for data storage.
 * @param line The current line number being processed in the assembly file.
 * @param data_image_head A pointer to the head of the linked list where data instructions are stored.
 * @return Returns 1 if the instruction was successfully processed, 0 if the instruction was undefined.
 */
int instruction_data_process(char *str,char *first_word, char *rest_of_line,int * DC,int line,data_image **data_image_head);

/**
 * @brief Processes an opcode instruction in the assembly code.
 *
 * This function handles the parsing and conversion of an opcode instruction into its binary form
 * and stores it in the instruction memory. It performs the following steps:
 * 1. Validates the number of arguments for the given opcode.
 * 2. Parses the arguments and converts them into binary strings.
 * 3. Adds the binary representation of the opcode and its arguments to the instruction memory.
 * 4. Increments the Instruction Counter (IC) for each piece of binary data added to the memory.
 * 5. Frees allocated memory used for binary strings after processing.
 *
 * @param first_word The opcode to be processed.
 * @param rest_of_line The remaining part of the line containing the operands.
 * @param IC A pointer to the Instruction Counter (IC), tracking the current memory address for instructions.
 * @param line The current line number in the assembly file.
 * @param instruction_memory_head A pointer to the head of the instruction memory linked list.
 * @param head A pointer to the head of the macro linked list.
 * @return Returns 1 if the opcode and its arguments were successfully processed, 0 if an error occurred.
 */
int opcode_process(char *first_word, char *rest_of_line,int * IC,int line,instruction_memory **instruction_memory_head);

/**
 * @brief Converts a decimal number to a binary representation.
 *
 * This function converts a given decimal number into its binary representation and stores
 * it in the provided character array. The binary string is right-aligned and padded with '0'
 * to match the specified length.
 *
 * @param length The length of the binary representation, including the null terminator.
 * @param decimal The decimal number to be converted to binary.
 * @param binary The character array where the binary representation will be stored. 
 *               It should have at least 'length' + 1 elements to accommodate the null terminator.
 */
void convert_to_binary(int length, int decimal, char binary[]);


/**
 * @brief Converts a decimal number to its binary representation, handling negative numbers.
 *
 * This function converts a given decimal number to its binary representation and stores
 * it in the provided character array. The binary string is padded with '0's to ensure it
 * matches the specified length. If the number is negative, the function computes its two's
 * complement representation.
 *
 * @param length The length of the binary representation, including the null terminator.
 * @param decimal The decimal number to be converted to binary.
 * @param binary The character array where the binary representation will be stored. 
 *               It should have at least 'length' + 1 elements to accommodate the null terminator.
 */
void decimal_to_binary(int length, int decimal, char binary[]);

/**
 * @brief Detects and processes data-related instructions in a given string.
 *
 * This function checks if the instruction is related to data definition (".data" or ".string") 
 * and processes it accordingly. It updates the data image and data count based on the instruction.
 *
 * @param str The input string that contains the instruction and additional data.
 * @param first_word The instruction keyword (e.g., ".data" or ".string").
 * @param rest_of_line A buffer to store the remaining part of the line after the instruction keyword.
 * @param DC A pointer to the data count (DC) which is updated during processing.
 * @param line The line number where the instruction was found.
 * @param data_image_head A pointer to the head of the data image linked list, which is updated during processing.
 *
 * @return 1 if the instruction was recognized and processed successfully, 0 otherwise.
 */
int instr_data_detection(char *str,char *first_word, char *rest_of_line,int * DC,int line,data_image **data_image_head);

/**
 * @brief Detects if a given string is a valid instruction.
 *
 * This function checks whether the input string matches any of the known instructions 
 * by comparing it against a predefined list of instructions. 
 *
 * @param str The input string to be checked for validity as an instruction.
 *
 * @return 1 if the string matches a known instruction, 0 otherwise.
 */
int instr_detection(char *str);

/**
 * @brief Detects if a given string is a valid opcode.
 *
 * This function checks whether the input string matches any of the known opcodes 
 * by comparing it against a predefined list of opcodes. 
 *
 * @param str The input string to be checked for validity as an opcode.
 *
 * @return 1 if the string matches a known opcode, 0 otherwise.
 *
 * @note This function uses the global array `OPCODES` which holds the list of known opcodes
 *       and the constant `OPCODES_COUNT` that represents the number of opcodes in the list.
 *       Each element in `OPCODES` is expected to be a struct with a member `name_of_opcode`
 *       that stores the name of the opcode.
 */
int opcode_detection(char *str);

/**
 * @brief Detects if a given string is a valid register and returns its number.
 *
 * This function checks whether the input string matches any of the known registers
 * by comparing it against a predefined list of registers. It returns the number
 * of the register if found, otherwise, it returns 0.
 *
 * @param str The input string to be checked for validity as a register.
 *
 * @return The number of the register if the string matches a known register,
 *         0 otherwise.
 */
int reg_detection(char *str);

/**
 * @brief Detects if there are extra characters in a string beyond the first token.
 *
 * This function checks whether there are any additional tokens (i.e., extra characters) 
 * in the input string after the first token. It tokenizes the string based on spaces and 
 * determines if there is more than one token.
 *
 * @param str The input string to be checked for extra characters.
 *
 * @return 1 if extra characters are found beyond the first token, 0 otherwise.
 *
 * @note This function allocates memory for a copy of the input string to avoid modifying
 *       the original string. The copy is tokenized using `strtok` to split the string by spaces.
 *       After checking for extra tokens, the allocated memory is freed.
 */
int extra_char_detection(char *str);

/**
 * @brief Checks if the given string ends with a colon.
 *
 * This function determines whether the provided string ends with a colon (':').
 * It is useful for identifying labels in a line of code where labels are typically
 * denoted by a trailing colon.
 *
 * @param str The string to be checked.
 *
 * @return `true` if the string ends with a colon, `false` otherwise.
 */
bool endsWithColon(const char *str);

/**
 * @brief Checks if the given string is alphanumeric.
 *
 * This function determines whether every character in the provided string is either
 * a letter or a digit. It is useful for validating identifiers or labels that should
 * consist solely of alphanumeric characters.
 *
 * @param str The string to be checked.
 *
 * @return `true` if all characters in the string are alphanumeric, `false` otherwise.
 */
bool is_alphanumeric_string(const char* str);

/**
 * @brief Checks if the given string represents an instruction.
 *
 * This function determines if the provided string begins with a period ('.'),
 * which is used to denote instructions or directives in certain assembly languages.
 *
 * @param str The string to be checked.
 *
 * @return `1` if the string starts with a period ('.'), indicating it is an instruction;
 *         `0` otherwise.
 */
int is_instruction(char * str);


/**
 * @brief Validates and processes a string for insertion into the data image.
 *
 * This function performs the following steps:
 * 1. Checks if the provided string starts and ends with double quotes.
 * 2. If valid, it processes each character in the string (excluding the quotes) and adds it to the data image.
 * 3. Adds a null terminator (`'\0'`) to the data image to mark the end of the string.
 * 4. Updates the data counter (`DC`) accordingly.
 * 5. If the string does not start and end with quotes, it prints an error message indicating the line number.
 *
 * @param rest_of_line The string to be validated and processed.
 * @param DC Pointer to the data counter that tracks the current position in the data image.
 * @param line The line number where the string was found, used for error reporting.
 * @param data_image_head Pointer to the head of the data image linked list where the string will be added.
 */
void check_valid_string(char *rest_of_line,int * DC,int line,data_image **data_image_head);

/**
 * @brief Validates and processes data from a string for insertion into the data image.
 *
 * This function performs the following steps:
 * 1. Checks if the string format is valid by verifying comma separation.
 * 2. Creates a copy of the input string to avoid modifying the original.
 * 3. Tokenizes the string using commas as delimiters.
 * 4. For each token:
 *    - Checks if it represents a valid integer.
 *    - Adds the integer value to the data image.
 *    - Updates the data counter (`DC`).
 * 5. If any token is not a valid integer, prints an error message with the line number.
 * 6. If the data format is invalid, prints an error message with the line number.
 *
 * @param rest_of_line The string containing the data to be validated and processed.
 * @param DC Pointer to the data counter that tracks the current position in the data image.
 * @param line The line number where the data was found, used for error reporting.
 * @param data_image_head Pointer to the head of the data image linked list where the data will be added.
 */
void check_valid_data(char *rest_of_line, int * DC,int line,data_image **data_image_head);

/**
 * @brief Updates the address of labels of type ".data" by adding the instruction counter (IC) value.
 *
 * This function iterates through the linked list of labels and performs the following steps:
 * 1. Checks if the label's type is ".data".
 * 2. If the label type is ".data", it updates the `address_of_label` by adding the current value of the instruction counter (IC).
 * 3. Continues to the next label in the list until all labels have been processed.
 *
 * @param label_head Pointer to the head of the linked list of labels.
 * @param IC The current value of the instruction counter to be added to the address of labels of type ".data".
 */
void update_data_label(label *label_head,int IC);

/**
 * @brief Checks the validity of the data format in the string, ensuring proper comma usage.
 *
 * This function performs the following checks on the input string:
 * 1. Ensures the string does not start with a comma.
 * 2. Ensures that no two consecutive commas exist.
 * 3. Ensures that the string does not end with a comma.
 *
 * @param rest_of_line The string to be checked for valid comma usage in a data format.
 * 
 * @return 1 if the format is valid, 0 otherwise.
 */
int check_valid_data_comma(char *rest_of_line);

/**
 * @brief Checks if the given string represents a valid integer.
 *
 * This function performs the following checks:
 * 1. Handles empty strings or strings with only non-digit characters.
 * 2. Skips optional leading '+' or '-' signs.
 * 3. Ensures that there are digits left after any optional sign.
 * 4. Checks if the remaining string is composed only of digits.
 *
 * @param rest_of_line The string to be checked for integer validity.
 * 
 * @return 1 if the string is a valid integer, 0 otherwise.
 */
int is_valid_integer(char *token);

/**
 * @brief Checks if the given string starts and ends with a double quote.
 *
 * This function verifies that:
 * 1. The string is not empty.
 * 2. The first and last characters of the string are double quotes (").
 *
 * @param rest_of_line The string to be checked.
 * 
 * @return 1 if the string starts and ends with a double quote, 0 otherwise.
 */
int starts_and_ends_with_quote(char *rest_of_line);

/**
 * @brief Trims leading and trailing whitespace characters from a string.
 *
 * This function removes any whitespace characters from the beginning and end of the given string.
 * It does not modify the string if it is entirely whitespace.
 *
 * @param str The string to be trimmed.
 * 
 * @return A pointer to the trimmed string.
 */
char *trim_whitespace(char *str);

/**
 * @brief Checks if a line is empty or contains only whitespace characters.
 *
 * This function trims leading and trailing whitespace characters from the given line
 * and checks if the resulting string is empty. A line is considered empty if it contains
 * only whitespace characters or is completely blank.
 *
 * @param line The line to be checked.
 * 
 * @return 1 if the line is empty or contains only whitespace characters, 0 otherwise.
 */
int is_empty_line(char *line);

/**
 * @brief Finds a label by its name in the linked list of labels.
 *
 * This function traverses the linked list of labels to search for a label with the
 * specified name. It returns a pointer to the label if found, or NULL if the label
 * is not present in the list.
 *
 * @param name_of_label The name of the label to search for.
 * @param label_head The head of the linked list of labels.
 * 
 * @return A pointer to the label if found, NULL otherwise.
 */
label* find_label_by_name(const char *name_of_label, label* label_head);

/**
 * @brief Updates the type of a label.
 *
 * This function updates the type of a given label by freeing the existing type string
 * and assigning a new type string to the label. The new type string is duplicated using
 * the `duplicate` function.
 *
 * @param lbl A pointer to the label whose type is to be updated.
 * @param new_type The new type string to be assigned to the label.
 */
void update_label_type(label *lbl, const char *new_type);

/**
 * @brief Creates an entry file with the specified label information.
 *
 * This function searches for a label by its name in the given linked list of labels.
 * If the label is found, it creates a new file with the specified filename and writes
 * the label's name and address to the file. If the label is not found, an error message
 * is printed. If the file cannot be opened, an error message is printed using `perror`.
 *
 * @param filename The name of the file to be created.
 * @param rest_of_line The name of the label to search for in the label list.
 * @param label_head A pointer to the head of the linked list of labels.
 */
void create_entry_file(const char *filename, const char *content, label* label_head);

/**
 * @brief Adds a new data image node to the linked list.
 *
 * This function converts an integer value to its binary representation,
 * stores the binary string along with the address and line number in a new
 * node, and adds this node to the end of the linked list pointed to by
 * `data_image_head`.
 *
 * @param value The integer value to be added to the data image list.
 * @param address The address to be associated with the value.
 * @param data_image_head A pointer to the head of the linked list where the data will be stored.
 * @param line The line number in the source code where the data originates.
 */
void add_to_data_image(int value,int address, data_image** data_image_head,int line);

/**
 * @brief Checks if a given label name is valid.
 *
 * This function checks if the given `name_of_label` is valid by ensuring it does not conflict 
 * with any macro names stored in a linked list, is not an instruction, opcode, or register, 
 * does not contain extra characters, starts with an alphabetic character, and is composed 
 * only of alphanumeric characters.
 *
 * @param name_of_label The label name to be validated.
 * @return int 1 if the label is valid, 0 otherwise.
 */
int is_valid_label(char *name_of_label);

/**
 * @brief Adds a new label to the linked list of labels.
 *
 * This function allocates memory for a new label, sets its name, address, and type, 
 * and adds it to the end of the linked list of labels.
 *
 * @param name_of_label The name of the label to be added.
 * @param address_of_label A pointer to the address of the label. If NULL, the address is set to 0.
 * @param head A pointer to the pointer to the head of the linked list.
 * @param type_of_label The type of the label (e.g., data, code).
 */
void add_label_to_list(char *name_of_label, int *address_of_label, label **head,char *type_of_label);

/**
 * @brief Searches for a label in a linked list by its name.
 *
 * This function traverses a linked list of labels, searching for a label with a specific name.
 * If the label is found, it sets the `found` flag to 1, prints a message indicating the label is already defined, 
 * and returns a pointer to the label. If the label is not found, it sets the `found` flag to 0 and returns NULL.
 *
 * @param head Pointer to the head of the linked list of labels.
 * @param name_of_label The name of the label to search for.
 * @param found Pointer to an integer that will be set to 1 if the label is found, otherwise 0.
 * @return A pointer to the label if found, or NULL if not found.
 */
label* search_label_on_list(label *head, char *name_of_label, int *found);

/**
 * @brief Saves information about a line of code in a linked list.
 *
 * This function creates a new node in a linked list of `line_data` structures, storing information about a specific line of code from a file.
 * It stores the file name, line number, and line content in the newly created node and adds the node to the end of the linked list.
 *
 * @param name_of_file The name of the file where the line is from.
 * @param num_of_line The line number within the file.
 * @param content_of_line The content of the line to be stored.
 * @param line_data_head A pointer to the pointer of the head of the linked list.
 */
void save_data_line(char* name_of_file,short num_of_line,char* content_of_line,line_data ** line_data_head);

/**
 * @brief Duplicates a given string by allocating memory and copying its content.
 *
 * This function creates a duplicate of the input string by allocating sufficient memory 
 * to store the string, copying the content of the input string into the newly allocated memory, 
 * and returning a pointer to the duplicated string.
 *
 * @param str The string to be duplicated.
 * @return A pointer to the newly allocated and duplicated string. 
 *         The caller is responsible for freeing this memory when no longer needed.
 */
char* duplicate(const char* str);

/**
 * @brief Frees the memory allocated for a linked list of labels.
 *
 * This function iterates through a linked list of labels and frees the memory 
 * allocated for each label's name, type, and the label struct itself.
 *
 * @param head A pointer to the head of the linked list of labels.
 */
void free_label_list(label *head);

/**
 * @brief Converts a comma-separated string of numbers into a binary string.
 *
 * This function takes a string of comma-separated numbers, converts each number
 * to its binary representation, and concatenates them into a single binary string.
 * It also appends the specified ARE (Addressing, Relocatability, Extern) value at the end.
 *
 * @param length The length of the binary representation for each number.
 * @param str A string containing the comma-separated numbers to be converted.
 * @param ARE A string representing the ARE value to be appended to the binary string.
 */
void convert_str_to_binary(int length,char *str, char *ARE);

/**
 * @brief Validates the number of arguments in a given string against the expected number of arguments for an opcode.
 *
 * This function checks if the number of arguments in the `rest_of_line` string matches the expected number
 * for the opcode specified in `str`. It counts the number of arguments by checking for commas, 
 * and then compares this count with the expected argument count for the opcode.
 *
 * @param str The string containing the opcode name.
 * @param rest_of_line The string containing the rest of the line, which includes the arguments.
 * @param num_of_opcode A pointer to an integer that will hold the opcode number if a match is found.
 * @return 1 if the number of arguments matches the expected number for the opcode, 0 otherwise.
 */
int valid_num_argument(char * str ,char * rest_of_line,int * num_of_opcode);

/**
 * @brief Parses the opcode arguments and converts them to binary representations.
 *
 * This function processes the arguments of an opcode, determines their types and sizes, and generates binary strings for the first, second, and third words based on the detected argument types.
 *
 * @param name_of_opcode The name of the opcode to validate.
 * @param arguments The arguments string associated with the opcode.
 * @param first_word_to_binary The buffer to store the binary representation of the first word.
 * @param second_word_to_binary The buffer to store the binary representation of the second word.
 * @param third_word_to_binary The buffer to store the binary representation of the third word.
 * @param fieldBitSize1 Pointer to an integer to store the bit size of the first field.
 * @param fieldBitSize2 Pointer to an integer to store the bit size of the second field.
 * @param fieldBitSize3 Pointer to an integer to store the bit size of the third field.
 * @param detected_label_on_first_pass Pointer to an integer to track detected labels.
 * @return Returns 1 if parsing and validation are successful, otherwise returns 0.
 */
int parsing_arg(char *name_of_opcode, char *arguments,char *first_word_to_binary,char *second_word_to_binary,char *third_word_to_binary,int * fieldBitSize1,int * fieldBitSize2,int * fieldBitSize3, int *detected_label_on_first_pass);

/**
 * @brief Detects and converts an argument to its corresponding integer value or register number.
 *
 * This function analyzes the provided `argument` string to determine if it represents a valid integer or
 * a register. It then returns the appropriate value based on the type of the argument.
 *
 * - If the argument is a valid integer, the function converts it to an integer and returns it.
 * - If the argument is identified as a register, the function returns the register number (adjusted by subtracting 1).
 * - If the argument is neither a valid integer nor a recognized register, the function returns -1.
 *
 * @param argument A string representing the argument to be detected and converted.
 * @return The integer value of the argument if it is a valid integer or register number (adjusted), or -1 if invalid.
 */
int detection_argument(char *argument);

/**
 * @brief Identifies the addressing modes of source and target arguments.
 *
 * This function determines the addressing modes for source and target arguments based on their initial characters
 * and updates the corresponding type of addressing modes. It also checks if the arguments could be labels if 
 * the initial determination is inconclusive.
 *
 * - The function iterates through predefined types of addressing methods to identify the type of each argument based
 *   on its first character.
 * - If the argument's initial character does not match any predefined type, it checks if the argument could be a label.
 *
 * @param source_arg A string representing the source argument.
 * @param target_arg A string representing the target argument.
 * @param type_of_source_arg A pointer to an integer where the type of the source argument's addressing mode will be stored.
 * @param type_of_target_arg A pointer to an integer where the type of the target argument's addressing mode will be stored.
 * @param head A pointer to the head of the linked list of labels used for label validation.
 */
void identifyAddressingMode(char *target_arg,char *source_arg,int *type_of_target_arg,int *type_of_source_arg);

/**
 * @brief Validates the opcode and its associated addressing modes.
 *
 * This function checks if the provided opcode and its source and target addressing modes are valid.
 * It compares the provided opcode with a list of valid opcodes and ensures that the source and target
 * addressing modes match the expected types for the given opcode.
 *
 * @param name_of_opcode The name of the opcode to be validated.
 * @param num_of_opcode A pointer to an integer where the number associated with the opcode will be stored if valid.
 * @param type_of_target_arg The addressing mode type of the target argument.
 * @param type_of_source_arg The addressing mode type of the source argument.
 *
 * @return Returns 1 if the opcode is valid and the addressing modes match; otherwise, returns 0.
 */
int validateParameters(char *name_of_opcode,int *num_of_opcode,int type_of_target_arg,int type_of_source_arg);

/**
 * @brief Adds a new instruction to the instruction memory list.
 *
 * This function creates a new node for the instruction memory list with the provided line number, address,
 * and binary string representation of the instruction. It then adds this new node to the end of the instruction memory list.
 *
 * @param line The line number where the instruction is located.
 * @param address The address of the instruction in memory.
 * @param binary_str A string representing the binary encoding of the instruction.
 * @param instruction_memory_head A pointer to the head of the instruction memory list. This will be updated if the list is empty.
 */
void add_to_instruction_memory(int line,int address,char* binary_str , instruction_memory** instruction_memory_head);

/**
 * @brief Frees all nodes in the instruction memory list.
 *
 * This function iterates through the entire instruction memory list, freeing memory allocated for each node's
 * binary string and then freeing the node itself. The process continues until all nodes in the list are freed.
 *
 * @param head A pointer to the head of the instruction memory list.
 */
void free_instruction_memory(instruction_memory *head);

/**
 * @brief Converts the first number in a comma-separated string to binary and appends it with the remaining numbers.
 *
 * This function takes a comma-separated string of numbers and converts each number to its binary representation. 
 * The first number is treated as a special case where its binary representation is converted using `decimal_to_binary`.
 * Subsequent numbers are converted using `convert_to_binary`. All binary representations are concatenated and appended 
 * with the value of `ARE`. The final binary string is then copied back into the original string `str`.
 *
 * @param length The length of the binary representation for each number.
 * @param str A comma-separated string of numbers. The string is modified to contain the final binary representation.
 */

/**
 * @brief Converts the first number in a comma-separated string to binary and appends it with the remaining numbers.
 *
 * This function takes a comma-separated string of numbers and converts each number to its binary representation. 
 * The first number is treated as a special case where its binary representation is converted using `decimal_to_binary`.
 * Subsequent numbers are converted using `convert_to_binary`. All binary representations are concatenated and appended 
 * with the value of `ARE`. The final binary string is then copied back into the original string `str`.
 *
 * @param length The length of the binary representation for each number.
 * @param str A comma-separated string of numbers. The string is modified to contain the final binary representation.
 * @param ARE A string to be appended at the end of the final binary representation.
 */
void convert_first_word_to_binary(int length, char* str, char* ARE);

void print_memory(instruction_memory *instruction_head, data_image *data_head, int IC, int DC, FILE *fp);


void copyAndConvertFile(char *sourceFile, char *destinationFile);
unsigned int binaryToOctal(const char *binary);