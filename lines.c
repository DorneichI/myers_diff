#include <stdio.h>
#include <stdlib.h>

#include "lines.h"

Line *file_to_lines(const char *file_path) {

  FILE *fp = fopen(file_path, "r");
  if (!fp)
    return NULL;

  Line *lines = malloc(sizeof(Line));
  Line *zeroeth_line = lines;

  int line_number = 0;
  int c;

  while (1) {
    int string_size = 128;
    char *line = malloc(string_size);
    if (!line) {
      free_lines(lines);
      return NULL;
    }
    int line_length = 0;
    while ((c = fgetc(fp)) != EOF && c != '\n') {
      if (line_length == string_size - 1) {
        string_size *= 2;
        char *new_line = realloc(line, string_size);
        if (!new_line) {
          free(line);
          free_lines(lines);
          return NULL;
        }
        line = new_line;
      }
      line[line_length++] = (char)c;
    }

    line[line_length] = '\0';

    Line *new_line = malloc(sizeof(Line));
    new_line->line_number = ++line_number;
    new_line->line = line;
    lines->next = new_line;

    lines = new_line;

    if (c == EOF) {
      new_line->next = NULL;
      break;
    }
  }

  fclose(fp);
  Line *first_line = zeroeth_line->next;
  free(zeroeth_line);
  return first_line;
}

void free_lines(Line *lines) {
  if (!lines)
    return;
  free(lines->line);
  Line *next_line = lines->next;
  free(lines);
  free_lines(next_line);
}

void print_lines(Line *lines) {
  if (!lines)
    return;
  printf("%i %s\n", lines->line_number, lines->line);
  print_lines(lines->next);
}

int count_lines(Line *lines) {
  int count = 0;
  while (lines) {
    count++;
    lines = lines->next;
  }
  return count;
}

char *line_at(Line *lines, int index) {
  while (lines && index > 0) {
    lines = lines->next;
    index--;
  }
  return lines ? lines->line : NULL;
}