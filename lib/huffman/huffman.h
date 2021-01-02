#ifndef __HUFFMAN__
#define __HUFFMAN__

#include "dynamic_frequency/dynamicFrequency.h"
#include <stdlib.h>
#include <string.h>
#include "dynamic_frequency/dynamicFrequency.h"
#include "static_frequency/staticFrequency.h"
#include "../utils/utils.h"
#include "../utils/map/map.h"
#include "../utils/hash_map/hashMap.h"
#include "../utils/bit_stream/bitOps.h"

typedef struct {
  int character;
  int length;
} CanonicalValue;

char *getStrEncode(Node *root, char c);
Node *createHuffmanTree(Map *freqMap);
HashMap *getHuffmanTable(Data *data);

#endif