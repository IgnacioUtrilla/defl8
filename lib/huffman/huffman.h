#ifndef __HUFFMAN__
#define __HUFFMAN__

#include "static_frequency/staticFrequency.h"
#include "../utils/utils.h"
#include "../utils/map/map.h"
#include "../utils/hash_map/hashMap.h"

char *getStrEncode(Node *root, char *c);
Node *createHuffmanTree(Map *freqMap);
HashMap *createHuffmanTable(Node *treeRoot);

#endif