#include "utils.h"
#include <stdlib.h>
#include <string.h>

Node *createNode(void *value, float weight) {
  Node *node = (Node *) malloc(sizeof(Node));
  node->right = NULL;
  node->left = NULL;
  node->value = value;
  node->weight = weight;
  return node;
}

char *concat(const char *str1, const char *str2) {
  char *result = (char *) malloc(strlen(str1) + strlen(str2) + 1); // +1 for \0
  strcpy(result, str1);
  strcat(result, str2);
  return result;
}

/**
 * Function to convert an int to a binary string of n bits
 *
 * @param {Integer} number - number to convert
 * @param {Integer} number_of_bits
 * @param {Char pointer (string)} dest - where to save
 */
void int2bin(unsigned int number, int number_of_bits, char *dest) {
  char *inverseBinaryNumber = (char *) malloc(number_of_bits + 1);
  for (unsigned int i = 0; i < number_of_bits; i++) {
    unsigned int temp = number >> i;
    strcat(inverseBinaryNumber, (temp & 1) ? "1" : "0");
  }

  for (int i = number_of_bits - 1; i >= 0; i--) {
    char str[2] = {inverseBinaryNumber[i], '\0'};
    strcat(dest, str);
  }

  free(inverseBinaryNumber);
}

int bin2int(char *str) {
  return (int) strtol(str, NULL, 2);
}