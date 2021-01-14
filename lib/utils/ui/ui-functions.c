#include <stdio.h>
#include "ui-functions.h"

void printTitle() {
  printf("██████╗ ███████╗███████╗██╗      █████╗\n");
  printf("██╔══██╗██╔════╝██╔════╝██║     ██╔══██╗\n");
  printf("██║  ██║█████╗  █████╗  ██║     ╚█████╔╝\n");
  printf("██║  ██║██╔══╝  ██╔══╝  ██║     ██╔══██╗\n");
  printf("██████╔╝███████╗██║     ███████╗╚█████╔╝\n");
  printf("╚═════╝ ╚══════╝╚═╝     ╚══════╝ ╚════╝\n");
  printf("by RB & IUM v1.0\n");
}

void printCompressionRatio(char *before, char *after) {
  FILE *fpBef = fopen(before, "r");
  FILE *fpAft = fopen(after, "r");

  fseek(fpBef, 0L, SEEK_END);
  fseek(fpAft, 0L, SEEK_END);

  // calculating the size of the file
  long beforeSize = ftell(fpBef);
  long afterSize = ftell(fpAft);

  float ratio = ((float) (beforeSize - afterSize) / beforeSize) * 100;
  // closing the file
  fclose(fpBef);
  fclose(fpAft);

  printf("\nadding: %s (compression ratio: %.0f%%)\n", after, ratio);

}