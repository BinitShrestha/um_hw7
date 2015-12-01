 /*
 * fetch.h
 * by Russell Parker & Binit Shrestha, 11/23/15
 * UM Assignment 
 *    
 * Summary: This file outlines the fetch module, which consists of only a
 *          single function which takes care of fetching the next UM
 *          instruction.
 */

#ifndef FETCH_H
#define FETCH_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "seq.h"
#include "mem.h"
#include "segmem.h"

uint32_t fetch(Seq_T segMem, uint32_t pc);

#endif
