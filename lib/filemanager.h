#include <string.h>

/**
 * TODO:
 * - Restituire dato strutturato
 * - Migliorare/aggiungere commenti
 * - Migliorare log errore
 */
char * readFile(char *filename) {
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
    return buffer;
}

/**
 *
 * TODO
 *  1. Salvare i dati a blocchi e non tutto assieme?
 *  2. Rivedere come rilevare la dimensione del dati => fwrite
 *     Utilizzare il dato strutturato
 *  3. Problemi di encoding (es.: new line)
 *
 * @param {Pointer} data
 * @param {String} filename
 * @return {number} - 0 = no errors
 */
int saveFile(char *data, char *filename) {
    FILE * file;
    file = fopen(filename, "w");

    if (!file) perror("Error: On opening file");

    fwrite(data, sizeof(char), strlen(data), file);
    fclose(file);
    return 0;
}