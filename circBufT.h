#ifndef CIRCBUFT_H_
#define CIRCBUFT_H_

// *******************************************************
// 
// circBufT.h
//
// P.J. Bones UCECE, modified by Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 21 May 2021
// 
// *******************************************************

//*****************************************************************************
// Includes
//*****************************************************************************
#include <stdint.h>

// *******************************************************
// Buffer structure
typedef struct {
	uint32_t size;		// Number of entries in buffer
	uint32_t windex;	// index for writing, mod(size)
	uint32_t rindex;	// index for reading, mod(size)
	uint32_t *data;		// pointer to the data
} circBuf_t;

// *******************************************************

//*****************************************************************************
// Function declarations
//*****************************************************************************
uint32_t *initCircBuf (circBuf_t *buffer, uint32_t size);
void writeCircBuf (circBuf_t *buffer, uint32_t entry);
uint32_t readCircBuf (circBuf_t *buffer);
void freeCircBuf (circBuf_t *buffer);

#endif /*CIRCBUFT_H_*/
