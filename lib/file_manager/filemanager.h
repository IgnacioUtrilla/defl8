#ifndef __FIlE_MANAGER__
#define __FILE_MANAGER__

#include "../utils/utils.h"

Data readFile(char *filename);
int saveFile(Data data, char *filename);

#endif