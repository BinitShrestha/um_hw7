 /*
 * segmem.c
 * by Russell Parker & Binit Shrestha, 11/23/15
 * UM Assignment 
 *    
 * Summary: The segmem module takes care of the segmented memory of the UM.
 *          The main memory segment is implemented as a Hanson sequence. A
 *          second data structure (also a sequence) is used to keep track of
 *          which segments have been unmapped; when new segments are mapped, 
 *          the module will first attempt to use this sequence as a queue of 
 *          where the segment can be mapped to. 
 */

#include "segmem.h"


/*
 * load: Takes in memory sequence, segment ID, and index, return the word
 *       at the segmented memory location. It is a CRE to pass in a segment ID
 *       that does not map to a current segment (one that has been unmapped or 
 *       never mapped) or to pass in in an index out of the range of the 
 *       segment.
 */
uint32_t load(Seq_T segMem, uint32_t segID, uint32_t index)
{
        struct segment *s = (struct segment *)Seq_get(segMem, segID);

        if (s == NULL) {
                fprintf(stderr, "ERROR: Cannot load from unmapped segment.\n");
                exit(EXIT_FAILURE);
        } else if (index >= s->size) {
                fprintf(stderr, "ERROR: Index outside of segment range.\n");
                exit(EXIT_FAILURE);
        }

        return s->mem[index];
}

/*
 * store: Takes in the memory sequence, segment ID, index, and value to be 
 *        stored. It is a CRE to call the function with a segment ID that is
 *        not current, or an index beyond the range of the segment.
 */
void store(Seq_T segMem, uint32_t segID, uint32_t index, uint32_t val)
{
        struct segment *s = (struct segment *)Seq_get(segMem, segID);

        if (s == NULL) {
                fprintf(stderr, "ERROR: Cannot load from unmapped segment.\n");
                exit(EXIT_FAILURE);
        } else if (index >= s->size) {
                fprintf(stderr, "ERROR: Index outside of segment range.\n");
                exit(EXIT_FAILURE);
        }

        s->mem[index] = val;
}

/*
 * deleteSeg: Takes in the memory segment sequence, queue of deleted memory,
 *            and the segment ID to be deleted. Frees memory associated with
 *            the segment, and updates its value to NULL in the memory segment
 *            sequence (does not use remove to preserve ordering). Finally, 
 *            adds the segment ID to the deleted memory queue. 
 */
void deleteSeg(Seq_T segMem, Seq_T delSeg, uint32_t segID)
{
        struct segment *s = (struct segment *)Seq_get(segMem, segID);

        if (s == NULL) {
                fprintf(stderr, "ERROR: Segment already unmapped.\n");
                exit(EXIT_FAILURE);
        }

        FREE(s->mem);
        FREE(s);

        /* update segment in sequence to NULL to indicate it is unmapped */
        Seq_put(segMem, segID, NULL);
        /* add segment ID to queue of deleted memory segments */
        Seq_addhi(delSeg, (void *)(uintptr_t)segID);
}

/*
 * createSeg: Takes in the segmented memory sequence, deleted memory queue, 
 *            and the size (in 32 bit words) of the new memory segment. 
 *            Allocates memory for the new segment. First checks if there are 
 *            any deleted memory segments that have not been re-mapped; if so,
 *            the function takes a segment ID from the queue. Finally, the
 *            function adds the new segment to the segmented memory sequence, 
 *            either at the segment ID from the queue, or else at the top of
 *            the sequence if the queue was empty.
 */
int createSeg(Seq_T segMem, Seq_T delSeg, unsigned int size) 
{
        struct segment *s = (struct segment *)ALLOC(sizeof(*s));
        s->mem = (uint32_t *)ALLOC(size * BYTES_PER_WORD);
        s->size = size;
        unsigned i;
        for (i = 0; i < s->size; i++) {
                s->mem[i] = 0;
        }

        if (Seq_length(delSeg) != 0) {
                uint32_t segID = (uint32_t)(uintptr_t)Seq_remlo(delSeg);
                Seq_put(segMem, segID, s);
                return segID;
        } else {
                Seq_addhi(segMem, s); 
                return Seq_length(segMem) - 1;
        }
}

/*
 * replaceSeg:
 */
void replaceSeg(Seq_T segMem, uint32_t oldSeg, uint32_t newSeg)
{
        if (oldSeg == newSeg) {
                return;
        }

        struct segment *s, *dupliP;
        unsigned int i;

        s = (struct segment *)Seq_get(segMem, oldSeg);
        free(s->mem);
        free(s);

        s = (struct segment *)Seq_get(segMem, newSeg);

        dupliP = (struct segment *)ALLOC(sizeof(*dupliP));
        dupliP->size = s->size;
        dupliP->mem = (uint32_t *)ALLOC(dupliP->size * BYTES_PER_WORD);
        for (i = 0; i < dupliP->size; i++) {
                dupliP->mem[i] = s->mem[i];
        }

        Seq_put(segMem, oldSeg, dupliP);
}
