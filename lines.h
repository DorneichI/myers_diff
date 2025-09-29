#ifndef LINES
#define LINES

typedef struct {
  char **data;
  int size;
} Lines;

Lines *file_to_lines(const char *file_path);

void free_lines(Lines *lines);

void print_lines(Lines *lines);

#endif