#ifndef __LZ78_DECODING__
#define __LZ78_DECODING__

#include "../../utils/bit_stream/bitOps.h"
#include "../../utils/hash_map/hashMap.h"
#include "../../utils/utils.h"
#include "../../huffman/static_frequency/staticFrequency.h"
#include "../../utils/bit_stream/bitStream.h"

typedef enum { NOT, FIXED, DYNAMIC, ERROR } encodeType;

void decoding(char *fileName, char *outputFile);
void addDictionaryEntry(Data **dictionaryEntry,
                        char *oldChar,
                        char *newChar,
                        int oldCharLength,
                        int newCharLength);
int decodeLZ78(uc *bit, HashMap *hash_map, int maxSizeEncode);
char *decodeChar(uc *bit, HashMap *hash_map, int maxSizeEncode);

#endif








