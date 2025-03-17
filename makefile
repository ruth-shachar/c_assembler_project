# Compiler and flags
CC = gcc
CFLAGS = -ansi -Wall -pedantic -g

# Source files
SRC = assembler.c appendix.c pre_assembler.c pre_assembler_help.c scanner.c first_pass.c handle.c first_pass_help.c second_pass.c second_pass_help.c

# Object files
OBJ = $(SRC:.c=.o)

# Executable name
TARGET = assembler

# Default rule
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile individual source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -rf *.o $(TARGET) *.am *.ob *.ent *.ext

.PHONY: clean
