#include <stdio.h>

#include "myers.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("argument count must be 2, given: %i)\n", argc - 1);
    return -1;
  }
  char *file1 = argv[1];
  char *file2 = argv[2];

  int x = shortest_edit(file1, file2);

  printf("shortest edit is: %i\n", x);
}