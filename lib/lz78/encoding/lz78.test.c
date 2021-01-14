#include "criterion/criterion.h"
#include <stdio.h>
#include <math.h>

#include "lz78.c"
#include "./utils/utils.c"
#include "../../utils/bit_stream/bitStream.c"
#include "../../utils/bit_stream/bitOps.c"
#include "../../utils/map/map.c"
#include "../../utils/hash_map/hashMap.c"
#include "../../utils/utils.c"
#include "../../file_manager/filemanager.c"
#include "../../huffman/huffman.c"
#include "../../huffman/static_frequency/staticFrequency.c"
#include "../../huffman/dynamic_frequency/dynamicFrequency.c"

//Test(LZ78, ENCODING) {
//  Data input;
//  input.ptr = NULL;
//  input.size = 0;
//
//  Data output;
//  output.ptr = NULL;
//  output.size = 0;
//
//  // Output array size
//  size_t size = 0;
//
//  input.ptr = "aaa";
//  char out[] = {0x61, 0x61, 0x61};
//  size = sizeof(out) / sizeof(char);
//  cr_expect_arr_eq(lz78(input).ptr, out, size, "SHORT MESSAGE");
//
//  input.ptr = NULL;
//  cr_expect_eq(lz78(input).ptr, output.ptr, "NULL MESSAGE");
//
//  input.ptr = "aaaaaaaaaa";
//  char out2[] = {0xD0, 0x74, 0x2D, 0x0F, 0x41};
//  size = sizeof(out2) / sizeof(char);
//  cr_expect_arr_eq(lz78(input).ptr, out2, size, "LONG MESSAGE WITH SAME CHAR");
//}
