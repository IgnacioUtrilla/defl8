#include "lz78.h"

const unsigned int MAX_BLOCK_SIZE = 65536; // 64 KiB

/*
 * TODO (extra)
 *  1. Eseguire in contemporanea SH e DH (ottimizzazione)
 *  2. Programmazione dinamica (memoize)?
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
  data->size = 0;

  char *header = (char *) malloc(sizeof(char) * 4); // 3 bits + \0
  int write_buffer = 0;

  while (readBlock(MAX_BLOCK_SIZE, data) == ST_OK) {
    write_buffer = 1;
    HashMap *dynamicHuffmanTable = getHuffmanTable(data);

    Map *staticCode = encoding(data, staticHuffmanTable);
    Map *dynamicCode = encoding(data, dynamicHuffmanTable);

    Evaluate *evaluatedStaticCode = staticCode ? evaluate(staticCode, STATIC_HUFFMAN) : NULL;
    Evaluate *evaluatedDynamicCode = evaluate(dynamicCode, DYNAMIC_HUFFMAN);

    unsigned int staticCodeSize = evaluatedStaticCode ? evaluatedStaticCode->numOfBits : NULL;
    unsigned int dynamicCodeSize = evaluatedDynamicCode->numOfBits;
    unsigned long dataDimensionNonCompressed = data->size * 8;

    strcpy(header, isEOF() ? "1" : "0");

    if (staticCode && staticCodeSize < dynamicCodeSize && staticCodeSize < dataDimensionNonCompressed) {
      // save static code, header 001
      strcat(header, "01");
      writeCode(header, staticCode);
    } else if ((!staticCode || dynamicCodeSize < staticCodeSize) && dynamicCodeSize < dataDimensionNonCompressed) {
      // save dynamic code, header 010
      strcat(header, "10");
      writeStringOfBitsIntoFile(header);
      writeCanonicalHuffman(evaluatedDynamicCode->maxIndexSize, dynamicHuffmanTable);
      writeCode(NULL, dynamicCode);
    } else {
      // save non compressed block, header 000
      strcat(header, "00");
      writeNotCompressedBlock(header, data);
    }

    free(evaluatedStaticCode);
    free(evaluatedDynamicCode);
  }

  puts("Finish");

  free(header);

  closeStream(READ, NULL);
  closeStream(WRITE, write_buffer);
}