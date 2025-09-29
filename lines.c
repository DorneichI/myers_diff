#include <stdio.h>
#include <stdlib.h>

#include "lines.h"

void free_line_data(char **data, int size) {
  for (int i = 0; i < size; i++) {
    free(data[i]);
  }
}

Lines *file_to_lines(const char *file_path) {

  FILE *fp = fopen(file_path, "r");
  if (!fp)
    return NULL;

  int size = 16;
  char **data = malloc(size * sizeof(char *));

  int line_number = 0;
  int c;

  while (1) {
    if (line_number >= size) {
      size *= 2;
      char **new_data = realloc(data, size * sizeof(char *));
      if (!new_data) {
        free_line_data(data, line_number);
        free(data);
        return NULL;
      }
      data = new_data;
    }

    int string_size = 16;
    char *line = malloc(string_size);
    if (!line) {
      free_line_data(data, line_number);
      free(data);
      return NULL;
    }
    int line_length = 0;
    while ((c = fgetc(fp)) != EOF && c != '\n') {
      if (line_length == string_size - 1) {
        string_size *= 2;
        char *new_line = realloc(line, string_size);
        if (!new_line) {
          free(line);
          free_line_data(data, line_number);
          free(data);
          return NULL;
        }
        line = new_line;
      }
      line[line_length++] = (char)c;
    }

    line[line_length] = '\0';

    data[line_number++] = line;
    if (c == EOF) {
      break;
    }
  }

  char **new_data = realloc(data, line_number * sizeof(char *));
  if (!new_data) {
    free(data);
    return NULL;
  }
  data = new_data;

  fclose(fp);
  Lines *lines = malloc(sizeof(Lines));
  lines->data = data;
  lines->size = line_number;
  return lines;
}

void free_lines(Lines *lines) {
  if (!lines)
    return;
  free_line_data(lines->data, lines->size);
  free(lines);
}

void print_lines(Lines *lines) {
  if (!lines)
    return;
  for (int i = 0; i < lines->size; i++) {
    printf("%i %s\n", i + 1, lines->data[i]);
  }
}