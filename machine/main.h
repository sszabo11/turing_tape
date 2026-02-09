#include "hashmap.h"

typedef struct {
  char *state;
  int value;
  int write;
  char *dir;
  char *new_state;
} Instruction;

typedef struct {
  int *memory;
  int len;
} Memory;

typedef struct {
  Memory *memory;
  Instruction **instructions;
  int num_instructions;
  HashTable *instructions_table;
  char state[64];
  int cell;
} Machine;

char *print_cell_value(int *val);
