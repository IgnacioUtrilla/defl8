#include "lz78.h"

const unsigned int MAX_BLOCK_SIZE = 3;//65536; // 64 KiB

/*
 * TODO (extra)
 *  1. Eseguire in contemporanea SH e DH (ottimizzazione)
 *  2. Programmazione dinamica (memoize)?
 *
 * TODO
 *  2. fix: (gestire file piccolo)
 *  3. Rivedere nomenclatura variabili e funzioni
 *
 */

/**
 * TODO
 * @return
 */
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

    if (!strcmp(data->ptr, ""))
      continue;

    HashMap *dynamicHuffmanTable = getHuffmanTable(data);

    Map *staticCode = encoding(data, staticHuffmanTable);
    Map *dynamicCode = encoding(data, dynamicHuffmanTable);

    unsigned int staticCodeSize = evaluate(staticCode, STATIC_HUFFMAN);
    unsigned int dynamicCodeSize = evaluate(dynamicCode, DYNAMIC_HUFFMAN);
    unsigned long dataDimensionNonCompressed = data->size * 8;

    char *header = (char *) malloc(sizeof(char) * 4); // 3 bits + \0
    strcpy(header, isLastBlock() ? "1" : "0");

    if (staticCodeSize < dynamicCodeSize && staticCodeSize < dataDimensionNonCompressed) {
      // save static code, header 001
      strcat(header, "01");
      writeCode(header, staticCode);
    } else if (dynamicCodeSize < staticCodeSize && dynamicCodeSize < dataDimensionNonCompressed) {
      // save dynamic code, header 010
      strcat(header, "10");
      writeCode(header, staticCode);
    } else {
      // save non compressed block, header 000
      strcat(header, "00");
      writeStringOfBitsIntoFile(header);
      writeBlock(data);

      // end-of-non-compressed-block code
      writeStringOfBitsIntoFile("00000000");
    }

    free(header);
    // FIXME: createHuffmanTree si spacca con una freqMap con un solo elemento
  }

  closeStream(READ);
  closeStream(WRITE);
}