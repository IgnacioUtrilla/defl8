#include "criterion/criterion.h"
#include <criterion/theories.h>
#include "hashMap.c"

/*
 * HASH FUNCTION
 */

TheoryDataPoints(HASHMAP, HASH_FUNCTION_PERMUTATION) = {
    DataPoints(char *, "tets", "tset", "tste", "ttes", "ttse"),
    DataPoints(char *, "estt", "etst", "etts", "sett", "stet", "test"),
};

Theory((char *k1, char *k2), HASHMAP, HASH_FUNCTION_PERMUTATION) {
  cr_expect_neq(hash(k1), hash(k2));
}

Test(HASHMAP, HASH_FUNCTION_SIMPLE_CASES) {
  char *k1 = "a";
  char *k2 = "b";

  cr_expect_neq(hash(k1), hash(k2));

  cr_expect_eq(hash(k1), hash(k1));
  cr_expect_eq(hash(k2), hash(k2));

  cr_expect_neq(hash(k1), hash("aa"));
}

/*
 * CREATE
 */

Test(HASHMAP, CREATE) {
  HashMap *hashMap = createHashMap();
  cr_expect_eq(hashMap->size, 0);
  cr_expect_not_null(hashMap->array);
}

/*
 * INSERT
 */

// TODO convert to theories
Test(HASHAMP, INSERT) {
  HashMap *hashMap = createHashMap();
  cr_expect_eq(hashMap->size, 0);

  int v1 = 1;
  hashMap->insert(hashMap, "t1", &v1);

  char *str = "string";
  hashMap->insert(hashMap, "t2", str);

  hashMap->insert(hashMap, "t3", NULL);

  cr_expect_eq(hashMap->size, 3);
}

/*
 * GET
 */

Test(HASHMAP, GET) {
  HashMap *hashMap = createHashMap();

  // Get integer
  int v1 = 1;
  hashMap->insert(hashMap, "t1", &v1);
  int r1 = *((int *) hashMap->get(hashMap, "t1"));
  cr_expect_eq(r1, v1);

  // Get String
  char *str = "string";
  hashMap->insert(hashMap, "t2", &str);
  char *r2 = *((char **) hashMap->get(hashMap, "t2"));
  cr_expect_str_eq(r2, str);
}

/*
 * DELETE
 */

Test(HASHMAP, DELETE) {
  HashMap *hashMap = createHashMap();

  cr_expect_eq(hashMap->size, 0);

  int v1 = 1;
  hashMap->insert(hashMap, "test", &v1);

  cr_expect_eq(hashMap->size, 1);

  cr_expect_eq(hashMap->delete(hashMap, "test"), 0);
  cr_expect_eq(hashMap->size, 0);
}