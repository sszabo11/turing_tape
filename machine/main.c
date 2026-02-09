#include "main.h"
#include "draw.h"
#include "unistd.h"
//  #include "hashmap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIX_C_SOURCE 200809L
#define MAX_INSTRUCTIONS 300
#define MAX_MEMORY 500

// typedef struct {
//   char *state;
//   int value;
//   int write;
//   char *dir;
//   char *new_state;
// } Instruction;
//
// typedef struct {
//   int *memory;
//   int len;
// } Memory;
//
// typedef struct {
//   Memory *memory;
//   Instruction **instructions;
//   int num_instructions;
//   HashTable *instructions_table;
//   char state[64];
//   int cell;
// } Machine;

char *read_file_to_string(const char *filename) {
  FILE *file_ptr = fopen(filename, "rb");

  if (file_ptr == NULL) {
    perror("Error opening file");
    return NULL;
  };

  fseek(file_ptr, 0, SEEK_END);
  long length = ftell(file_ptr);
  fseek(file_ptr, 0, SEEK_SET);

  char *buffer = (char *)malloc(length + 1);

  if (buffer == NULL) {
    perror("Error allocating memory");
    fclose(file_ptr);
    return NULL;
  }

  size_t bytes_read = fread(buffer, 1, length, file_ptr);

  if (bytes_read != length) {
    perror("Error reading file");
    free(buffer);
    fclose(file_ptr);
    return NULL;
  }

  buffer[length] = '\0';
  fclose(file_ptr);

  return buffer;
}

int get_value(char *str) {
  switch (*str) {
  case '0':
    return 0;
  case '1':
    return 1;
  case '_':
    return 2;
  default:
    return -1;
    ;
  }
  return -1;
}

char *print_cell_value(int *val) {

  switch (*val) {
  case 0:
    return "0";
  case 1:
    return "1";
  case 2:
    return "_";
  default:
    return NULL;
  }
}

Memory *parse_file(char *path, int *out_count, Instruction **instructions,
                   char *state) {
  const int LINE_LEN = 300;

  FILE *file_ptr = fopen(path, "r");

  if (file_ptr == NULL) {
    perror("Failed to open file");
    return 0;
  }

  if (!instructions) {
    fclose(file_ptr);
    *out_count = 0;
    return 0;
  }

  char line[LINE_LEN];

  int count = 0;
  int in_instructions = 0;
  int in_memory = 0;
  int in_state = 0;

  Memory *memory = malloc(sizeof(Memory));
  memory->len = 0;
  memory->memory = NULL;
  while (fgets(line, sizeof(line), file_ptr)) {

    if (line[0] == ';') {
      continue;
    }

    size_t len = strlen(line);

    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }
    if (strncmp(line, "STATE_BEGIN", 11) == 0) {
      in_instructions = 0;
      in_memory = 0;
      in_state = 1;
      continue;
    }
    if (strncmp(line, "INSTRUCTIONS", 12) == 0) {
      in_instructions = 1;
      in_memory = 0;
      in_state = 0;
    }

    if (strncmp(line, "MEMORY", 6) == 0) {
      in_instructions = 0;
      in_memory = 1;
      in_state = 0;
    }

    if (in_state) {
      char s[12];
      strcpy(s, line);

      if (strlen(s) == 0) {
        continue;
      }

      printf("State: '%s'\n", s);
      printf("l: '%s'\n", line);

      printf("\n");
      strcpy(state, line);
    }

    if (in_instructions) {
      char *line_copy = strdup(line);
      if (!line_copy)
        continue;

      char *token = strtok(line_copy, ";");

      if (token == NULL) {
        free(line_copy);
        continue;
      }

      Instruction *instr = calloc(1, sizeof(Instruction));
      if (!instr) {
        free(line_copy);
        continue;
      }

      int col = 0;
      char *field = strtok(token, " \t");

      while (field != NULL && col < 5) {

        switch (col) {
        case 0:
          // State
          instr->state = strdup(field);
          break;
        case 1:
          // Value
          instr->value = get_value(field);
          break;
        case 2:
          // Write
          instr->write = get_value(field);
          break;
        case 3:
          // Direction
          instr->dir = strdup(field);
          break;
        case 4:
          // New state
          instr->new_state = strdup(field);
          break;
        }

        col++;
        field = strtok(NULL, " \t");
      }

      if (col >= 3) {
        if (count < MAX_INSTRUCTIONS) {

          instructions[count++] = instr;
        } else {
          free(instr->state);
          free(instr->dir);
          free(instr->new_state);
          free(instr);
        }
      } else {
        free(instr->state);
        free(instr->dir);
        free(instr->new_state);
        free(instr);
      }
      free(line_copy);
    }

    char tape[1024] = {0};

    if (in_memory) {
      if (*line && line[0] == '_') {

        char *line_copy = strdup(line);
        if (!line_copy)
          continue;

        char *token = strtok(line_copy, " ");

        if (token == NULL) {
          free(line_copy);
          continue;
        }

        int i = 0;
        while (token != NULL) {
          if (*token == '_') {
            token = strtok(NULL, " ");
            continue;
          }

          tape[i] = *token;

          token = strtok(NULL, " ");
          i++;
        }
        free(line_copy);
        int len = strlen(tape);

        if (memory->memory == NULL) {
          memory->len = len;
          memory->memory = malloc(memory->len * sizeof(int));
          for (int i = 0; i < len; i++) {
            memory->memory[i] = tape[i] - '0';
          }
        }
      }
    }
  }

  fclose(file_ptr);

  *out_count = count;
  return memory;
}

static void free_instruction(Instruction *instr) {
  if (!instr)
    return;
  free(instr->state);
  free(instr->dir);
  free(instr->new_state);
  free(instr);
}

void free_memory(Memory *mem) {
  if (!mem)
    return;
  free(mem->memory);
  free(mem);
}

/* Helper to free the whole array */
void free_instructions(Instruction **instructions, int count) {
  if (!instructions)
    return;
  for (int i = 0; i < count; i++) {
    free_instruction(instructions[i]);
  }
  free(instructions);
}

Machine *init_machine(char *filepath) {
  Machine *machine = malloc(sizeof(Machine));
  strcpy(machine->state, "0");

  machine->num_instructions = 0;

  machine->instructions = calloc(MAX_INSTRUCTIONS, sizeof(Instruction));
  machine->memory = parse_file(filepath, &machine->num_instructions,
                               machine->instructions, machine->state);
  printf("%s\n", machine->state);
  machine->cell = 0;
  machine->instructions_table = create_hashmap(machine->num_instructions * 2);

  return machine;
}

void encode_key(char *dest, int dest_size, char *state, int value) {
  snprintf(dest, dest_size, "%s|%d", state, value);
}

void encode_value(char *dest, int dest_size, int write, char *dir,
                  char *new_state) {
  snprintf(dest, dest_size, "%d|%s|%s", write, dir, new_state);
}

void decode_value(Instruction *dest, char *str) {
  if (!dest)
    return;

  int write;
  char dir[20];
  char dir2[20];
  char _a[20];
  char new_state[64];

  sscanf(str, "%d|%s", &write, dir);
  sscanf(dir, "%c|%[a-z0-9A-Z-]", _a, new_state);
  sscanf(dir, "%c|", dir2);

  dest->new_state = strdup(new_state);
  dest->dir = strdup(dir2);
  dest->write = write;
}

void free_machine(Machine *machine) { free(machine); }

int execute_instruction(char *instruction_str, Machine *machine,
                        Memory *memory) {
  Instruction *instr = malloc(sizeof(Instruction));
  decode_value(instr, instruction_str);

  // printf("\n");
  // printf("Write: %d \n", instr->write);
  // printf("Dir: %s \n", instr->dir);
  // printf("New state: %s\n", instr->new_state);
  printf("%s\n", instr->dir);
  char *prefix = "hault";

  // Check if state starts with hault, then end program
  if (strncmp(instr->new_state, prefix, strlen(prefix)) == 0) {
    free(instr->new_state);
    free(instr->dir);
    free(instr);
    return 1;
  }

  // Update state
  strcpy(machine->state, instr->new_state);

  // Update cell memory position
  if (*instr->dir == '>') {
    if (machine->cell >= memory->len - 1) {
      machine->cell = 0;
    } else {
      machine->cell++;
    }
  } else if (*instr->dir == '<') {
    if (machine->cell == 0) {
      machine->cell = memory->len - 1;
    } else {
      machine->cell--;
    }

  } else if (*instr->dir == '.') {
    // Do nothing
  }

  // Update cell memory value
  memory->memory[machine->cell] = instr->write;

  if (memory->memory[machine->cell] != instr->write) {
    printf("\033[0;32mUPDATED MEMORY!\n");
    printf("\033[0m");
  }

  free(instr->new_state);
  free(instr->dir);
  free(instr);
  return 0;
}

int main(int argc, char *argv[]) {
  srand(2);

  Machine *machine = init_machine(argv[1]);

  printf("STARTING MEMORY:\n");
  for (int i = 0; i < machine->memory->len; i++) {
    printf("%d", machine->memory->memory[i]);
  }
  // printf("\n");

  for (int i = 0; i < machine->num_instructions; i++) {

    Instruction *intsr = machine->instructions[i];

    char key[256];
    char *value = malloc(256);
    if (!value) {
      continue;
    }

    encode_key(key, 256, intsr->state, intsr->value);
    encode_value(value, 256, intsr->write, intsr->dir, intsr->new_state);
    // printf("Key: '%s'\n", key);
    // printf("Value: '%s'\n", value);

    const char *r = insert_value(machine->instructions_table, key, value);

    if (r == NULL) {
      free(value);
      printf("Failed to insert key into hashmap\n");
      free_machine(machine);
      free_memory(machine->memory);
      free_instructions(machine->instructions, machine->num_instructions);
      // free_memory(start_memory);
      free_table(machine->instructions_table);
      return 0;
    };

    // printf("------------------\n");
    // printf("Instruction:\n");
    // printf("State: %s\n", intsr->state);
    // printf("Value: %d\n", intsr->value);
    // printf("Write: %d\n", intsr->write);
    // printf("Dir: %s\n", intsr->dir);
    // printf("New State: %s\n", intsr->new_state);
    // printf("------------------\n");
    // printf("\n");
  };

  int count = 0;
  int hault = 0;
  printf("State: %s | Starting...\n", machine->state);
  while (!hault) {
    draw(machine);
    if (count > 1000000) {
      break;
    }
    int *cell = &machine->memory->memory[machine->cell];

    char key[256];
    encode_key(key, 256, machine->state, *cell);

    // printf("Getting key: '%s'\n", key);
    char *instruction = get_key_value(machine->instructions_table, key);

    if (instruction == NULL) {
      // printf("No instruction found\n");
      hault = 1;
      count++;
      continue;
    }
    // printf("Found instruction: %s for key: %s\n", instruction, key);

    hault = execute_instruction(instruction, machine, machine->memory);
    count++;
    sleep(1);
  }

  // printf("\nENDING MEMORY:\n");
  // for (int i = 0; i < machine->memory->len; i++) {
  //   char *val = print_cell_value(&machine->memory->memory[i]);

  //  printf("%s", val);
  //}

  printf("\nRan '%s'\n", argv[1]);

  free_memory(machine->memory);
  free_instructions(machine->instructions, machine->num_instructions);
  free_table(machine->instructions_table);
  free_machine(machine);
}

void test_hashmap(HashTable *table) {
  if (table == NULL) {
    printf("Failed to create hashtable\n");
    free_table(table);
    return;
  }

  int key_len = 5;
  for (int i = 0; i < 100; i++) {
    int *value_ptr = malloc(sizeof(int));
    if (value_ptr == NULL) {
      continue;
    }
    *value_ptr = i;
    char *key = malloc(key_len + 1);

    if (key == NULL) {
      printf("Failed to alloc key\n");
      continue;
    }

    for (int j = 0; j < key_len; j++) {
      int num = rand() % 26;
      char letter = 'A' + num; // after ascii 65
      key[j] = letter;
    };

    key[key_len] = '\0';
    printf("%s\n", key);

    const char *r = insert_value(table, key, value_ptr);

    if (r == NULL) {
      printf("Failed to insert key into hashmap\n");
      free(key);
      free_table(table);
      return;
    };

    free(key);
  }
  int *v = get_key_value(table, "HMNMY");

  if (v == NULL) {
    printf("Value not found\n");
    free_table(table);
    return;
  };

  printf("Value: %d\n", *v);

  free_table(table);
}
