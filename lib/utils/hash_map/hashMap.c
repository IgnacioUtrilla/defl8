#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "hashMap.h"

/**
 * hmNode struct definition
 */
typedef struct hmnode {
  void *value;
  char *key;
} hmNode;

/**
 *
 * Hash function to getHashMapValue an index
 *
 * @param {Pointer to char} key
 * @return {Integer} index
 */
static int hash(char *key) {
  const int k = 5; // number of characters per block
  const int m = MAX_HASHMAP_SIZE - 1; // index from 0 to 255
  long long int result = 0;

  int interactions = ceil((double) strlen(key) / k);
  int charIndex = 0;
  while (interactions > 0) {

    for (int i = 0; i < k; i++) {
      if (key[charIndex] == '\0') break;

      result += (long long int) powf(512, k - i) * key[charIndex];
      charIndex++;
    }

    interactions--;
  }

  return (int) (result % m);
};

/**
 *
 * Get an element from hashmap
 *
 * @param {Pointer to Hashmap} hashmap
 * @param {Pointer to char} key
 * @return {Generic pointer} value
 */
void *getHashMapValue(HashMap *hashmap, char *key) {
  int index = hash(key);
  do {
    hmNode *node = hashmap->array[index];
    if (node != NULL && !strcmp(node->key, key))
      return node->value;

    index++;
  } while (MAX_HASHMAP_SIZE_X2 - index - 1 != 0);

  // TODO - handle error
  return 0;
};

/**
 *
 * Delete an element from hashmap
 *
 * @param {Pointer to Hashmap} hashmap
 * @param {Pointer to char} key
 */
int deleteHashMapValue(HashMap *hashmap, char *key) {
  int index = hash(key);
  hmNode *node = hashmap->array[index];

  do {
    if (node != NULL && !strcmp(node->key, key)) {
      hashmap->array[index] = NULL;
      hashmap->size--;
      free(node);
      return 0;
    }

    index++;
  } while (MAX_HASHMAP_SIZE_X2 - index - 1 != 0);

  return 1;
};

/**
 *
 * Handle collisions with linear probing
 *
 * @param {Pointer to Hashmap} hashmap
 * @param {Pointer to char} key
 * @param {Generic pointer} value
 */
int insertHashMapValue(HashMap *hashmap, char *key, void *value) {
  if (hashmap->size == MAX_HASHMAP_SIZE - 1) {
    // TODO gestire errore
    return 1;
  }

  int index = hash(key);

  do {
    hmNode *node = hashmap->array[index];

    // If exist an element with the same key
    if (node != NULL && !strcmp(node->key, key)) {
      return 1;
    }

    if (hashmap->array[index] == NULL) {
      hmNode *node = (hmNode *) malloc(sizeof(hmNode));
      node->key = key;
      node->value = value;

      hashmap->array[index] = node;
      hashmap->size++;
      return 0;
    }

    index++;
  } while (MAX_HASHMAP_SIZE_X2 - index - 1 != 0);

  return 1;
};

/**
 * Hashmap "constructor"
 *
 * @return {Hashmap struct} hashmap
 */
HashMap *createHashMap() {
  HashMap *hashmap = (HashMap *) malloc(sizeof(HashMap));
  hashmap->size = 0;
  hashmap->insert = &insertHashMapValue;
  hashmap->delete = &deleteHashMapValue;
  hashmap->get = &getHashMapValue;

  return hashmap;
}