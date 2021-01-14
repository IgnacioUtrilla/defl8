#include "criterion/criterion.h"
#include <criterion/theories.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "map.c"

/**
 * TODO
 *  Mostare il valori che vanno in errore per le Theory
 */

/*
 * CREATE
 */

Test(MAP, CREATE) {
  Map *map = createMap();
  cr_expect_eq(map->size, 0, "DEFAULT DIMENSION AFTER CREATION");
  cr_expect_eq(map->element, NULL, "ELEMENT EQ. NULL");
}

/*
 * INSERT
 */

TheoryDataPoints(MAP, INSERT) = {
    DataPoints(char *, "a", "test", NULL),
    DataPoints(float, 0.0f, -1.f, 1.f, -2.f, 2.f, FLT_MAX, FLT_MIN, +INFINITY, -INFINITY),
};

Theory((char *key, float value), MAP, INSERT) {
  Map *map = createMap();
  map->insert(map, key, &value);
  cr_expect_eq(map->size, 1, "DIMENSION INCREMENT AFTER ADD AN ELEMENT");
}

/*
 * GET
 */

TheoryDataPoints(MAP, GET) = {
    DataPoints(char *, "a", "test"),
    DataPoints(float, 0.0f, -1.f, 1.f, -2.f, 2.f, FLT_MAX, FLT_MIN, +INFINITY, -INFINITY)
};

Theory((char *key, float value), MAP, GET) {
  Map *map = createMap();
  map->insert(map, key, &value);
  cr_expect_eq(*(float *) map->get(map, key), value, "GET EXISTENT VALUE");
}

Test(MAP, GET_WITH_NULL_KEY) {
  Map *map = createMap();
  cr_expect_eq(map->get(map, NULL), 0);
}

/*
 * DELETE
 */

Test(MAP, DELETE) {
  Map *map = createMap();
  int *value = (int *) malloc(sizeof(int));
  *value = 123;

  map->insert(map, "test", &value);
  map->delete(map, "test");
  cr_expect_eq(map->size, 0);
}