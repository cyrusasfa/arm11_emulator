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
  	if (strcmp(map->keys, key) == 0) {
  	  pos = i;
  	}
  }
  if (pos == -1) {
  	printf("Key not in map: %s\n", key);
  	return NOT_FOUND;
  }
  return map->values[pos];
}

void insert(Map *map, char *key, int value) {
  if (map->space <= map->size) {
  	double_space(map); // reallocates space for the list
  }
  map->keys[map->size] = key;
  map->values[map->size] = value;
  map->size++;
}

void double_space(Map *map) {
  map->space *= 2;
  map->keys = (char **)realloc(map->keys, sizeof(char) * map->space);
  map->values = (int *)realloc(map->values, sizeof(int) * map->space);
}

// typedef struct node {
//   char *key;
//   int value;
//   struct node next;
//   bool has_next;
// } Node;

// typedef struct map {
//   Node *root;
//   int size;
// } Map;

// void init(Map *map) {
//   map->size = 0;
// }

// int look_up(Map map*, char *k) {
//   Node curr = map->root;
//   if (map->size == 0) {
//   	printf("look_up error, map size is: %i\n", map->size);
//   	return;
//   }
//   Node *curr = map->root;
//   while (has_next(curr)) {
//     if (strcmp(curr->key, k) == 0) {
//       return curr->value;
//     }
//     curr = curr->next;
//   }
//   printf("Key not found: %s/n", k);
// }

// void insert(Map *map, char *k, int v) {
//   if (map->size == 0) {
//   	map->root = (Node *)malloc(sizeof(Node));
//   	(map->root)->key = k;
//   	(map->root)->value = v;
//   	(map->root)->has_next = false;
//   } else {
//   	Node curr = map->root;
//   	while (has_next(curr)) {
//       curr = curr->next
//   	}
//   	Node *new = (Node *)malloc(sizeof(Node));
//   	new->key = k;
//   	new->value = v;
//   	new->has_next = false;
//   	curr->next = new;
//   	curr->has_next = true;
//   }
//   map->size++;
// }

// bool has_next(Node *node) {
//   return node->has_next;
// }
