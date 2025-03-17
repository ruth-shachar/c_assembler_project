#ifndef LABRATORY_C_FINAL_PROJECT_GLOBALS_H
#define LABRATORY_C_FINAL_PROJECT_GLOBALS_H

/*The File contain all the global values in the program*/

/* Maximum length of a label in command line  */
#define MAX_LABEL_LENGTH 31

/* Maximum length of a single command line  */
#define MAX_LINE_LENGTH 81
/* Default IC value */
#define IC_INIT_VALUE 100
/* arbitrary very big number for line length */
#define BIG_NUMBER_CONST 1000

#define INSTRUCTIONS_COUNT 4

#define OPCODES_COUNT 16

#define REG_COUNT 8

#define WORD_LEN 16

#define FILED_SIZE 13

/*This struct holds information about the location of a particular piece of code within a source file.*/
typedef struct location {
    char *file_name; /* The name of the source file.*/
    int line_num;    /*The line number within the source file.*/
} location;


/* Define a line struct*/
typedef struct line_data {
    char *file_name;/*The file name will help us to print relevant errors*/
   
    short number;   /*The line number will helps us to track the current line number and to print errors*/
    
    char *data;     /*The content of the line*/
    
    struct line_data * next;/*pointer to the next line*/

} line_data;

/*This struct holds information about a specific opcode used in assembly language instructions*/
typedef struct op_code {
    char *name_of_opcode;    /* The opcode corresponding to the operation */
    int num_of_opcode;/* the number of opcode*/
    int arg_num;     /* The number of arguments for the operation */
    int  target_type[4];/*all the possible values to target type*/
    int source_type[4];/*all the possible values to source type*/
} op_code;

/*This struct is used to define a macro*/
typedef struct node {
    char *macro_name; /*The name of the macro*/
    char *macro_content; /*The content of the macro*/
    int macro_line; /*The line number where the macro was defined.*/
    struct node *next; /*A pointer to the next node in the linked list*/
} node;

/*This struct is used to define a register*/
typedef struct Register{
    char *name_of_register; /*The name of the register*/
    int reg_num; /*The number of the register.*/
}Register;

/*This struct is used to store information about data values*/
typedef struct data_image {
    int address; /*The address where the data value is stored*/
    char* binary_value; /* The binary representation of the data value.*/
    int line; /*The line number in the source file where this entry was processed.*/
    struct data_image *next; /*A pointer to the next entry in the data image linked list.*/
} data_image;

typedef struct label{
    char *name_of_label;  /*The name of the label.*/
    char * type_of_label; /* The type of the label*/
    int address_of_label; /* The address of the label in memory or code. */
    struct label *next;   /* A pointer to the next label in the linked list.*/
}label;

typedef struct type_of_argument{
    char first_char;               /* The first character of the argument type.*/
    int num_of_addressing_method;  /*The number of addressing methods for this argument type*/
}type_of_argument;

typedef struct addressing_method{
    char *name_of_opcode[5]; /*Array of opcode names associated with this addressing method.*/
    int source_operand[4];   /*Array of source operand types supported by this addressing method.*/
    int target_operand[4];   /*Array of target operand types supported by this addressing method.*/
}addressing_method;

typedef struct instruction_memory{
    int address;      /*The memory address where the instruction is located.*/
    int  line;        /*The line number in the source code where the instruction is defined.*/
    char *binary_str; /* A string representing the binary encoding of the instruction.*/
    struct instruction_memory * next; /*Pointer to the next instruction in the linked list.*/
}instruction_memory;

#endif