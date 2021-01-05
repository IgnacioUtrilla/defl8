#include "utils.h"

/**
 * LZ78 Algorithm
 *
 * @param {Data struct pointer} data - { char pointer, size }
 * @param {Hashmap pointer} huffmanTable
 * @return {Map pointer} (malloc created with constructor) - code generated
 */
Map *encoding(Data *data, HashMap *huffmanTable) {
  char *P = NULL;
  HashMap *dictionary = createHashMap();
  Map *code = createMap();

  for (int index = 0; index < data->size; index++) {
    int C = (uc) data->ptr[index];
    char strC[2] = {C, '\0'};
    char *PC = concat(P == NULL ? "" : P, strC); // FIXME: eseguire il free della memoria

    if (dictionary->get(dictionary, PC) != 0) {
      P = (char *) PC;
    } else {
      void *encodedStr = huffmanTable->get(huffmanTable, strC);
      int pIndex = P == NULL ? 0 : *(int *) dictionary->get(dictionary, P);
      code->insert(code, pIndex, encodedStr);

      int *newIndex = (int *) malloc(sizeof(int)); // FIXME: eseguire il free della memoria
      *newIndex = dictionary->size + 1;
      dictionary->insert(dictionary, PC, newIndex);

      P = NULL;
    }
  }

  // TODO: duplicated code
  if (P != NULL) {
    int pIndex = *(int *) dictionary->get(dictionary, P);
    code->insert(code, pIndex, NULL);
  }

  return code;
}

/**
 * Get the number of bits that occupy the LZ78 code depending on the huffman table
 *
 * @param {Map pointer} code - element [index, encoded string]
 * @return number of bits
 */
Evaluate *evaluate(Map *code, huffman_type type) {
  Evaluate *eval = (Evaluate *) malloc(sizeof(Evaluate));
  Element *element = code->element;
  unsigned int numOfBits = 5; // Initial 3 bits = header + 2 bits of end-of-compressed-block (11)
  unsigned int maxIndexSize = 0;

  do {
    unsigned int index = element->key;
    char *encodedString = element->value;

    unsigned int indexSize = index < 16 ? 4 : index < 256 ? 8 : 12;
    maxIndexSize = maxIndexSize < indexSize ? indexSize : maxIndexSize;

    numOfBits += indexSize;
    numOfBits += encodedString ? strlen(encodedString) : 8; // 8 == possibile ultimo risultato di lz78 (NULL)
  } while ((element = element->next) != NULL);

  if (type == DYNAMIC_HUFFMAN) {
    numOfBits += 8 + maxIndexSize * 256; // length size byte + 256 chars * max length
  }

  eval->numOfBits = numOfBits;
  eval->maxIndexSize = maxIndexSize;
  return eval;
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

/**
 * Encode index
 *
 * @param {Unsigned int} index
 * @return {Char pointer} (malloc) - number of bits + binary number
 */
char *getStringOfBitsForIndex(unsigned int index) {
  int indexSize = index < 16 ? 4 : index < 256 ? 8 : 12;
  char *res = (char *) malloc(indexSize + 3); // +3 = '\0' + 2 bits of index size
  strcpy(res, "");

  switch (indexSize) {
    case 4:strcat(res, "00");
      break;
    case 8:strcat(res, "01");
      break;
    case 12:strcat(res, "10");
      break;
    default:break;
  }

  int2bin(index, indexSize, res);
  return res;
}

/**
 * Write LZ78 code result into the output file
 *
 * @param {Char pointer} header - block header
 * @param {Map pointer} data - LZ78 result
 */
void writeCode(char *header, Map *data) {
  writeStringOfBitsIntoFile(header);

  Element *code = data->element;
  while (code != NULL) {
    unsigned int index = code->key;
    char *encodedIndex = getStringOfBitsForIndex(index);
    char *encoded = code->value;

    writeStringOfBitsIntoFile(encodedIndex);

    if (encoded) {
      writeStringOfBitsIntoFile(encoded);
    } else {
      // Si entra in questa situazione quando non si ha un prossimo carattere
      writeStringOfBitsIntoFile("00000000");
    }

    free(encodedIndex);
    code = code->next;
  }

  // end-of-compressed-block code
  writeStringOfBitsIntoFile("11");
}
