#include "main.h"
#include <assert.h>
#include <stdio.h>

void clear_terminal() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}
void clear_terminal_ansi() {
  // Escape sequence: \e[1;1H moves cursor to top-left, \e[2J clears the screen
  printf("\e[1;1H\e[2J");
}

void draw(Machine *machine) {
  clear_terminal_ansi();
  printf("\nTuring Machine\n");
  const int height = 3;
  const int len = machine->memory->len * height + 1;

  // for (int i = 0; i < machine->memory->len; i++) {
  //   char *val = print_cell_value(&machine->memory->memory[i]);

  //  printf("%s", val);
  //}
  printf("\n");
  printf("%s\n\n", machine->state);
  assert(height % 2 != 0);

  // for (int col = 0; col < len; col++) {

  //  int index = (col) % (height);

  //  if ((col) % height == 0 && ((col) / height == machine->cell)) {
  //    printf("↓");
  //  } else {

  //    printf(" ");
  //  }
  //}
  // for (int col = 0; col < len; col++) {
  //  if (col == machine->cell) {
  //    printf("↓");
  //  } else {
  //    printf(" ");
  //
  //  }
  //}

  for (int col = 0; col < machine->memory->len; col++) {
    for (int h = 0; h < height; h++) {
      if (col == machine->cell && h == height / 2 + 1) {
        printf("↓"); // Center the arrow
      } else {
        printf(" ");
      }
    }
    printf(" "); // Space for the separator column
  }

  printf("\n");

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < len; col++) {
      if (col == 0 || col % height == 0) {
        if (row == 0) {
          printf("┬");
        } else if (row % (height - 1) == 0) {
          printf("┴");
        } else {
          printf("│");
        }
      }

      int center = (height / 2) + 1;
      int relative = col / height;
      if ((col - 1) % height == 0 && (row + 1) % center == 0) {
        int index = (col - 1) / height;

        char *val = print_cell_value(&machine->memory->memory[index]);
        printf("%s", val);
      } else if (row == 0 || row % (height - 1) == 0) {
        printf("─");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
};
