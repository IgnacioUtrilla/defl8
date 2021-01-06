#include <stdlib.h>
#include <string.h>
#include "huffman.h"

Element findDeleteMin(Map *freqMap) {
  Element min = findMin(freqMap, NULL);
  freqMap->delete(freqMap, min.key);
  return min;
}

/**
 * Funzione per controllare se una determinata chiave ha un nodo rispettivo
 * In caso negativo crea un nuovo nodo
 *
 * @param {Map pointer} freqMap
 * @param {Map pointer} nodeMap
 * @param {Element pointer} min
 * @param {Pointer Node pointer} node
 */
void checkIfNodeExists(Map *freqMap, Map *nodeMap, Element *min, Node **node) {
  (*min) = findDeleteMin(freqMap);
  (*node) = nodeMap->get(nodeMap, min->key);
  if ((*node) == NULL) {
    (*node) = createNode(min->key, *(float *) min->value);
  } else {
    nodeMap->delete(nodeMap, (*node)->value);
  }
}

/**
 *
 * This function returns the encoded string (huffman table)
 *
 * @param {Node pointer} root - huffman tree root
 * @param {Char pointer} c - the string to encoded
 * @return {Char pointer} encoded string
 */
char *getStrEncode(Node *root, char *c) {
  static Node *currentNode = NULL;
  static char *res = "";

  currentNode = root;

  if (currentNode->left && strstr(currentNode->left->value, c) != NULL) {
    currentNode = currentNode->left;
    res = concat(res, "0");
  }

  if (currentNode->right && strstr(currentNode->right->value, c) != NULL) {
    currentNode = currentNode->right;
    res = concat(res, "1");
  }

  if (!currentNode->left && !currentNode->right) {
    char *final = concat(res, "");
    res = "";
    return final;
  }

  return getStrEncode(currentNode, c);
}

Node *createHuffmanTree(Map *freqMap) {
  Map *nodeMap = createMap();
  Element firstMin = findDeleteMin(freqMap);
  if (freqMap->size == 0) {
    Node *only = createNode(firstMin.key, *(float *) firstMin.value);
    return only;
  }
  Element secondMin = findDeleteMin(freqMap);

  Node *firstNode = createNode(firstMin.key, *(float *) firstMin.value);
  Node *secondNode = createNode(secondMin.key, *(float *) secondMin.value);

  Node *newNode =
      createNode(concat(firstMin.key, secondMin.key), *(float *) firstMin.value + *(float *) secondMin.value);

  newNode->left = firstNode;
  newNode->right = secondNode;
  firstNode->parent = newNode;
  secondNode->parent = newNode;

  float *newWeight = (float *) malloc(sizeof(float));
  *newWeight = newNode->weight;
  freqMap->insert(freqMap, newNode->value, newWeight);
  nodeMap->insert(nodeMap, newNode->value, newNode);

  while (freqMap->size > 1) {
    Node *firstNode;
    checkIfNodeExists(freqMap, nodeMap, &firstMin, &firstNode);

    Node *secondNode;
    checkIfNodeExists(freqMap, nodeMap, &secondMin, &secondNode);

    newNode = createNode(concat(firstMin.key, secondMin.key), *(float *) firstMin.value + *(float *) secondMin.value);
    firstNode->parent = newNode;
    secondNode->parent = newNode;
    newNode->left = firstNode;
    newNode->right = secondNode;

    float *newWeight = (float *) malloc(sizeof(float));
    *newWeight = newNode->weight;
    freqMap->insert(freqMap, newNode->value, newWeight);
    nodeMap->insert(nodeMap, newNode->value, newNode);
  }

  Node *ret = (Node *) nodeMap->element->value;
  return ret;
}

/**
 * ONLY FOR CANONICAL HUFFMAN
 */
int sortByCodeWordLength(const void *a, const void *b) {
  CanonicalValue *ca = (CanonicalValue *) a;
  CanonicalValue *cb = (CanonicalValue *) b;

  return ca->length - cb->length;
}

/**
 * ONLY FOR CANONICAL HUFFMAN
 * Sort only if the two values have the same codeword length
 */
int sortByAlphabeticalValue(const void *a, const void *b) {
  CanonicalValue *ca = (CanonicalValue *) a;
  CanonicalValue *cb = (CanonicalValue *) b;

  if (ca->length != cb->length)
    return 0;

  return ca->character - cb->character;
}

HashMap *createCanonicalHuffmanTable(int size, CanonicalValue *initCanonicalArray) {
  HashMap *table = createHashMap();

  // sort by codeword length
  qsort(initCanonicalArray, size, sizeof(CanonicalValue), sortByCodeWordLength);

  // sort by alphabetical value
  qsort(initCanonicalArray, size, sizeof(CanonicalValue), sortByAlphabeticalValue);

  for (int i = 0; i < size; i++) {
    if (!initCanonicalArray[i].character)
      continue;

    char *currentKey = (char *) malloc(sizeof(char) * 2);
    char keyValue[2] = {(char) initCanonicalArray[i].character, '\0'};
    strcpy(currentKey, keyValue);

    int currentLength = initCanonicalArray[i].length;
    char *encodedStr = (char *) malloc(sizeof(char) * (currentLength + 1));

    if (!i) {
      int2bin(0, currentLength, encodedStr);
      table->insert(table, currentKey, encodedStr);
      continue;
    }

    int previousLength = initCanonicalArray[i - 1].length;
    char previusKey[2] = {(char) initCanonicalArray[i - 1].character, '\0'};
    char *previousEncodedStr = (char *) table->get(table, previusKey);

    int newEncodedStr = bin2int(previousEncodedStr);
    newEncodedStr = newEncodedStr + 0x1;
    newEncodedStr = currentLength > previousLength ? newEncodedStr << (currentLength - previousLength) : newEncodedStr;
    int2bin(newEncodedStr, currentLength, encodedStr);

    table->insert(table, currentKey, encodedStr);
  }

  return table;
}

HashMap *getHuffmanTable(Data *data) {
  Map *freqMap = createFreqMap(*data); // TODO: passare un puntatore a data
  Node *tree = createHuffmanTree(freqMap);
  char *rootKey = tree->value; // first value (root)

  int numberOfCharacters = (int) strlen(rootKey);
  CanonicalValue initCanonicalArray[numberOfCharacters];

  for (int i = 0; i < numberOfCharacters; i++) {
    char key[2] = {rootKey[i], '\0'};
    char *encodedStr = getStrEncode(tree, key);

    CanonicalValue *canonicalValue = (CanonicalValue *) malloc(sizeof(CanonicalValue));
    canonicalValue->character = (int) rootKey[i];
    canonicalValue->length = (int) strlen(encodedStr);

    initCanonicalArray[i] = *canonicalValue;
  }

  return createCanonicalHuffmanTable(numberOfCharacters, initCanonicalArray);
}