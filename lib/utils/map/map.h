#ifndef __HUFFMAN_MAP__
#define __HUFFMAN_MAP__

typedef struct element {
  char *key;
  void *value;
  struct element *next;
} Element;

typedef struct map {
  int size;
  void (*insert)(struct map *, char *, void *);
  int (*delete)(struct map *, char *);
  void *(*get)(struct map *, char *);
  Element *element;
} Map;

Map *createMap();
void insertMapValue(Map *map, char *key, void *value);
int deleteMapValue(Map *map, char *key);
void *getMapValue(Map *map, char *key);
Element findMin(Map *map, Element (*getValue)(void *));
#endif