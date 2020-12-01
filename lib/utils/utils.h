#ifndef __UTILS__
#define __UTILS__

// Struttura dati
typedef struct data {
  char *ptr;
  long size;
} Data;

// Struttura albero Huffman
typedef struct node {
  struct node *parent;
  char *value;
  float weight;
  struct node *left;  // add 0
  struct node *right; // add 1
} Node;

Node *createNode(void *value, float weight);
char *concat(const char *str1, const char *str2);

#endif
