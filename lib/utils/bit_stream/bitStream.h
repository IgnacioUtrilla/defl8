#ifndef __BIT_STREAM__
#define __BIT_STREAM__

#include "bitOps.h"
#include "../utils.h"

typedef enum { ST_OPEN, ST_CLOSED, ST_ERROR, ST_OK, ST_EOS, ST_EOF } stream_status;
typedef enum { READ, WRITE } stream_mode;

// open a bit stream 
stream_status openStream(char *filename, stream_mode mode);

// close a bit stream 
stream_status closeStream(stream_mode mode, int write_buffer);

// read one bit from stream
stream_status readBit(uc *bit);

// write one bit to stream
stream_status writeBit(uc bit);

char *defineEncodeString(uc *bit, int dim);

int isNextEnd(uc *bit, int size, char *equal);
stream_status readBlock(size_t size, Data *data);

void writeBlock(Data *data);

int isEOF();

void writeBlock(Data *data);

void writeStringOfBitsIntoFile(char *data);

#endif