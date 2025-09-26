#include <stdlib.h>
#include <string.h>

#include "lines.h"

int shortest_edit(char *file1, char *file2) {
  Line *lines_a = file_to_lines(file1);
  Line *lines_b = file_to_lines(file2);

  int m = count_lines(lines_a, 0);
  int n = count_lines(lines_b, 0);
  int max_edits = m + n;

  int *v = malloc((2 * max_edits + 1) * sizeof(int));

  v[1] = 0;

  for (int d = 0; d <= max_edits; d++) {
    for (int k = -d; k <= d; k++) {
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

      v[k] = x;

      if (x >= n && y >= m)
        return d;
    }
  }
  return -1;
}
