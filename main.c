#include <stdio.h>
#include "lib/file_manager/filemanager.h"

int main(int argc, char** argv) {
    Data data = readFile("../main.c");
    saveFile(data, "output.c");
    return 0;
}
