#ifndef MYERS
#define MYERS

typedef struct {
  int **trace;
  int size;
} TraceResult;

void free_trace(TraceResult *result);

TraceResult *shortest_edit(Line *lines_a, Line *lines_b, int size_a,
                           int size_b);

typedef struct {
  int *track;
  int size;
} TrackResult;

void free_track(TrackResult *result);

TrackResult *backtrack(int size_a, int size_b, TraceResult *trace);

#endif