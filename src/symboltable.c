#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "symboltable.h"

void init_table(Map *map) {
  map->size = 0;
  map->space = INIT_SPACE;
  map->keys = (char **)malloc(sizeof(char) * map->space);
  map->values = (int *)malloc(sizeof(int) * map->space);
}

void free_table(Map *map) {
  // reset map
  free(map->keys);
  free(map->values);
  free(map);
}

int look_up(Map *map, char *key) {
  // return 0 if not found
  int pos = -1;
  for (int i = 0; i < map->size; i++) {
  	if (strcmp(map->keys[i], key) == 0) {
  	  pos = i;
  	}
  }
  if (pos == -1) {
  	printf("Key not in map: %s\n", key);
  	return NOT_FOUND;
  }
  return map->values[pos];
}

void double_space(Map *map) {
  map->space *= 2;
  map->keys = (char **)realloc(map->keys, sizeof(char) * map->space);
  map->values = (int *)realloc(map->values, sizeof(int) * map->space);
}

void insert(Map *map, char *key, int value) {
  if (map->space <= map->size) {
  	double_space(map); // reallocates space for the list
  }
  map->keys[map->size] = key;
  map->values[map->size] = value;
  map->size++;
}
