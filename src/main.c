#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "board.h"

void _endwin_wrapper() { endwin(); }

int main(int argc, char **argv) {
    srand(time(NULL));
    if(!initscr()) {
        fprintf(stderr, "Failed to initialize ncurses\n");
        return 1;
    }
    curs_set(0);
    atexit(_endwin_wrapper);

    struct board_t *board = board_new(30, 16, 5, 5, 99);
    if(!board) {
        return 1;
    }

    board_uncover(board, 5, 5);

    erase();
    for(size_t i = 0; i < board->h; i++) {
        for(size_t j = 0; j < board->w; j++) {
            if(board->b[j + i * board->w].uncovered) {
                if(board->b[j + i * board->w].val == CELL_EMPTY) continue;
                if(board->b[j + i * board->w].val == CELL_MINE) {
                    mvaddch(i, j, 'M');
                    continue;
                }
                mvprintw(i, j, "%d", board->b[j + i * board->w].val);
            } else {
                mvaddch(i, j, '*');
            }
        }
    }
    refresh();
    getch();

    board_del(board);
    return 0;
}
