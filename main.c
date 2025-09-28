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

  TraceResult *trace = shortest_edit(lines_a, lines_b, size_a, size_b);

  printf("shortest edit is: %i\n", trace->size);

  TrackResult *track = backtrack(size_a, size_b, trace);

  for (int i = 0; i < track->size * 2 - 1; i += 2) {
    printf("x: %i, y: %i\n", track->track[i], track->track[i + 1]);
  }

  free_lines(lines_a);
  free_lines(lines_b);
  free_trace(trace);
  free_track(track);
}