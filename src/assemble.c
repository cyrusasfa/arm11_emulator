#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LENGTH (512)
#define MAX_OPERANDS (3)

struct Instruction {
  int numOperands;
  char* label;
  char* opcode;
  char * operands[MAX_OPERANDS];
};

void write_to_output(FILE *dst, int x);

void tokeniseLine(struct Instruction*  parsedinput, FILE* src);

int getnumlines(FILE* src);

int main(int argc, char **argv) {
  assert(argc == 3);
  
  const char *srcname = argv[1];
  const char* dstname = argv[2];
  
  FILE *src;
  FILE * dst;
  
  src = fopen(srcname, "r");
  dst = fopen(dstname, "wb");
  
  assert (src != NULL);
  assert (dst != NULL);
  const int linenum = getnumlines(src);
  struct Instruction instructions[linenum];      
  tokeniseLine(instructions, src);
    
      
  
  fclose(src);
  fclose(dst);
  
  return EXIT_SUCCESS;
}

//writes an int (instruction) to the binary file;
void writeToOutput(FILE *dst, int x) {
  fwrite(&x, sizeof(x), 1, dst);
}

int getnumlines(FILE* src) {
  int num = 0;
  char str[MAX_LENGTH];
  while (fgets(str, MAX_LENGTH, src)) {
    num++;
  }
  fseek(src, 0, SEEK_SET);
  return num;
}

void tokeniseLine(struct Instruction* instructions, FILE* src) {
  char line[MAX_LENGTH];
  for (int i = 0; fgets(line, MAX_LENGTH, src); i++) {
    instructions[i].numOperands = 0;
    if (strchr(line, ':')) {
      instructions[i].label = strtok(line, ":");
      printf("%s\n", instructions[i].label);
    }
    else {
      instructions[i].opcode = strtok(line, " ");
      printf("%s\n", instructions[i].opcode);  
      char* nextOperand = strtok(NULL, ",");
      while(nextOperand!= NULL) {
        instructions[i].operands[instructions[i].numOperands]
         = nextOperand;
        nextOperand = strtok(NULL, ",");
        instructions[i].numOperands ++;
      }
      for (int j = 0; j < instructions[i].numOperands; j++) {
          printf("%s\n", instructions[i].operands[j]);
      }
    }
  }
}

#define INIT_SPACE = 50

typedef struct key_value_map {
  char **keys; // label references
  int *values; // addresses 
  int size;
  int space;
} Map; 
// can refer to struct as SymbolTable rather than struct SymbolTable

// ADT procedures
void init(Map *map);
int look_up(Map *map, char *key);
void insert(Map *map, char *key, int value);
void double_size(Map *map);

void init(Map *map) {
  map->size = 0;
  map->space = INIT_SPACE;
  map->keys = (char **)malloc(sizeof(char) * max_size);
  map->values = (int *)malloc(sizeof(int) * max_size);
}

void free(Map *map) {
  // reset map
  free(map->keys);
  free(map->values);
  free(map);
}

int look_up(Map *map, char *key) {
  // return 0 if not found
  int pos = -1;
  for (int i = 0, i < map->size, i++) {
  	if (strcmp(map->keys, key) == 0) {
  	  pos = i;
  	}
  }
  if (pos == -1) {
  	printf("Key not in map: %s\n", key);
  	return;
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
//   	if (strcmp(curr->key, k) == 0) {
//       return curr->value;
//   	}
//   	curr = curr->next;
//   }
//   printf("Key not found: %s/n", k);
// }

// void insert(Map *map, char *k, int v) {
//   if (map->size == 0) {
//   	map->root = malloc(sizeof(Node));
//   	(map->root)->key = k;
//   	(map->root)->value = v;
//   	(map->root)->has_next = false;
//   } else {
//   	Node curr = map->root;
//   	while (has_next(curr)) {
//       curr = curr->next
//   	}
//   	Node *new = malloc(sizeof(Node));
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


