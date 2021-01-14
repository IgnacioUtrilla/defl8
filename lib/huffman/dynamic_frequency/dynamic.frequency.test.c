#include "criterion/criterion.h"
#include "../static_frequency/staticFrequency.c"
#include "../huffman.c"
#include "../../utils/utils.c"
#include "../../utils/hash_map/hashMap.c"
#include "../../utils/map/map.c"
#include "dynamicFrequency.c"

Test(FREQUENCYMAP, CREATE_FREQMAP) {
  Data test;
  char *string = "aaff";
  test.ptr = string;
  test.size = 4;
  Map *freqMap = createFreqMap(test);
  cr_expect_eq(freqMap->size, 2);
}

Test(FREQUENCYMAP, CHECK_FRQ_NUMBER) {
  Data test;
  char *string = "aaff";
  test.ptr = string;
  test.size = 4;
  Map *freqMap = createFreqMap(test);
  cr_expect_float_eq(*(float *) freqMap->element->value, 0.5f, 0);
  char *string2 = "asdf";
  test.ptr = string2;
  freqMap = createFreqMap(test);
  cr_expect_float_eq(*(float *) freqMap->element->value, 0.25f, 0);
}
