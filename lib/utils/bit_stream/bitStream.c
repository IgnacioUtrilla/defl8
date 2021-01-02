#include <string.h>
#include "bitStream.h"

FILE *fin;
FILE *fout;
stream_status status_in = ST_CLOSED;
stream_status status_out = ST_CLOSED;

// Local buffer to store data
uc buf_in = 0;
uc buf_out = 0;

// Counter of local buffer
char in_cnt = 0;
char out_cnt = 0;

// open a bit stream 
stream_status openStream(char *filename, stream_mode mode) {
  if (mode == READ) {
    if (status_in != ST_CLOSED) {
      printf("Error input stream not closed\n");
      status_in = ST_ERROR;
      return ST_ERROR;
    }

    if ((fin = fopen(filename, "rb")) == NULL) {
      printf("Error opening file\n");
      status_in = ST_ERROR;
      return ST_ERROR;
    }

    status_in = ST_OPEN;
    return status_in;
  }

  if (mode == WRITE) {
    if (status_out != ST_CLOSED) {
      printf("Error output stream not closed\n");
      status_out = ST_ERROR;
      return ST_ERROR;
    }

    if ((fout = fopen(filename, "w")) == NULL) {
      printf("Error opening file\n");
      status_out = ST_ERROR;
      return ST_ERROR;
    }

    status_out = ST_OPEN;
    return status_out;
  }
}

/**
 * ATTENZIONE LA POSIZIONE È BYTE X BYTE NON BIT X BIT
 */
int isEOF() {
  fpos_t pos;
  fgetpos(fin, &pos);

  fgetc(fin);
  int isEOF = feof(fin) ? 1 : 0;
  fsetpos(fin, &pos);

  return isEOF;
}

stream_status readBlock(size_t size, Data *data) {
  if (isEOF()) return ST_EOF;

  memset(data->ptr, 0, data->size);
  data->size = fread(data->ptr, sizeof(char), size, fin);

  return ST_OK;
}

/**
 * Write the binary string into the output file
 *
 * @param {Char pointer} data - binary string
 */
void writeStringOfBitsIntoFile(char *data) {
  for (unsigned int i = 0; i < strlen(data); i++) {
    uc bit = data[i] - '0';
    writeBit(bit);
  }
}

/**
 * Write a block of data
 *
 * @param {Data pointer} data
 */
void writeBlock(Data *data) {
  char *str = (char *) malloc(sizeof(char) * 9);
  for (unsigned int i = 0; i < data->size; i++) {
    strcpy(str, "");
    int2bin((unsigned int) data->ptr[i], 8, str);
    writeStringOfBitsIntoFile(str);
  }
  free(str);
}

/**
 * Write the binary string into the output file
 *
 * @param {Char pointer} data - binary string
 */
void writeStringOfBitsIntoFile(char *data) {
  for (unsigned int i = 0; i < strlen(data); i++) {
    uc bit = data[i] - '0';
    writeBit(bit);
  }
}

/**
 * Write a block of data
 *
 * @param {Data pointer} data
 */
void writeBlock(Data *data) {
  for (unsigned int i = 0; i < data->size; i++) {
    for (unsigned int j = 0; j < 8; j++) {
      uc bit = ((data->ptr[i] >> j) & 1 ? '1' : '0');
      writeBit(bit);
    }
  }
}

// close a bit stream
stream_status closeStream(stream_mode mode) {
  if (mode == READ) {
    if (status_in != ST_OPEN) {
      printf("Error input stream not open\n");
      status_in = ST_ERROR;
      return ST_ERROR;
    }
    fclose(fin);
    status_in = ST_CLOSED;
    return status_in;
  }
  if (mode == WRITE) {
    if (status_out != ST_OPEN) {
      printf("Error input stream not open\n");
      status_out = ST_ERROR;
      return ST_ERROR;
    }

    fputc(buf_out, fout);
    fclose(fout);
    status_out = ST_CLOSED;
    return status_out;
  }
}

// read one bit from stream
stream_status readBit(uc *bit) {

  if (status_in != ST_OPEN) return ST_ERROR;

  if (in_cnt == 0) {
    int ch;
    if (EOF != (ch = fgetc(fin))) {
      if (ferror(fin) != 0) {
        printf("Problems with input file\n");
        return ST_ERROR;
      }
      buf_in = (uc) ch;
      in_cnt = 8;
    } else {
      return ST_EOS;
    }
  }

  (*bit) = bitStatus(&buf_in, in_cnt - 1);
  in_cnt--;
  return ST_OK;
}

// write one bit from stream
stream_status writeBit(uc bit) {
  if (status_out != ST_OPEN) return ST_ERROR;

  (bit > 0) ? setBit(&buf_out, 7 - out_cnt) : clearBit(&buf_out, 7 - out_cnt);

  out_cnt++;
  if (out_cnt == 8) {
    fputc(buf_out, fout);
    buf_out = out_cnt = 0;
  }

  return ST_OK;
}