#include "lz78.h"

const unsigned int MAX_BLOCK_SIZE = 65536; // 64 KiB

/*
 * TODO (extra)
 *  1. Eseguire in contemporanea SH e DH (ottimizzazione)
 *  2. Programmazione dinamica (memoize)?
 *
 * TODO
 *  1. fix: (gestire file piccolo)
 *  2. Rivedere nomenclatura variabili e funzioni
 *  3. Rinominare il main e aggiungere i suoi parametri
 *
 */

/**
 * Compressor function
 *
 * @param {Char pointer} fin - input file directory
 * @param {Char pointer} fout - output file directory
 */
int main() {
  char *input = "../lib/lz78/encoding/test_files/test1/input.txt";
  char *output = "../lib/lz78/encoding/test_files/test1/output.txt";
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

    Evaluate *evaluatedStaticCode = evaluate(staticCode, STATIC_HUFFMAN);
    Evaluate *evaluatedDynamicCode = evaluate(dynamicCode, DYNAMIC_HUFFMAN);

    unsigned int staticCodeSize = evaluatedStaticCode->numOfBits;
    unsigned int dynamicCodeSize = evaluatedDynamicCode->numOfBits;
    unsigned long dataDimensionNonCompressed = data->size * 8;

    char *header = (char *) malloc(sizeof(char) * 4); // 3 bits + \0
    strcpy(header, isEOF() ? "1" : "0");

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

    free(evaluatedStaticCode);
    free(evaluatedDynamicCode);
    free(header);
  }

  closeStream(READ);
  closeStream(WRITE);
}