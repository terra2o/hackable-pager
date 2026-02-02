/* pager.h */

#ifndef PAGER_H
#define PAGER_H

#include <stdio.h>

/* globals */
extern char **lines;
extern int line_count;
extern int top_line;

/* rendering */
void render(int top_line, int rows);

/* file */
void load_file(FILE *f);

/* syntax highlighting helpers */
int  is_keyword(const char *word);
void print_c_line(const char *line);

#endif /* PAGER_H */
