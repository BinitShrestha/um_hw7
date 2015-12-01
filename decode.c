 /*
 * decode.c
 * by Russell Parker & Binit Shrestha, 11/23/15
 * UM Assignment 
 *    
 * Summary: 
 */

#include "decode.h"


/*
 * decode: the lone function in this module takes in a UM instruction as a
 *         32 bit word, extracts the relevent parts, and returns them. It
 *         returns an array of 32 bit words -- each word in the array contains
 *         an instruction (always the first element), an indicator of a 
 *         register, or a value.
 */
uint32_t *decode(uint32_t instruction, uint32_t *instrArr)
{
        uint32_t opcode = Bitpack_getu(instruction, 4, 28);
        //uint32_t *instrArr = (uint32_t *)ALLOC(sizeof(uint32_t) * 4);
        instrArr[0] = opcode;

    switch ((int)opcode) {
        case 0: /* case of three registers of equal length */
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            decode_three_registers(instrArr, instruction);
                break;
        case 8: /* case of two registers of equal length */
        case 12:
            decode_three_registers(instrArr, instruction);
            break; 
        case 9: /* the case of one register */
        case 10:
        case 11:
            decode_three_registers(instrArr, instruction);
            break;
        case 7: /* case of no registers */
            break;
        case 13: /* special case of load_val */
            instrArr[1] = Bitpack_getu(instruction, 3, 25);
            instrArr[2] = Bitpack_getu(instruction, 25, 0); 
            break;
        }
        return instrArr;
}

void decode_three_registers(uint32_t *instrArr, uint32_t instruction) 
{
            /* TODO: assert that leading bits are 0 */
            instrArr[1] = Bitpack_getu(instruction, 3, 6);
            instrArr[2] = Bitpack_getu(instruction, 3, 3);
            instrArr[3] = Bitpack_getu(instruction, 3, 0);
}
