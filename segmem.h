 /*
 * segmem.h
 * by Russell Parker & Binit Shrestha, 11/23/15
 * UM Assignment 
 *    
 * Summary: 
 */

#ifndef SEGMEM_H
#define SEGMEM_H

#include "seq.h"
#include "assert.h"
#include "mem.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

static const int BYTES_PER_WORD = 4;

struct segment {
        uint32_t *mem;
        unsigned int size;
};

uint32_t load(Seq_T segMem, uint32_t segID, uint32_t index);
void store(Seq_T segMem, uint32_t segID, uint32_t index, uint32_t val);
void deleteSeg(Seq_T segMem, Seq_T delMem, uint32_t segID);
int createSeg(Seq_T segMem, Seq_T delMem, unsigned int size); 
void replaceSeg(Seq_T segmem, uint32_t oldSeg, uint32_t newSeg);

#endif
