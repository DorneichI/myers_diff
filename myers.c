#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lines.h"
#include "myers.h"

void free_trace(traces *trace) {
  if (!trace)
    return;
  if (trace->data) {
    for (int i = 0; i <= trace->size; i++) {
      free(trace->data[i]);
    }
    free(trace->data);
  }
  free(trace);
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

traces *shortest_edit(Lines *lines_a, Lines *lines_b) {
  int n = lines_a->size;
  int m = lines_b->size;
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

      while (x < n && y < m && !strcmp(lines_a->data[x], lines_b->data[y])) {
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
        traces *result = malloc(sizeof(traces));
        result->size = d;
        result->data = trace;
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

void free_track(tracks *track) {
  if (!track)
    return;
  free(track->data);
  free(track);
}

tracks *backtrack(int size_a, int size_b, traces *trace) {
  int lengthof_route = 1;
  int *route = malloc((size_a + size_b + 1) * 2 * sizeof(int));
  int x = size_a;
  int y = size_b;
  int offset = x + y;

  for (int i = trace->size; i >= 0; i--) {
    int k = x - y;
    int prev_k;
    if (k == -i || (k != i && trace->data[i][k - 1 + offset] <
                                  trace->data[i][k + 1 + offset]))
      prev_k = k + 1;
    else
      prev_k = k - 1;
    int prev_x = trace->data[i][prev_k + offset];
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

  tracks *result = malloc(sizeof(tracks));
  result->size = lengthof_route;
  result->data = route;
  return result;
}

void free_diffs(diffs *diff) {
  if (!diff)
    return;
  free(diff->data);
  free(diff);
}

diffs *create_diffs(tracks *track, Lines *lines_a, Lines *lines_b) {
  diff *data = malloc((track->size - 1) * sizeof(diff));

  for (int i = 0; i < track->size * 2 - 3; i += 2) {
    char *line_a = lines_a->data[track->data[i + 2]];
    char *line_b = lines_b->data[track->data[i + 3]];
    if (track->data[i] == track->data[i + 2]) {
      data[track->size - i / 2 - 2].action = INSERT;
      data[track->size - i / 2 - 2].line_a = NULL;
      data[track->size - i / 2 - 2].line_b = line_b;
    } else if (track->data[i + 1] == track->data[i + 3]) {
      data[track->size - i / 2 - 2].action = DELETE;
      data[track->size - i / 2 - 2].line_a = line_a;
      data[track->size - i / 2 - 2].line_b = NULL;
    } else {
      data[track->size - i / 2 - 2].action = EQUAL;
      data[track->size - i / 2 - 2].line_a = line_a;
      data[track->size - i / 2 - 2].line_b = line_b;
    }
  }
  diffs *result = malloc(sizeof(diffs));
  result->data = data;
  result->size = track->size - 1;
  return result;
}

const char *COLORS[ACTION_COUNT] = {
    [INSERT] = "\e[32m",
    [DELETE] = "\e[31m",
    [EQUAL] = "\e[39m",
};

void print_diff(diffs *data) {
  for (int i = 0; i < data->size; i++) {
    if (data->data[i].action == INSERT) {
      printf("%s+ %s\n", COLORS[INSERT], data->data[i].line_b);
    } else if (data->data[i].action == DELETE) {
      printf("%s- %s\n", COLORS[DELETE], data->data[i].line_a);
    } else {
      printf("%s  %s\n", COLORS[EQUAL], data->data[i].line_a);
    }
    printf("%s", COLORS[EQUAL]);
  }
}