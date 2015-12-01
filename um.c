 /*
 * um.c
 * by Russell Parker & Binit Shrestha, 11/23/15
 * UM Assignment 
 *    
 * Summary: This file contains the program's main. It initializes the data
 *          structures needed in the UM, and starts the UM in an infinite loop
 *          which runs until a HALT instruciton is reached. The main modules
 *          that run within the loop are fetch, decode, and execute. 
 */

#include "seq.h"
#include "execute.h"
#include "mem.h"
#include "fetch.h"
#include "decode.h"
#include "segmem.h"
#include <stdlib.h>
#include <sys/stat.h>


struct segment *readCode(const char *filePath);
size_t fileLength(const char *filePath);
uint32_t *loadInstructions(FILE *f, size_t words);


int main(int argc, char* argv[])
{
        if (argc != 2) {
                fprintf(stderr, "ERROR: Bad number of arguments.");
                exit(EXIT_FAILURE);
        }

        /* load code from specified file, and initialize segmented memory with
           the code as segment 0 */
        struct segment *code = readCode(argv[1]);
        Seq_T segMem = Seq_seq(code);

        /* initialize deleted memory queue and initialize all registers to 0 */
        Seq_T delMem = Seq_new(0); 
        uint32_t registers[8] = { 0 }; 

        uint32_t pc = 0;
        uint32_t *instrArr = (uint32_t *)ALLOC(sizeof(uint32_t) * 4);

        /* loop through machine cycle, until halt instruction or error */
        while (1) { 
               uint32_t instruction = fetch(segMem, pc);
               uint32_t *decoded = decode(instruction, instrArr);
               pc = execute(decoded, registers, segMem, delMem, pc);
        }
}

/*
 * fileLength: Returns the length of a file given a char pointer to a 
 *             file path. 
 */
size_t fileLength(const char *filePath) 
{
        struct stat st;
        if (stat(filePath, &st) != 0) {
                return 0;
        }
        return st.st_size;
}

/*
 * readCode: Read in code from a file pointer. Allocates a struct segment 
 *           which contains the instrucitons, and the number of instruction.
 *           A reference to this struct is returned to the client. The function
 *           uses a helper function loadInstructions to copy the provided code
 *           into an allocated array.
 */
struct segment *readCode(const char *filePath)
{
        size_t fileSize = fileLength(filePath);
        if (fileSize == 0) { /* TODO: check if this is necessary */
                fprintf(stderr, "ERROR: Empty program.");
                exit(EXIT_FAILURE);
        }
        FILE *f = fopen(filePath, "rb");
        if (f == NULL) {
                fprintf(stderr, "Problem opening file.");
        }

        uint32_t *code = loadInstructions(f, fileSize / BYTES_PER_WORD);
        struct segment *seg = (struct segment *)ALLOC(sizeof(*seg));
        seg->size = fileSize / BYTES_PER_WORD;
        seg->mem = code;

        fclose(f);
        return seg;
}

/*
 * loadInstructions: This function takes in a file pointer and a number of
 *                   words. It allocates enough memory to hold the provided
 *                   number of words (as a uint32_t array), and fills the array
 *                   with instructions from the file. Finally, the function 
 *                   returns a reference to the allocated array.
 */
uint32_t *loadInstructions(FILE *f, size_t words)
{
        uint32_t *code = (uint32_t *)ALLOC(words * BYTES_PER_WORD);
        uint32_t instr;

        int i, j;
        for (i = 0; i < (int)words; i++) {
                instr = 0;
                /* read in the big-endian instruction one byte at a time */
                for (j = 24; j >= 0; j -= BITS_PER_BYTE) {
                        instr = Bitpack_newu(instr, BITS_PER_BYTE, j, fgetc(f));
                }
                code[i] = instr;
        }
        return code;
}
