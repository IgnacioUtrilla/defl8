#include "criterion/criterion.h"
#include <stdio.h>
#include "lz78.c"

Test(LZ78, DECODING) {
Data input;
input.ptr = NULL;
input.size = 0;

Data output;
output.ptr = NULL;
output.size = 0;

// Output array size
size_t size = 0;

char in[] = { 0x61, 0x61, 0x61 };
input.ptr = in;
char out[] = "aaa";
size = sizeof(in) / sizeof(char);

cr_expect_arr_eq(lz78Decoding(input).ptr, out, size, "SHORT MESSAGE");

input.ptr = NULL;
cr_expect_eq(lz78Decoding(input).ptr, output.ptr, "NULL MESSAGE");

char in2[] = { 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61};
//in2 = { 0xD0, 0x74, 0x2D, 0x0F, 0x41 };
input.ptr = in2;
char out2[] = "aaaaaaaaaa";
//cr_expect_arr_eq(lz78Decoding(input).ptr, out2, size, "LONG MESSAGE WITH SAME CHAR");
size = sizeof(in2) / sizeof(char);
cr_expect_arr_eq(lz78Decoding(input).ptr, out2, size, "LONG MESSAGE WITH SAME CHAR");
}
