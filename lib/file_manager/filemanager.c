#include "filemanager.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * TODO:
 *  1. Migliorare/aggiungere commenti
 *  2. Migliorare log errore
 */
Data readFile(char *filename) {
    Data data;
    FILE * file;
    long size;
    char * buffer;
    size_t result;

    file = fopen(filename,"rb");
    if (file==NULL) {
        perror("Error: File not found");
        exit(1);
    }

    fseek(file , 0 , SEEK_END);
    size = ftell(file);
    rewind(file);

    //Preparo buffer per salvare file
    buffer = (char*) malloc (sizeof(char)*size);
    if (buffer == NULL) {
        perror("Error: On memory allocation");
        exit(2);
    }

    //Leggo file e lo salvo in buffer
    result = fread(buffer,sizeof(char),size,file);
    if (result != size) {
        perror("Error: On reading file");
        exit(3);
    }

    fclose(file);
    
    data.ptr = buffer;
    data.size = size;
    return data;
}

/**
 *
 * TODO
 *  Salvare i dati a blocchi e non tutto assieme?
 *
 * @param {Pointer} data
 * @param {String} filename
 * @return {number} - 0 = no errors
 */
int saveFile(Data data, char *filename) {
    FILE * file;
    file = fopen(filename, "w");

    if (!file) perror("Error: On opening file");

    fwrite(data.ptr, sizeof(char), data.size, file);
    fclose(file);
    return 0;
}