#include <stdlib.h>
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