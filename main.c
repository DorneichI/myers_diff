#include <stdio.h>
#include <stdlib.h>

#include "lines.h"
#include "myers.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("argument count must be 2, given: %i)\n", argc - 1);
    return -1;
  }
  char *file1 = argv[1];
  char *file2 = argv[2];

  Lines *lines_a = file_to_lines(file1);
  Lines *lines_b = file_to_lines(file2);

  traces *trace = shortest_edit(lines_a, lines_b);

  printf("shortest edit is: %i\n", trace->size);

  tracks *track = backtrack(lines_a->size, lines_b->size, trace);

  diffs *line_diff_result = create_diffs(track, lines_a, lines_b);

  print_diff(line_diff_result);

  free_lines(lines_a);
  free_lines(lines_b);
  free_trace(trace);
  free_track(track);
  free_diffs(line_diff_result);
}