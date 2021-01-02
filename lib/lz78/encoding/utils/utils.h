#ifndef DEFLATE_LIB_LZ78_ENCODING_UTILS_UTILS_H_
#define DEFLATE_LIB_LZ78_ENCODING_UTILS_UTILS_H_

#include <string.h>

#include "../../../utils/utils.h"
#include "../../../utils/map/map.h"
#include "../../../utils/hash_map/hashMap.h"
#include "../../../utils/bit_stream/bitStream.h"
#include "../../../file_manager/filemanager.h"

typedef enum { DYNAMIC_HUFFMAN, STATIC_HUFFMAN } huffman_type;

void writeCode(char *header, Map *data);
unsigned int evaluate(Map *code, huffman_type type);
void writeStringOfBitsIntoFile(char *data);

#endif
