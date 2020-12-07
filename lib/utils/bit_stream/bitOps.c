#include "bitOps.h"

// Set the i-th bit to 1
void setBit(uc *byte, uc bit_number) {
  (*byte) |= 1 << bit_number;
}

// Set the i-th bit to 0
void clearBit(uc *byte, uc bit_number) {
  (*byte) &= ~(1 << bit_number);
}

// Complement i-th bit
void toggleBit(uc *byte, uc bit_number) {
  (*byte) ^= 1 << bit_number;
}

// Get the status of i-th bit, returns 0 or 1
uc bitStatus(uc *byte, uc bit_number) {
  return ((*byte & 1 << bit_number) > 0 ? 1 : 0);
}