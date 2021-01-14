#ifndef DEFLATE_LIB_LZ78_ENCODING_UTILS_UTILS_H_
#define DEFLATE_LIB_LZ78_ENCODING_UTILS_UTILS_H_

#include <string.h>

#include "../../../utils/utils.h"
#include "../../../utils/map/map.h"
#include "../../../utils/hash_map/hashMap.h"
#include "../../../utils/bit_stream/bitStream.h"

typedef struct evaluate {
  unsigned int numOfBits;
  unsigned int maxIndexSize;
} Evaluate;

typedef enum { DYNAMIC_HUFFMAN, STATIC_HUFFMAN } huffman_type;

void writeCanonicalHuffman(unsigned int maxIndexSize, HashMap *huffman_table);
void writeCode(char *header, Map *data);
void writeNotCompressedBlock(char *header, Data *data);
Evaluate *evaluate(Map *code, huffman_type type);
Map *LZ78Encoding(Data *data, HashMap *huffmanTable);

#endif
