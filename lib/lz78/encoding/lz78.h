#ifndef __LZ78_ENCODING__
#define __LZ78_ENCODING__

#include <string.h>

#include "utils/utils.h"
#include "../../file_manager/filemanager.h"
#include "../../huffman/huffman.h"

// TODO rivedere nome
Map *encoding(Data *data, HashMap *huffmanTable);

#endif