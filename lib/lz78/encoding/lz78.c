#include "lz78.h"

const unsigned int MAX_BLOCK_SIZE = 65536; // 64 KiB

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