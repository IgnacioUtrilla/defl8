#include <string.h>
#include "lz78.h"

int checkLastBlock(uc *bit) {
  return bitStatus(bit, 0);
}

encodeType defineEncode(uc *bit) {
  char *encode = defineEncodeString(bit, 2);
  if (!strcmp(encode, "00")) {
    return NOT;
  } else if (!strcmp(encode, "01")) {
    return FIXED;
  } else if (!strcmp(encode, "10")) {
    return DYNAMIC;
  } else {
    return ERROR;
  }
}

int defineIndexLength(uc *bit) {
  char *encode = defineEncodeString(bit, 2);
  if (!strcmp(encode, "10")) {
    return 4;
  } else if (!strcmp(encode, "01")) {
    return 8;
  } else if (!strcmp(encode, "11")) {
    return 0;
  }
}

char *decodeChar(uc *bit, HashMap *hash_map, int maxSizeEncode) {
  char *newChar = NULL;
  char *encode = "";

  if (isNextEnd(bit, 8, "00000000"))
    return "00000000";

  for (int i = 0; i < maxSizeEncode; i++) {
    char *newBit = defineEncodeString(bit, 1);
    encode = concat(encode, newBit);

    for (int j = 0; j < MAX_HASHMAP_SIZE_X2; j++)
      if (hash_map->array[j] != NULL && !strcmp(hash_map->array[j]->value, encode)) {
        newChar = hash_map->array[j]->key;
        break;
      }

    if (newChar) break;
  }

  return newChar;
}

void addDictionaryEntry(Data **dictionaryEntry, char *oldChar, char *newChar, int oldCharLength, int newCharLength) {
  Data *old = *dictionaryEntry;
  *dictionaryEntry = (Data *) malloc(sizeof(Data));
  (*dictionaryEntry)->size = oldCharLength + newCharLength;
  (*dictionaryEntry)->ptr = concatCharArrays(oldChar, newChar, oldCharLength, newCharLength);
  free(old);
}

int decodeLZ78(uc *bit, HashMap *hash_map, int maxSizeEncode) {
  int dicCount = 1;
  Data *newBlock;
  Data *dictionary[256] = {0};
  int indexLength;
  char *newChar;
  int index;
  Data *data = (Data *) malloc(sizeof(Data));
  data->size = 0;
  data->ptr = (char *) malloc(sizeof(char));

  while ((indexLength = defineIndexLength(bit))) {
    index = bin2int(defineEncodeString(bit, indexLength));
    newChar = decodeChar(bit, hash_map, maxSizeEncode);

    if (!strcmp(newChar, "00000000")) {
      char *old = data->ptr;

      data->ptr = concatCharArrays(data->ptr, dictionary[index]->ptr, data->size, dictionary[index]->size);
      data->size = data->size + dictionary[index]->size;
      if (old) free(old);
      continue;
    }
    if (dicCount == 256) dicCount = 1;

    if (!index) {
      char *oldChar = (char *) malloc(0);
      addDictionaryEntry(&dictionary[dicCount], oldChar, newChar, 0, 1);
      dicCount++;
      int temp = dicCount - 1;
      newBlock = dictionary[temp];
    } else {
      addDictionaryEntry(&dictionary[dicCount], dictionary[index]->ptr, newChar, dictionary[index]->size, 1);
      dicCount++;
      int temp = dicCount - 1;
      newBlock = dictionary[temp];
    }
    char *old = data->ptr;
    data->ptr = concatCharArrays(data->ptr, newBlock->ptr, data->size, newBlock->size);
    data->size = data->size + newBlock->size;

    if (old) free(old);
  }
  writeBlock(data);
  free(data);
  return indexLength;
}

int notBlock(uc *bit, int *indexLength) {
  char *totalBytesCode = defineEncodeString(bit, 17);
  unsigned int totalBytes = bin2int(totalBytesCode);
  for (int i = 0; i < totalBytes; i++) {

    char *newChar = defineEncodeString(bit, 8);
    char c = (char) bin2int(newChar);
    char *writeC = (char *) malloc(sizeof(char) * 2);
    writeC[0] = c;
    writeC[1] = '\0';

    Data *writeData = (Data *) malloc(sizeof(Data));
    writeData->ptr = writeC;
    writeData->size = 1;
    writeBlock(writeData);

    free(writeData);
    free(writeC);
  }
  (*indexLength) = 0;
  return *indexLength;
}

void decoding(char *fileName, char *outputFile) {
  openStream(outputFile, WRITE);
  openStream(fileName, READ);

  stream_status st;
  uc bit;
  char *length;
  int indexLength;
  int length_int;

  HashMap *hash_map_fix = getHuffmanStaticTable();

  while ((st = readBit(&bit)) != ST_EOS) {
    int isLast = checkLastBlock(&bit);
    encodeType encode_type = defineEncode(&bit);

    switch (encode_type) {
      case NOT:indexLength = notBlock(&bit, &indexLength);
        break;
      case FIXED:indexLength = decodeLZ78(&bit, hash_map_fix, 9);
        break;
      case DYNAMIC: {
        CanonicalValue canonicalLength[256] = {0};
        length = defineEncodeString(&bit, 4);
        indexLength = bin2int(length);

        int count = 0;
        for (int i = 0; i < 256; i++) {
          length = defineEncodeString(&bit, indexLength);
          length_int = bin2int(length);

          CanonicalValue *canonicalValue = (CanonicalValue *) malloc(sizeof(CanonicalValue));
          canonicalValue->character = i;
          canonicalValue->length = length_int;
          canonicalLength[i] = *canonicalValue;

          if (length_int) count++;
        }

        CanonicalValue canonicalLengthFiltered[count];
        int maxSizeEncode = count = 0;
        for (int i = 0; i < 256; i++) {
          if (canonicalLength[i].length) {
            canonicalLengthFiltered[count] = canonicalLength[i];
            maxSizeEncode = maxSizeEncode < canonicalLength[i].length ? canonicalLength[i].length : maxSizeEncode;
            count++;
          }
        }

        HashMap *hash_map_dyn = createCanonicalHuffmanTable(count, canonicalLengthFiltered);
        indexLength = decodeLZ78(&bit, hash_map_dyn, maxSizeEncode);
        break;
      }
      case ERROR:
      default:printf("ERROR\n");
        break;
    }

    if (isLast) break;
  }

  closeStream(WRITE, NULL);
  closeStream(READ, NULL);
}