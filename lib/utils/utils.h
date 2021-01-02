#ifndef __UTILS__
#define __UTILS__

// Struttura dati
typedef struct data {
  char *ptr;
  unsigned long size;
} Data;

// Struttura albero Huffman
typedef struct node {
  struct node *parent;
  char *value;
  float weight;
  int length;
  struct node *left;  // add 0
  struct node *right; // add 1
} Node;

Node *createNode(void *value, float weight, int length);
char *concat(char *str1, char *str2);
void int2bin(unsigned int number, int number_of_bits, char *dest);
int bin2int(char *str);
char *concatCharArrays(char *str1, char *str2, int str1Lenght, int str2Length);
#endif