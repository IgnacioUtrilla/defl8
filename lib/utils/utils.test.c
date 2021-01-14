#include "criterion/criterion.h"
#include "./utils.c"

Test(UTILS, INT2BIN) {
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

Test(UTILS, BIN2INT) {
  cr_expect_eq(bin2int("0"), 0);
  cr_expect_eq(bin2int("0100"), 4);
  cr_expect_eq(bin2int("1101"), 13);
  cr_expect_eq(bin2int("10000000"), 128);
  cr_expect_eq(bin2int("11111111"), 255);
  cr_expect_eq(bin2int("111111111111"), 4095);
}