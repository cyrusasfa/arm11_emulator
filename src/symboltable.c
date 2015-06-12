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
  if (map->keys == NULL) {
    perror("malloc for keys failed\n");
    exit(EXIT_FAILURE);
  }
  map->values = (int *)malloc(sizeof(int) * map->space);
  if (map->values == NULL) {
    perror("malloc for values failed\n");
    exit(EXIT_FAILURE);
  }
}

void free_table(Map *map) {
  for (int i = 0; i < map->size; i++) {
    free(map->keys[i]);
  }
  free(map->keys);
  free(map->values);
  free(map);
}

int look_up(Map *map, char *key) {
  // return 0 if not found
  int index = get_index(map, key);
  if (index == -1) {
    printf("Key not in map: %s\n", key);
    return NOT_FOUND;
  }
  return map->values[index];
}

void double_space(Map *map) {
  map->space *= 2; 
  map->keys = (char **)realloc(map->keys, sizeof(char) * map->space);
  if (map->keys == NULL) {
    perror("realloc for keys failed\n");
    exit(EXIT_FAILURE);
  }
  map->values = (int *)realloc(map->values, sizeof(int) * map->space);
  if (map->values == NULL) {
    perror("realloc for keys failed\n");
    exit(EXIT_FAILURE);
  }
}

void insert(Map *map, char *key, int value) {
  if (map->space <= map->size) {
  	double_space(map); // reallocates space for the list
  }
  if (contains(map, key)) {
    map->values[get_index(map, key)] = value;
  } else {
    map->keys[map->size] = malloc(sizeof(char) * MAX_LENGTH);
    if (map->keys[map->size] == NULL) {
      perror("malloc failed for key insertion\n");
      exit(EXIT_FAILURE);
    }
    strcpy(map->keys[map->size], key);
    map->values[map->size] = value;
    map->size++;
  }
}

int get_index(Map *map, char *key) {
  int index = -1;
  for (int i = 0; i < map->size; i++) {
    if (strcmp(map->keys[i], key) == 0) {
      index = i;
    }
  }
  return index;
}

int contains(Map *map, char *key) {
  int res = get_index(map, key);
  return res != NOT_FOUND;
}
