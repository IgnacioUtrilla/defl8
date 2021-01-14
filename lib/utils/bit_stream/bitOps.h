#ifndef __BIT_OPS__
#define __BIT_OPS__
#include <stdio.h>
#include <stdlib.h>

#define B_B_P "%c%c%c%c%c%c%c%c"
#define B2B(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

typedef unsigned char uc;

// Set the i-th bit to 1
void setBit(uc *byte, uc bit_number);

// Set the i-th bit to 0
void clearBit(uc *byte, uc bit_number);

// Complement i-th bit
void toggleBit(uc *byte, uc bit_number);

// Get the status of i-th bit, returns 0 or 1
uc bitStatus(uc *byte, uc bit_number);

#endif