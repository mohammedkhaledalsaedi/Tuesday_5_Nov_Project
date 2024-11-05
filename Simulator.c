#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 100
#define INSTRUCTION_WIDTH 8

//Instruction memory and PC
unsigned char instruction_memory[MEMORY_SIZE];
unsigned char program_counter = 0;

// Registers
unsigned char Register_A = 0;
unsigned char Register_B = 0;
unsigned char Register_O = 0;

// converting a binary string to an unsigned char
unsigned char binary_to_byte(const char *binary) {
    unsigned char result = 0;
    for (int i = 0; i < INSTRUCTION_WIDTH; i++) {
        result = (result << 1) | (binary[i] - '0');
    }
    return result;
}


void load_file_to_memory(const char *filename, int *lines_loaded) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error! Could not open file.\n");
        exit(1);
    }

    char line[INSTRUCTION_WIDTH + 2];
    *lines_loaded = 0;

    while (fgets(line, sizeof(line), file) != NULL && *lines_loaded < MEMORY_SIZE) {
        line[strcspn(line, "\n")] = '\0';
        instruction_memory[*lines_loaded] = binary_to_byte(line);
        (*lines_loaded)++;
    }

    fclose(file);
}

// 2-to-4 decoder function
void decoder_2to4(int D1, int D0) {
    if (D1 == 0 && D0 == 0) {
        printf("Register_A is enabled.\n");
    } else if (D1 == 0 && D0 == 1) {
        printf("Register_B is enabled.\n");
    } else if (D1 == 1 && D0 == 0) {
        printf("Register_O is enabled.\n");
    } else {
        printf("No register enabled (D1 = 1, D0 = 1).\n");
    }
}

// Decode D1 and D0 bits
void fetch_and_decode() {
    
    unsigned char instruction = instruction_memory[program_counter];
    
    // third and fourth bits from the left
    int D1 = (instruction >> 5) & 1; 
    int D0 = (instruction >> 4) & 1; 

   
    decoder_2to4(D1, D0);
}

int main() {
    int lines_loaded = 0;
    load_file_to_memory("fibonacci.bin", &lines_loaded);

    
    printf("Program Execution:\n");
    for (int i = 0; i < lines_loaded; i++) {
        program_counter = i; 
        printf("PC: %02d, Instruction: ", program_counter);
        for (int j = INSTRUCTION_WIDTH - 1; j >= 0; j--) {
            printf("%d", (instruction_memory[i] >> j) & 1); 
        }
        printf("\n");

       
        fetch_and_decode();
    }

    return 0;
}

