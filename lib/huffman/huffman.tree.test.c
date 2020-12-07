#include "criterion/criterion.h"
#include "static_frequency/staticFrequency.c"
#include "../utils/utils.c"
#include "../utils/hash_map/hashMap.c"
#include "../utils/map/map.c"
#include "huffman.c"

Test(HUFFMAN, treeWeigth) {
  Node *root = getHuffmanStaticRoot();
  cr_expect_float_eq(root->weight, 1,0.05);
}

Test(HUFFMAN, rootStringLength) {
  Node *root = getHuffmanStaticRoot();
  cr_expect_eq (strlen(root->value), 26);
}
