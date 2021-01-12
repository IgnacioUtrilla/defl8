#include "map.h"
#include <stdlib.h>
#include <string.h>

// TODO - Aggiungere commenti

/**
 * TODO
 *  1. Verifica se la chiave non esiste già
 *
 * Insert an element into map if not already exist
 *
 * @param {Pointer to Map} *map
 * @param {Pointer to char} *key
 * @param {Float} value
 */
void insertMapValue(Map *map, char *key, void *value) {
  Element *newNode = (Element *) malloc(sizeof(Element));
  newNode->key = key;
  newNode->value = value;
  newNode->next = NULL;

  map->size++;

  if (!map->element) {
    map->element = newNode;
    map->last = newNode;
    return;
  }

  map->last->next = newNode;
  map->last = newNode;
}

void *getMapValue(Map *map, char *key) {
  if (map->element == NULL || !key)
    return 0;

  Element *element = map->element;
  while (strcmp(element->key, key) != 0 && element->next != NULL) {
    element = element->next;
  }

  if (!strcmp(element->key, key))
    return element->value;

  return 0;
}

int deleteMapValue(Map *map, char *key) {
  Element *element = map->element;
  Element *previousElement = map->element;

  if (!strcmp(map->element->key, key)) {
    map->element = map->element->next;
    map->size--;
//    free(element->value);
    free(element);
    return 0;
  }

  while (element->next != NULL && strcmp(element->key, key)) {
    previousElement = element;
    element = element->next;
  }

  if (!strcmp(element->key, key)) {
    if (element == map->last)
      map->last = previousElement;

    if (previousElement->next == element->next) {
      previousElement = element->next;
    } else {
      previousElement->next = element->next;
//      free(element->value);
      free(element);
    }

    map->size--;

    if (!map->size)
      map->element = NULL;

    return 0;
  }

  return 1;
}

/**
 * Funzione per la ricerca del valore minimo all'interno della mappa
 *
 * @param {Map pointer} map
 * @param {Function pointer} getValue - Se secondo parametro == NULL va ad utilizzare il valore value deferenziandolo
 * @return {map Element}
 */

Element findMin(Map *map, Element (*getValue)(void *)) {
  if (getValue == NULL) {
    Element *current = map->element;
    Element min;

    static char *minChar;
    float minValue = 1.01f;

    min.value = &minValue;
    for (int i = 0; i < map->size; i++) {
      if (*(float *) current->value < *(float *) min.value) {
        min = *current;
        minChar = current->key;
      }
      current = current->next;
    }

    return min;
  } else {
    return getValue(map);
  }
}

Map *createMap() {
  Map *map = (Map *) malloc(sizeof(Map));
  map->size = 0;
  map->element = NULL;
  map->last = NULL;
  map->insert = &insertMapValue;
  map->delete = &deleteMapValue;
  map->get = &getMapValue;

  return map;
}

// TODO: create destructor