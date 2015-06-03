#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H 

#define INIT_SPACE 50
#define NOT_FOUND -1

typedef struct key_value_map {
  char **keys; // label references
  int *values; // addresses 
  int size;
  int space;
} Map; 
// can refer to struct as SymbolTable rather than struct SymbolTable

// ADT procedures
void init_table(Map *map);
int look_up(Map *map, char *key);
void insert(Map *map, char *key, int value);
void double_size(Map *map);
void free_table(Map *map);

#endif