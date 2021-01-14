#include <stdio.h>
#include <string.h>
#include "lib/lz78/decoding/lz78.h"
#include "lib/lz78/encoding/lz78.h"
#include "lib/utils/ui/ui-functions.h"

int main(int argc, char *argv[]) {
  // argc rappresenta la dimensione (numero di elementi) dell'array (di stringhe) argv
  // il primo elemento di argv (a indice 0) e' il nome dell'eseguibile
  printTitle();

  if (argc < 2) {
    fprintf(stderr, "Numero insufficiente di argomenti\n");
    return 1;
  }

  // esempio di verifica della string all'indice 1 dell'array
  if (strcmp(argv[1], "-c") == 0) {
    encoding(argv[2], argv[3]);
    printCompressionRatio(argv[2], argv[3]);
  } else if (strcmp(argv[1], "-d") == 0)
    decoding(argv[2], argv[3]);
  else {
    fprintf(stderr, "Argomento non valido\n");
    return 2;
  }

  return 0;
}