The project consists of multiple C and header files, each serving a specific purpose in the program. It adheres to principles of clarity, readability, structured writing, and data abstraction.

The assembler works in three main stages:

* Pre-Processing
* First Pass
* Second Pass

These stages form the core algorithm of the assembler.

**Pre-Processing**
The pre-assembler reads the source file line by line and identifies macro definitions. If a macro is identified, the lines of the macro are added to a macro table and replaced in the source file.

**First Pass**
The first pass identifies symbols (labels) and assigns them numerical values ​​that represent their corresponding memory addresses, and also begins to generate the binary code that does not depend on the labels

**Second Pass**
In the second pass, the assembler generates the final machine code, replacing operation names with their binary equivalents and symbol names with their assigned memory locations.
