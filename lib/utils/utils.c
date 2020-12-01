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