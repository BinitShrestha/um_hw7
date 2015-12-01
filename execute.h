#ifndef EXECUTE_H
#define EXETUTE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "bitpack.h"
#include "assert.h"
#include "seq.h"
#include "mem.h"
#include "segmem.h"

static const int BITS_PER_BYTE = 8;

unsigned execute(uint32_t arr[], uint32_t registers[], Seq_T segMem, 
    Seq_T delMem, unsigned pc);
void add(uint32_t *val1, uint32_t *val2, uint32_t *result);
void multiply(uint32_t *val1, uint32_t *val2, uint32_t *result);
void divide(uint32_t *val1, uint32_t *val2, uint32_t *result);
void cond_move(uint32_t *check, uint32_t *dest, uint32_t *val);
void nand(uint32_t *val1, uint32_t *val2, uint32_t *result);
void output(uint32_t *data);
void input(uint32_t *val);
void load_val(uint32_t *reg, int val);
void halt(Seq_T segMem, Seq_T delMem, uint32_t *arr);
void seg_load(Seq_T segMem, uint32_t *reg, uint32_t *seg, uint32_t *loc);
void seg_store(Seq_T segMem, uint32_t *seg, uint32_t *loc, uint32_t *val);
void map_seg(Seq_T segMem, Seq_T delMem, uint32_t *seg_num, uint32_t *size);
void unmap_seg(Seq_T segMem, Seq_T delMem, uint32_t *seg);
void divide(uint32_t *result, uint32_t *numerator, uint32_t *denominator);
void load_program(Seq_T segMem, uint32_t newSeg);

#endif
