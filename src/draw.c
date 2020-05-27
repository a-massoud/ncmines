#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ncurses.h>
#include "gamestate.h"
#include "draw.h"

WINDOW *gamewin = NULL;
WINDOW *titlewin = NULL;

char use_color = 0;
char should_endwin = 0;

void _endwin_wrapper(void) {
    if(gamewin) delwin(gamewin);
    if(titlewin) delwin(titlewin);
    if(should_endwin) endwin();
    should_endwin = 0;
}

void exit_with_error(int c, const char *format, ...) {
    va_list args;
    va_start(args, format);

    _endwin_wrapper();
    fputs("ERROR: ", stderr);
    vfprintf(stderr, format, args);
    fputc('\n', stderr);
    va_end(args);

    exit(c);
}

void init(long w, long h) {
    if(!initscr()) {
        exit_with_error(1, "Failed to start ncurses");
    }
    atexit(_endwin_wrapper);
    should_endwin = 1;
    if(COLS < w || LINES < h) {
        exit_with_error(1, "Terminal not large enough");
    }
    curs_set(2);
    noecho();
    cbreak();

    gamewin = newwin(h - 1, w, 1, 0);
    titlewin = newwin(1, w, 0, 0);
    if(!gamewin || !titlewin) {
        exit_with_error(1, "Failed to initialize windows");
    }
    nodelay(gamewin, TRUE);
    keypad(gamewin, TRUE);

    if(has_colors()) {
        use_color = 1;
        start_color();
        init_pair(FLAG_COLORS, COLOR_CYAN, COLOR_BLACK);
        init_pair(MINE_COLORS, COLOR_RED, COLOR_RED);
        init_pair(WARN_COLORS, COLOR_BLACK, COLOR_YELLOW);
    }
}

void drawgame(struct gamestate_t *gs) {
    long i, j;
    struct board_cell_t ccell;
    werase(gamewin);
    if(gs->b->b == NULL) {
        for(i = 0; i < gs->b->h; i++) {
            for(j = 0; j < gs->b->w; j++) {
                mvwaddch(gamewin, i, j, '*');
            }
        }
    } else {
        for(i = 0; i < gs->b->h; i++) {
            for(j = 0; j < gs->b->w; j++) {
                ccell = gs->b->b[j + i * gs->b->w];
                if(ccell.uncovered) {
                    switch(ccell.val) {
                        case CELL_EMPTY:
                            break;
                        case CELL_MINE:
                            mvwaddch(gamewin, i, j, 'M' |
                                    (use_color ? COLOR_PAIR(MINE_COLORS)
                                     : A_STANDOUT));
                            break;
                        default:
                            mvwaddch(gamewin, i, j, (ccell.val + '0') |
                                    (use_color ? COLOR_PAIR(WARN_COLORS)
                                     : A_REVERSE));
                    }
                } else if(ccell.flag != 0) {
                    mvwaddch(gamewin, i, j, (ccell.flag == 1 ? 'F' : '?') |
                            (use_color ? COLOR_PAIR(FLAG_COLORS)
                             : A_BOLD));
                } else {
                    mvwaddch(gamewin, i, j, '*');
                }
            }
        }
    }
    wmove(gamewin, gs->y, gs->x);
    wrefresh(gamewin);
}

void drawheader(struct gamestate_t *gs) {
    static char minestr[8];
    werase(titlewin);
    mvwprintw(titlewin, 0, 0, "%d:%02d", (int)gs->gtime / 60, (int)gs->gtime % 60);
    snprintf(minestr, 8, "%d/%d", (int)gs->mines_found, (int)gs->b->nmines);
    mvwprintw(titlewin, 0, gs->b->w - strlen(minestr), minestr);
    wmove(gamewin, gs->y, gs->x);
    wrefresh(titlewin);
}

void drawend(struct gamestate_t *gs, char *msg) {
    endwin();
    should_endwin = 0;
    printf("%s\nTime: %d:%02d\n", msg, (int)gs->gtime / 60, (int)gs->gtime % 60);
}
