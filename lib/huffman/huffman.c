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

HashMap *createHuffmanTable(Node *treeRoot) {

}