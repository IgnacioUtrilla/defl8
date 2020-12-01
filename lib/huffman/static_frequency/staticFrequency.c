#include <stdlib.h>
#include <string.h>
#include "staticFrequency.h"

void setValue(Map *map, char *key, float value) {
  float *v = (float *) malloc(sizeof(float));
  *v = value;
  map->insert(map, key, v);
}

/**
 * Restituisce la tabella statica di Huffman (inglese)
 *
 * @link https://en.wikipedia.org/wiki/Letter_frequency
 * @return { Map } primo elemento della tabella statica
 */
Map getHuffmanStaticFreqMap() {
  static Map *map = NULL;
  if (map == NULL) {
    map = createMap();

    setValue(map, "e", 0.13f);
    setValue(map, "t", 0.091f);
    setValue(map, "a", 0.082f);
    setValue(map, "o", 0.075f);
    setValue(map, "i", 0.07f);
    setValue(map, "n", 0.067f);
    setValue(map, "s", 0.063f);
    setValue(map, "h", 0.061f);
    setValue(map, "r", 0.06f);
    setValue(map, "d", 0.043f);
    setValue(map, "l", 0.04f);
    setValue(map, "c", 0.028f);
    setValue(map, "u", 0.028f);
    setValue(map, "m", 0.024f);
    setValue(map, "w", 0.024f);
    setValue(map, "f", 0.022f);
    setValue(map, "g", 0.02f);
    setValue(map, "y", 0.02f);
    setValue(map, "p", 0.019f);
    setValue(map, "b", 0.015f);
    setValue(map, "v", 0.0098f);
    setValue(map, "k", 0.0077f);
    setValue(map, "j", 0.0015f);
    setValue(map, "x", 0.0015f);
    setValue(map, "q", 0.00095f);
    setValue(map, "z", 0.00074f);
  }

  return *map;
}

Node *getHuffmanStaticRoot() {
  static Node *root = NULL;
  if (!root) {
    Map staticHuffmanFreq = getHuffmanStaticFreqMap();
    root = createHuffmanTree(&staticHuffmanFreq);
  }
  return root;
}

HashMap *getHuffmanStaticTable() {
  static HashMap *staticTable = NULL;
  if (!staticTable) {
    staticTable = createHashMap();
    Node *treeRoot = getHuffmanStaticRoot();
    char *rootKey = treeRoot->value;

    for (int i = 0; i < strlen(rootKey); i++) {
      char temp[2] = {rootKey[i], '\0'};
      char *key = (char *) malloc(sizeof(char) * 2);
      key = concat(key, temp);

      char *encodedStr = getStrEncode(treeRoot, key);
      staticTable->insert(staticTable, key, encodedStr);
    }
  }
}