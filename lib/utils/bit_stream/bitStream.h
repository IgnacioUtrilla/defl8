#ifndef __BIT_STREAM__
#define __BIT_STREAM__

#include "bitOps.h"

typedef enum { ST_OPEN, ST_CLOSED, ST_ERROR, ST_OK, ST_EOS } stream_status;
typedef enum { READ, WRITE } stream_mode;

// open a bit stream 
stream_status openStream(char *filename, stream_mode mode);

// close a bit stream 
stream_status closeStream(stream_mode mode);

// read one bit from stream
stream_status readBit(uc *bit);

// write one bit to stream
stream_status writeBit(uc bit);

stream_status readBlock(size_t size, Data *data);

int isEOF();

#endif