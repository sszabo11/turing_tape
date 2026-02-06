#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define _POSIX_C_SOURCE 200809L
#include <string.h>

#define MAX_INSTRUCTIONS 300

typedef struct {
  int state;
  int value;
  int write;
  int dir;
  int new_state;
} Instruction;

int main() {

  FILE *file_ptr = fopen("source.tring", "r");

  Instruction *instructions[MAX_INSTRUCTIONS] = {0};
  int instr_count = 0;

  int i = 0;
  char line[300];

  while (fgets(line, sizeof(line), file_ptr) &&
         instr_count < MAX_INSTRUCTIONS) {

    Instruction *inst = malloc(sizeof(Instruction));

    if (!inst) {
      fprintf(stderr, "Memory allocation failed\n");
      break;
    }

    inst->dir = -1;
    inst->state = -1;
    inst->value = -1;
    inst->write = -1;
    inst->new_state = -1;
    char *token;
    char *rest = line;
    int col = 0;

    while ((token = strtok_r(rest, " \n", &rest)) != NULL) {
      printf("Token: %s\n", token);

      switch (col) {
      case 0:
        printf("d");
        inst->state = atoi(token);
        break;
      case 1:
        inst->value = atoi(token);
        break;
      case 2:
        inst->write = atoi(token);
        break;
      case 3:
        inst->dir = strcmp(token, ">") == 0   ? 1
                    : strcmp(token, "<") == 0 ? 0
                                              : -1;
        break;
      case 4:
        inst->new_state = atoi(token);
      default:
        break;
      }

      col++;
    };
    if (col < 4) {
      free(inst);
      continue;
    };

    instructions[instr_count++] = inst;
  };
  fclose(file_ptr);

  for (int i = 0; i < instr_count; i++) {
    printf("Instruction\n");
    printf("State: %d\n", instructions[i]->state);
    printf("Value: %d\n", instructions[i]->value);
    printf("Write: %d\n", instructions[i]->write);
    printf("Shift: %d\n", instructions[i]->dir);
    printf("New state: %d\n", instructions[i]->new_state);
    printf("\n");
  }

  for (int i = 0; i < instr_count; i++) {
    free(instructions[i]);
  }
}
