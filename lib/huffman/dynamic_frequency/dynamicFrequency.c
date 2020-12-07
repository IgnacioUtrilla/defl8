#include <stdlib.h>
#include <string.h>
#include "dynamicFrequency.h"

/**
 * Funzione per contare la quantità di caratteri presenti in una stringa
 *
 * @param {Data} buffer
 * @return {HashMap pointer}
 */
HashMap *countChar(Data buffer) {
  HashMap *list = createHashMap();
  char *ptr = buffer.ptr;
  for (char c = *ptr; c; c = *++ptr) {
    //check if char in hasMap
    char *temp = (char *) malloc(sizeof(char *) * 2);
    char new[2] = {c, '\0'};
    strcpy(temp, new);
    float *value = (float *) getHashMapValue(list, temp);
    if (value) {
      *value = *value + 1;
    } else {
      float *v = (float *) malloc(sizeof(float));
      *v = 1.0f;
      insertHashMapValue(list, temp, v);
    }
  }
  return list;
}

/**
 *
 * Funzione per convertire HashMap conta caratteri in mappa di frequnze per Huffman
 *
 * @param {HashMap pointer} hash_map
 * @return {Map pointer}
 */
Map *convertToFrequencyMap(HashMap *hash_map) {
  Map *freqMap = createMap();
  for (int i = 0; i < MAX_HASHMAP_SIZE_X2; i++) {
    if (hash_map->array[i]) {
      hmNode *node = hash_map->array[i];
      freqMap->insert(freqMap, node->key, (float *) node->value);
    }
  }
  float sum = 0;
  Element *ptr = freqMap->element;
  for (int i = 0; i < freqMap->size; i++) {
    sum += *(float *) ptr->value;
    ptr = ptr->next;
  }
  ptr = freqMap->element;
  for (int i = 0; i < freqMap->size; i++) {
    *(float *) ptr->value = (*(float *) ptr->value) / sum;
    ptr = ptr->next;
  }
  return freqMap;
}

/**
 *
 * Funzione per creare mappa frequenza partendo da struct Data
 *
 * @param {Data} buffer
 * @return {Map pointer}
 */
Map *createFreqMap(Data buffer) {
  Map *freqMap = convertToFrequencyMap(countChar(buffer));
  return freqMap;
}
