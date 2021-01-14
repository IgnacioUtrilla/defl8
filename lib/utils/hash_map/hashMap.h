#ifndef __HASH_MAP__
#define __HASH_MAP__

#define MAX_HASHMAP_SIZE 256
#define MAX_HASHMAP_SIZE_X2 512

typedef struct hmnode {
  void *value;
  char *key;
} hmNode;

typedef struct hashmap {
  int size;
  struct hmnode *array[MAX_HASHMAP_SIZE_X2]; // Load Factor = 0.5, numero massimo di elementi = MAX_HASHMAP_SIZE
  void *(*get)(struct hashmap *, char *key);
  int (*delete)(struct hashmap *, char *key);
  int (*insert)(struct hashmap *, char *key, void *value);
} HashMap;

HashMap *createHashMap();
void removeHashMap(HashMap *hashmap);
void *getHashMapValue(struct hashmap *, char *key);
int deleteHashMapValue(struct hashmap *, char *key);
int insertHashMapValue(struct hashmap *, char *key, void *value);
static int hash(char *key);

#endif