#include <string.h>
#include "lz78.h"
#include "../../utils/bit_stream/bitStream.h"
#include "../../file_manager/filemanager.h"

const unsigned int MAX_BLOCK_SIZE = 65536; // 64 KiB

typedef enum { DYNAMIC_HUFFMAN, STATIC_HUFFMAN } huffman_type;

/*
 * TODO
 *  1. Valutare se c'è un blocco successivo oppure no (header block)
 *  2. Eseguire in contemporanea SH e DH (ottimizzazione)
 *  3. Programmazione dinamica (memoize)?
 *
 * TODO
 *  1. gestire file piccoli
 *  3. Rivedere nomenclatura variabili e funzioni
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
    void *encodedChar = huffmanTable->get(huffmanTable, P);
    int pIndex = *(int *) dictionary->get(dictionary, P);
    code->insert(code, !pIndex ? pIndex : 0, encodedChar);
  }

  return code;
}

/**
 * Get the number of bits that occupy the LZ78 code depending on the huffman table
 *
 * @param {Map pointer} code - element [index, encoded string]
 * @return number of bits
 */
unsigned int evaluate(Map *code, huffman_type type) {
  Element *element = code->element;
  unsigned int numOfBits = 5; // Initial 3 bits = header + 2 bits of end-of-compressed-block
  unsigned int maxIndexSize = 0;

  do {
    unsigned int index = element->key;
    char *encodedString = element->value;

    unsigned int indexSize = index < 16 ? 4 : index < 256 ? 8 : 12;
    maxIndexSize = maxIndexSize < indexSize ? indexSize : maxIndexSize;

    numOfBits += indexSize;
    numOfBits += strlen(encodedString);
  } while ((element = element->next) != NULL);

  if (type == DYNAMIC_HUFFMAN) {
    numOfBits += 8 + maxIndexSize * 256; // length size byte + 256 chars * max length
  }

  return numOfBits;
}

int writeStringOfBitsIntoFile(char *data) {
  for (unsigned int i = 0; i < strlen(data); i++) {
    uc bit = data[i] - '0';
    writeBit(bit);
  }

  return 0;
}

char *int2bin(unsigned int number, int number_of_bits, char *dest) {
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

void writeCode(char *header, Map *data) {
  writeStringOfBitsIntoFile(header);

  Element *code = data->element;
  while (code != NULL) {
    unsigned int index = code->key;
    char *encodedIndex = getStringOfBitsForIndex(index);
    char *encoded = code->value;

    writeStringOfBitsIntoFile(encodedIndex);
    writeStringOfBitsIntoFile(encoded);
    free(encodedIndex);

    code = code->next;
  }

  // end-of-compressed-block code
  writeStringOfBitsIntoFile("11");
}

int main() {
  char *input = "../lib/lz78/encoding/test_files/test1/input.txt";
  char *output = "../lib/lz78/encoding/test_files/test1/output.def8";
  HashMap *staticHuffmanTable = getHuffmanStaticTable();

  openStream(input, READ);
  openStream(output, WRITE);

  Data *data = (Data *) malloc(sizeof(Data));
  data->ptr = (char *) malloc(sizeof(char) * MAX_BLOCK_SIZE);

  while (readBlock(MAX_BLOCK_SIZE, data->ptr) == ST_OK) {
    data->size = strlen(data->ptr);
    HashMap *dynamicHuffmanTable = getHuffmanTable(data);

    Map *staticCode = encoding(data, staticHuffmanTable);
    Map *dynamicCode = encoding(data, dynamicHuffmanTable);

    unsigned int staticCodeSize = evaluate(staticCode, STATIC_HUFFMAN);
    unsigned int dynamicCodeSize = evaluate(dynamicCode, DYNAMIC_HUFFMAN);
    unsigned long dataDimensionNonCompressed = data->size * 8;

    if (staticCodeSize < dynamicCodeSize && staticCodeSize < dataDimensionNonCompressed) {
      // save static code, header 001
      writeCode("001", staticCode);
    } else if (dynamicCodeSize < staticCodeSize && dynamicCodeSize < dataDimensionNonCompressed) {
      // save dynamic code, header 010
      writeCode("010", staticCode);
    } else {
      // save non compressed block, header 000
      writeStringOfBitsIntoFile("000");
      saveFile(data, output);

      // end-of-non-compressed-block code
      writeStringOfBitsIntoFile("00000000");
    }

    // FIXME: crasha con blocchi piccoli (prova a mettere dimensione del blocco = 3)
  }

  closeStream(READ);
  closeStream(WRITE);
}