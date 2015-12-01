 /*
 * decode.h
 * by Russell Parker & Binit Shrestha, 11/23/15
 * UM Assignment 
 *    
 * Summary: This module is designed extract the relavent parts of a UM 
 *          instruction so that the execute module can perform the instruction.
 */

#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "seq.h"
#include "mem.h"
#include "bitpack.h"

uint32_t *decode(uint32_t instruction, uint32_t *instrArr);
void decode_three_registers(uint32_t *instrArr, uint32_t instruction);

#endif
