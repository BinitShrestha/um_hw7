 /*
 * execute.c
 * by Russell Parker & Binit Shrestha, 11/23/15
 * UM Assignment 
 *    
 * Summary: This file implments the interface detailed in execute.h. The 
 *          purpose of the execute module is to take in a decoded instruction
 *          (as a uint32_t array) and properly execute the isntruction. The 
 *          main function, execute, also takes in the data structures 
 *          associated with memory, the registers, and the program counter. 
 *          For each operation, it uses a helper function to accomplish the 
 *          task.
 */

#include "execute.h"
#define REG(boxed, i) registers[(unsigned int)boxed[i]]

/*
 * execute: Takes in the decoded instruction as a uint32_t array, the UM 
 *          registers, the segmented memory sequence, the deleted memory
 *          queue, and finally the program counter. The program executes
 *          the instruction, and returns the updated program counter (which
 *          will be incremented in every case except the load program opcode).
 */
unsigned execute(uint32_t arr[], uint32_t registers[], Seq_T segMem, 
    Seq_T delMem, unsigned pc) 
{
        pc++;
        uint32_t opcode = arr[0];

        switch (opcode) {
                case 0:
                        cond_move(&REG(arr, 3), &REG(arr, 1), &REG(arr, 2));
                        break;
                case 1:
                        seg_load(segMem, &REG(arr, 1), &REG(arr, 2), 
                                &REG(arr, 3));
                        break;
                case 2:
                        seg_store(segMem, &REG(arr, 1), &REG(arr, 2), 
                            &REG(arr, 3));
                        break;
                case 3:
                        add(&REG(arr, 2), &REG(arr, 3), &REG(arr, 1));
                        break;
                case 4:
                        multiply(&REG(arr, 1), &REG(arr, 2), &REG(arr, 3));
                        break;
                case 5:
                        divide(&REG(arr, 1), &REG(arr, 2), &REG(arr, 3));
                        break;
                case 6:
                        nand(&REG(arr, 2), &REG(arr, 3), &REG(arr, 1));
                        break;
                case 7:
                        halt(segMem, delMem, arr);
                        break;
                case 8:
                        map_seg(segMem, delMem, &REG(arr, 2), &REG(arr, 3));
                        break;
                case 9:
                        unmap_seg(segMem, delMem, &REG(arr, 3));
                        break;
                case 10:
                        output(&REG(arr, 3));
                        break;
                case 11:
                        input(&REG(arr, 3));
                        break;
                case 12:
                        load_program(segMem, REG(arr, 2));
                        pc = REG(arr, 3);
                        break;
                case 13:
                        load_val(&REG(arr, 1), arr[2]);
                        break;
                default: 
                        fprintf(stderr, "ERROR: Invalid opcode: %d.", opcode);
                        exit(EXIT_FAILURE);
                        break;
        }
        //free(arr);
        return pc;
}


/*
 * add: Adds two registers and puts value into another.
 */
void add(uint32_t *val1, uint32_t *val2, uint32_t *result)
{
        *result = *val1 + *val2;
}

/*
 * multiply: Multiply two registers and puts value into another.
 */
void multiply(uint32_t *val1, uint32_t *val2, uint32_t *result)
{
        *val1 = *result * *val2;
}

/*
 * divide: Divide two registers and puts value into another.
 */
void divide(uint32_t *result, uint32_t *numerator, uint32_t *denominator)
{
        if (*denominator == 0) {
                fprintf(stderr, "ERROR: cannot divide by 0.");
                exit(EXIT_FAILURE);
        }
        *result = *numerator / *denominator;
}

/*
 * cond_move: If the check register is not equal to zero, the destination
 *            register recieves the value register.
 */
void cond_move(uint32_t *check, uint32_t *dest, uint32_t *val)
{
        if ((int)*check != 0) {
                *dest = *val;
        }
}

/*
 * nand: Bitwise NAND two registers and puts value into another.
 */
void nand(uint32_t *val1, uint32_t *val2, uint32_t *result) 
{
        *result = ~(*val1 & *val2);
}

/*
 * output: Prints out a designated register out to terminal.
 */
void output(uint32_t *data)
{
        if (*data > 255) {
                fprintf(stderr, 
                "ERROR: cannot print value beyond Extended ASCII: %u.\n", 
                (unsigned)*data);
                exit(EXIT_FAILURE);
        }
        printf("%c", *data);
}

/*
 * input: Waits for client input and places into designated register.
 */
void input(uint32_t *val)
{
        int newNum = fgetc(stdin);
        if (newNum != EOF) {
                *val = newNum;
        } else {
                *val = ~0;        
        }
}

/*
 * load_val: Loads a value up to 25 bits into a designated register.
 */
void load_val(uint32_t *reg, int val)
{
        *reg = val;
}

/*
 * seg_load: Loads a register with a designated segment in memory.
 */
void seg_load(Seq_T segMem, uint32_t *reg, uint32_t *seg, uint32_t *loc)
{
        *reg = load(segMem, *seg, *loc);
}

/*
 * seg_store: Stores a value into a designated segment in memory.
 */
void seg_store(Seq_T segMem, uint32_t *seg, uint32_t *loc, uint32_t *val)
{
        store(segMem, *seg, *loc, *val);
}

/*
 * map_seg: Creates a new segement that is unused in memory of designated size.
 */
void map_seg(Seq_T segMem, Seq_T delMem, uint32_t *seg_num, uint32_t *size)
{
       *seg_num = createSeg(segMem, delMem, *size);
}

/*
 * unmap_seg: Deletes memory in a designated segment in memory.
 */
void unmap_seg(Seq_T segMem, Seq_T delMem, uint32_t *seg) 
{
       deleteSeg(segMem, delMem, *seg); 
}
/*Segment $m[$r[B]] is duplicated, and the
duplicate replaces $m[0], which is abandoned.
The program counter is set to point to
$m[0][$r[C]]. If $r[B] = 0, the load-program*/

/*
 * load_program: Duplicates a designated segment and then replaces
 *               whatever is in segment is 0 with it.
 */
void load_program(Seq_T segMem, uint32_t newSeg)
{
        replaceSeg(segMem, 0, newSeg);
}

/*
 * halt: Computation stops and deletes all memory that is allocated.
 */
void halt(Seq_T segMem, Seq_T delMem, uint32_t *arr)
{
        while (Seq_length(segMem) != 0) {
                struct segment *s = Seq_remlo(segMem);
                if (s != NULL) {
                        free(s->mem);
                        free(s);
                }
        }
        Seq_free(&segMem);
        Seq_free(&delMem);
        free(arr);
        exit(EXIT_SUCCESS);
}

#undef REG
