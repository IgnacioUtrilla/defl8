#include "utils.h"

/**
 * LZ78
 * Search Data value inside an array of Data pointer and return the index
 *
 * This logic handle data like an array of chars and not like a string
 * because the null char could be present
 *
 * @param {Array of Data pointers} source
 * @param {Data pointer} where - information to search
 * @return {int index}
 */
int getIndexBySearchingInDictionary(Data **source, Data *where) {
  for (int i = 0; i < 256; ++i) {
    Data *p_data = source[i];

    if (i != 0 && p_data == NULL) return -1;

    if (p_data != NULL && p_data->size == where->size) {
      for (int j = 0; j < p_data->size; j++) {
        if ((uc) p_data->ptr[j] != (uc) where->ptr[j]) break;
        if (j == p_data->size - 1) return i;
      }
    }
  }

  return -1;
}

/**
 * Free memory from a dictionary entry
 *
 * @param {Pointer to array of Data} dictionary
 * @param {int} index
 */
void freeDictionaryEntry(Data *const *dictionary, int index) {
  if (dictionary[index] != NULL) {
    free(dictionary[index]->ptr);
    free(dictionary[index]);
  }
}

/**
 * LZ78 Algorithm
 *
 * @param {Data struct pointer} data - { char pointer, size }
 * @param {Hashmap pointer} huffmanTable
 * @return {Map pointer} (malloc created with constructor) - code generated
 */
Map *encoding(Data *data, HashMap *huffmanTable) {
  Map *code = createMap();
  Data *dictionary[256] = {0};
  int index = 1;

  Data *strC = (Data *) malloc(sizeof(Data));
  strC->ptr = (char *) malloc(sizeof(char));

  Data *P = (Data *) malloc(sizeof(Data));
  P->ptr = NULL;
  P->size = 0;

  for (int i = 0; i < data->size; i++) {
    strC->ptr[0] = data->ptr[i];
    strC->size = 1;

    Data *PC = (Data *) malloc(sizeof(Data));
    PC->ptr = concatCharArrays(P->ptr, strC->ptr, P->size, strC->size); // FIXME: eseguire il free della memoria
    PC->size = P->size + strC->size;

    if (getIndexBySearchingInDictionary(dictionary, PC) != -1) {
      P->ptr = (char *) PC->ptr;
      P->size = PC->size;
    } else {
      char *encodedStr = (char *) huffmanTable->get(huffmanTable, strC->ptr);

      // When the static huffman table doesn't contain the character
      if (!encodedStr)return NULL;

      int pIndex = P->ptr == NULL ? 0 : getIndexBySearchingInDictionary(dictionary, P);
      code->insert(code, pIndex, encodedStr);

      Data *new_value = (Data *) malloc(sizeof(Data));
      new_value->ptr = PC->ptr;
      new_value->size = PC->size;

      // Free memory before replace a dictionary value if exists
      freeDictionaryEntry(dictionary, index);

      dictionary[index] = new_value;
      index = index < 255 ? index + 1 : 1;

      P->ptr = NULL;
      P->size = 0;
    }
  }

  if (P->ptr != NULL) {
    int pIndex = getIndexBySearchingInDictionary(dictionary, P);
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

    unsigned int indexSize = index < 16 ? 4 : 8;
    maxIndexSize = maxIndexSize < indexSize ? indexSize : maxIndexSize;

    numOfBits += indexSize;
    numOfBits += encodedString ? strlen(encodedString) : 8; // 8 == possibile ultimo risultato di lz78 (NULL)
  } while ((element = element->next) != NULL);

  if (type == DYNAMIC_HUFFMAN) {
    numOfBits += 4 + maxIndexSize * 256; // length size byte + 256 chars * max length
  }

  eval->numOfBits = numOfBits;
  eval->maxIndexSize = maxIndexSize;
  return eval;
}

/**
 * Encode index
 *
 * @param {Unsigned int} index
 * @return {Char pointer} (malloc) - number of bits + binary number
 */
char *getStringOfBitsForIndex(unsigned int index) {
  int indexSize = index < 16 ? 4 : 8;
  char *res = (char *) malloc(indexSize + 3); // +3 = '\0' + 2 bits of index size
  strcpy(res, "");

  switch (indexSize) {
    case 4:strcat(res, "10");
      break;
    case 8:strcat(res, "01");
      break;
    default:break;
  }

  int2bin(index, indexSize, res);
  return res;
}

/**
 * Write into the output file the canonical huffman table
 *
 * @param {Map pointer} data
 * @param {Hashmap pointer} huffman_table
 */
void writeCanonicalHuffman(unsigned int maxIndexSize, HashMap *huffman_table) {
  char lengthSize[9];
  int2bin(maxIndexSize, 4, lengthSize);
  writeStringOfBitsIntoFile(lengthSize);

  // 0..255 ASCII CODES
  for (int i = 0; i < 256; i++) {
    char c[2] = {(char) i, '\0'};
    char *encoded = huffman_table->get(huffman_table, c);

    char *res = (char *) malloc(sizeof(char) * (maxIndexSize + 1));
    strcpy(res, "");
    int2bin(encoded ? strlen(encoded) : 0, maxIndexSize, res);
    writeStringOfBitsIntoFile(res);
    free(res);
  }
}

/**
 * Write LZ78 code result into the output file
 *
 * @param {Char pointer} header - block header
 * @param {Map pointer} data - LZ78 result
 */
void writeCode(char *header, Map *data) {
  if (header)
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

/**
 * Write not compressed block in the output file
 *
 * @param {Char pointer} header
 * @param {Data pointer} data
 */
void writeNotCompressedBlock(char *header, Data *data) {
  char *length = (char *) malloc(sizeof(char) * 18); // 17 bit for 65536 + null char
  memset(length, 0, 18);
  int2bin(data->size, 17, length);

  writeStringOfBitsIntoFile(header);
  writeStringOfBitsIntoFile(length);
  writeBlock(data);

  free(length);
}