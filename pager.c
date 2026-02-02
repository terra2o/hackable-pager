/* pager.c */
/* hackable-pager by terra2o */

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "pager.h"

#define GUTTER_WIDTH 6

char **lines;
int line_count;
int top_line; // first visible line

static const char *c_keywords[] = 
{
    "int","char","return","if","else","for","while",
    "void","struct","break","continue","typedef",
    "static","const","unsigned","signed","sizeof", NULL
};

void load_file(FILE *f) 
{
    size_t len = 0;
    char *line = NULL;

    while (getline(&line, &len, f) != -1) 
    {
        lines = realloc(lines, sizeof(char*) * (line_count + 1));

        lines[line_count++] = strdup(line);
    }
    free(line);
}

void render(int top_line, int rows) 
{
    erase();

    for (int i = 0; i < rows && top_line + i < line_count; i++) 
    {
        move(i, 0);

        /* ========== LINE NUMBERS ==========*/
        attron(COLOR_PAIR(1));
        mvprintw(i, 0, "%*d |", GUTTER_WIDTH - 2, top_line + i + 1);
        attroff(COLOR_PAIR(1));

        /* ========== ACTUALLY PRINTING LINES ==========*/
        print_c_line(lines[top_line + i]);
    }

    refresh();
}


int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    load_file(f);

    fclose(f);

    initscr();
    noecho();
    keypad(stdscr, TRUE);

    /* ==== Controls =====*/

    if (!f) 
    {
        perror("fopen");
        return 1;
    }

    if (!has_colors()) 
    {
        endwin();
        fprintf(stderr, "Terminal does not support colors\n");
        exit(1);
    }

    /* ================= */

    start_color();
    init_pair(1, COLOR_BLUE,   COLOR_BLACK); // line numbers / keywords
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // strings
    init_pair(3, COLOR_YELLOW,COLOR_BLACK); // comments

    

    int row, col;
    getmaxyx(stdscr, row, col);

    int top_line = 0;
    int ch;

    render(top_line, row);

    while ((ch = getch()) != 'q') 
    {
        if (ch == KEY_DOWN && top_line < line_count - 1)
            top_line++;
        else if (ch == 'j' && top_line < line_count - 1)
            top_line++;
        else if (ch == KEY_UP && top_line > 0)
            top_line--;
        else if (ch == 'k' && top_line > 0)
            top_line--;

        render(top_line, row);
    }

    endwin();
}

/* ========== HELPERS ==========*/

int is_keyword(const char *word)
{
    for (int i = 0; c_keywords[i]; i++)
        if (strcmp(word, c_keywords[i]) == 0)
            return 1;
    return 0;
}

void print_c_line(const char *line)
{
    int i = 0;

    while (line[i])
    {
        /* COMMENT */
        if (line[i] == '/' && line[i+1] == '/')
        {
            attron(COLOR_PAIR(3));
            printw("%s", &line[i]);
            attroff(COLOR_PAIR(3));
            return;
        }

        /* STRING */
        if (line[i] == '"')
        {
            attron(COLOR_PAIR(2));
            printw("\"");
            i++;
            while (line[i] && line[i] != '"')
            {
                printw("%c", line[i++]);
            }
            if (line[i] == '"')
                printw("\"");
            attroff(COLOR_PAIR(2));
            i++;
            continue;
        }

        /* KEYWORD */
        if (isalpha((unsigned char)line[i]) || line[i] == '_')
        {
            char word[64];
            int j = 0;

            while ((isalnum((unsigned char)line[i]) || line[i] == '_') && j < 63)
                word[j++] = line[i++];

            word[j] = '\0';

            if (is_keyword(word))
                attron(COLOR_PAIR(1));

            printw("%s", word);

            if (is_keyword(word))
                attroff(COLOR_PAIR(1));

            continue;
        }

        /* NORMAL CHAR */
        printw("%c", line[i++]);
    }
}
