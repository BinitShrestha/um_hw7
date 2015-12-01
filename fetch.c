 /*
 * fetch.c
 * by Russell Parker & Binit Shrestha, 11/23/15
 * UM Assignment 
 *    
 * Summary: This file implements the fetch module. It takes care of retrieving
 *          the next UM instruction (encoded as a 32 bit word).
 */

#include "fetch.h"

static const int CODE_SEGMENT = 0;

/*
 * fetch: Takes in the segmented memory sequence and the program counter.
 *        The function retrives the instruction word from the code (stored
 *        in segment 0).
 */
uint32_t fetch(Seq_T segMem, uint32_t pc)
{
	return load(segMem, CODE_SEGMENT, pc);
}
