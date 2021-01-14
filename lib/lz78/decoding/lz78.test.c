#include "criterion/criterion.h"
#include <stdio.h>
#include "lz78.c"
#include "../../utils/bit_stream/bitOps.c"
#include "../../utils/bit_stream/bitStream.c"
#include "../../utils/utils.c"
#include "../../utils/hash_map/hashMap.c"
#include "../../utils/map/map.c"
#include "../../huffman/static_frequency/staticFrequency.c"
#include "../../huffman/huffman.c"
#include "../../file_manager/filemanager.c"

//FIXME
Test(LZ78, DECODING
) {
  char *fileName = "/Users/riccardo/supsi/deflate/lib/lz78/decoding/testFile/output.def8";
  lz78Decoding(fileName);
  Data output = readFile("/Users/riccardo/supsi/deflate/lib/lz78/decoding/testFile/ris.txt");
  printf("%s", output.ptr);
  cr_assert_str_eq("h", output.ptr);
}