#ifndef __DYNAMIC_FREQUENCY__
#define __DYNAMIC_FREQUENCY__

#include "../../utils/utils.h"
#include "../../utils/hash_map/hashMap.h"
#include "../../utils/map/map.h"

Map *createFreqMap(Data buffer);
HashMap *countChar(Data buffer);
Map *convertToFrequencyMap(HashMap *hash_map);

#endif