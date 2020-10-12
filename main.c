#include <stdio.h>
#include "./lib/filemanager.h"

int main(int argc, char** argv) {
    char * file;
    file = readFile("../main.c");
    saveFile(file, "output.c");
    return 0;
}