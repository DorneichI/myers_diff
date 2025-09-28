#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lines.h"
#include "myers.h"

void free_trace(TraceResult *result) {
  if (!result)
    return;
  if (result->trace) {
    for (int i = 0; i <= result->size; i++) {
      free(result->trace[i]);
    }
    free(result->trace);
  }
  free(result);
}

void cleanup_diff(int *v, int **trace, int trace_size) {
  if (v)
    free(v);
  if (trace) {
    for (int i = 0; i < trace_size; i++) {
      if (trace[i])
        free(trace[i]);
    }
    free(trace);
  }
}

TraceResult *shortest_edit(Line *lines_a, Line *lines_b, int size_a,
                           int size_b) {
  int n = size_a;
  int m = size_b;
  int max_edits = n + m;

  int *v = malloc((2 * max_edits + 1) * sizeof(int));
  int **trace = malloc((max_edits + 1) * sizeof(int *));
  if (!v || !trace) {
    return NULL;
  }

  v[max_edits] = 0;

  for (int d = 0; d <= max_edits; d++) {
    for (int k = -d; k <= d; k += 2) {
      int x, y;
      if (k == -d || (k != d && v[k - 1 + max_edits] < v[k + 1 + max_edits])) {
        x = v[k + 1 + max_edits];
      } else {
        x = v[k - 1 + max_edits] + 1;
      };
      y = x - k;

      while (x < n && y < m &&
             !strcmp(line_at(lines_a, x), line_at(lines_b, y))) {
        x++;
        y++;
      };
      v[k + max_edits] = x;

      if (x >= n && y >= m) {
        int *v_cpy = malloc((2 * max_edits + 1) * sizeof(int));
        if (!v_cpy) {
          cleanup_diff(v, trace, d);
          return NULL;
        }
        memcpy(v_cpy, v, (2 * max_edits + 1) * sizeof(int));
        trace[d] = v_cpy;
        cleanup_diff(v, NULL, 0);
        TraceResult *result = malloc(sizeof(TraceResult));
        result->size = d;
        result->trace = trace;
        return result;
      }
    }
    int *v_cpy = malloc((2 * max_edits + 1) * sizeof(int));
    if (!v_cpy) {
      cleanup_diff(v, trace, d);
      return NULL;
    }
    memcpy(v_cpy, v, (2 * max_edits + 1) * sizeof(int));
    trace[d] = v_cpy;
  }
  cleanup_diff(v, trace, max_edits + 1);
  return NULL;
}

void free_track(TrackResult *result) {
  if (!result)
    return;
  free(result->track);
  free(result);
}

TrackResult *backtrack(int size_a, int size_b, TraceResult *trace) {
  int lengthof_route = 1;
  int *route = malloc((size_a + size_b + 1) * 2 * sizeof(int));
  int x = size_a;
  int y = size_b;
  int offset = x + y;

  for (int i = trace->size; i >= 0; i--) {
    int k = x - y;
    int prev_k;
    if (k == -i || (k != i && trace->trace[i][k - 1 + offset] <
                                  trace->trace[i][k + 1 + offset]))
      prev_k = k + 1;
    else
      prev_k = k - 1;
    int prev_x = trace->trace[i][prev_k + offset];
    int prev_y = prev_x - prev_k;

    while (x > prev_x && y > prev_y) {
      route[lengthof_route * 2 - 2] = x;
      route[lengthof_route * 2 - 1] = y;
      lengthof_route++;
      x = x - 1;
      y = y - 1;
    }

    if (i > 0) {
      route[lengthof_route * 2 - 2] = x;
      route[lengthof_route * 2 - 1] = y;
      lengthof_route++;
    }

    x = prev_x;
    y = prev_y;
  }

  TrackResult *result = malloc(sizeof(TrackResult));
  result->size = lengthof_route;
  result->track = route;
  return result;
}