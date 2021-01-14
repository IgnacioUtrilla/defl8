#ifndef __STATIC_FREQUENCY__
#define __STATIC_FREQUENCY__

#include "../../utils/utils.h"
#include "../../utils/hash_map/hashMap.h"
#include "../../utils/map/map.h"
#include "../huffman.h"

Map getHuffmanStaticFreqMap();
Node *getHuffmanStaticRoot();
HashMap *getHuffmanStaticTable();

#endif
