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

  Line *lines_a = file_to_lines(file1);
  Line *lines_b = file_to_lines(file2);
  int size_a = count_lines(lines_a);
  int size_b = count_lines(lines_b);

  traces *trace = shortest_edit(lines_a, lines_b, size_a, size_b);

  printf("shortest edit is: %i\n", trace->size);

  tracks *track = backtrack(size_a, size_b, trace);

  for (int i = 0; i < track->size * 2 - 1; i += 2) {
    printf("x: %i, y: %i\n", track->data[i], track->data[i + 1]);
  }

  diffs *line_diff_result = create_diffs(track, lines_a, lines_b);
  diff *data = line_diff_result->data;

  for (int i = 0; i < line_diff_result->size; i++) {
    if (data[i].action == INSERT) {
      printf("INSERT\n");
      printf("%s\n", data[i].line_b);
    } else if (data[i].action == DELETE) {
      printf("DELETE\n");
      printf("%s\n", data[i].line_a);
    } else {
      printf("EQUAL\n");
      printf("%s\n", data[i].line_b);
    }
  }

  print_diff(line_diff_result);

  free_lines(lines_a);
  free_lines(lines_b);
  free_trace(trace);
  free_track(track);
}