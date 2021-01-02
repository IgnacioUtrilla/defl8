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

Test(LZ78, INT2BIN) {
  char *dest = (char *) malloc(sizeof(char) * 13); // bit massimi 12 + 1 (\0)
  int2bin(2, 4, dest);
  cr_expect_str_eq(dest, "0010");

  strcpy(dest, "");
  int2bin(0, 4, dest);
  cr_expect_str_eq(dest, "0000");

  strcpy(dest, "");
  int2bin(4, 4, dest);
  cr_expect_str_eq(dest, "0100");

  strcpy(dest, "");
  int2bin(8, 4, dest);
  cr_expect_str_eq(dest, "1000");

  strcpy(dest, "");
  int2bin(13, 4, dest);
  cr_expect_str_eq(dest, "1101");

  strcpy(dest, "");
  int2bin(128, 8, dest);
  cr_expect_str_eq(dest, "10000000");

  strcpy(dest, "");
  int2bin(255, 8, dest);
  cr_expect_str_eq(dest, "11111111");

  strcpy(dest, "");
  int2bin(4095, 12, dest);
  cr_expect_str_eq(dest, "111111111111");

  free(dest);
}