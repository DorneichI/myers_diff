#ifndef MYERS
#define MYERS

typedef struct {
  int **data;
  int size;
} traces;

void free_trace(traces *trace);

traces *shortest_edit(Line *lines_a, Line *lines_b, int size_a, int size_b);

typedef struct {
  int *data;
  int size;
} tracks;

void free_track(tracks *track);

tracks *backtrack(int size_a, int size_b, traces *trace);

enum action { INSERT, DELETE, EQUAL, ACTION_COUNT };

typedef struct {
  enum action action;
  char *line_a;
  char *line_b;
} diff;

typedef struct {
  diff *data;
  int size;
} diffs;

diffs *create_diffs(tracks *track, Line *lines_a, Line *lines_b);

void print_diff(diffs *data);

#endif