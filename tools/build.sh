#!/bin/bash

LIBS="
  ../main.c
  ../lib/utils/bit_stream/bitOps.c
  ../lib/utils/bit_stream/bitStream.c
  ../lib/utils/utils.c
  ../lib/utils/ui/ui-functions.c
  ../lib/utils/hash_map/hashMap.c
  ../lib/utils/map/map.c
  ../lib/huffman/static_frequency/staticFrequency.c
  ../lib/huffman/dynamic_frequency/dynamicFrequency.c
  ../lib/huffman/huffman.c
  ../lib/file_manager/filemanager.c
  ../lib/lz78/encoding/lz78.c
  ../lib/lz78/decoding/lz78.c
  ../lib/lz78/encoding/utils/utils.c
"

gcc -o "../def8" $LIBS -lm