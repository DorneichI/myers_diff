#ifndef LINES
#define LINES

typedef struct Line {
  int line_number;
  char *line;
  struct Line *next;
} Line;

Line *file_to_lines(const char *file_path);

void free_lines(Line *lines);

void print_lines(Line *lines);

int count_lines(Line *lines);

char *line_at(Line *lines, int index);

#endif