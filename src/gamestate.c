#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "board.h"
#include "draw.h"
#include "gamestate.h"

struct gamestate_t *gamestate_new(enum difficultylevel d) {
    struct gamestate_t *gs;
    gs = malloc(sizeof(struct gamestate_t));
    if(!gs)
        exit_with_error(1, "Failed to allocate gamestate");

    gs->x = 0;
    gs->y = 0;
    gs->mines_found = 0;
    gs->stime = 0;
    gs->gtime = 0;
    gs->started = 0;
    gs->b = malloc(sizeof(struct board_t));
    if(!gs->b) {
        free(gs);
        exit_with_error(1, "Failed to allocate dummy board");
    }
    gs->b->b = NULL;
    switch(d) {
        case DIFFICULTY_EASY:
            gs->b->w = 10;
            gs->b->h = 10;
            gs->b->nmines = 15;
            break;
        case DIFFICULTY_MEDIUM:
            gs->b->w = 16;
            gs->b->h = 16;
            gs->b->nmines = 40;
            break;
        case DIFFICULTY_HARD:
            gs->b->w = 30;
            gs->b->h = 16;
            gs->b->nmines = 99;
            break;
    }

    return gs;
}

void gamestate_update(struct gamestate_t *gs, char *should_drawgame,
        char *should_drawheader, char *endgame) {
    *endgame = 0;
    *should_drawgame = 0;
    *should_drawheader = 0;
    struct board_cell_t *ccell;
    int c;
    c = wgetch(gamewin);
    if(c != ERR) {
        switch(c) {
            case 'k':
            case KEY_UP:
                if(gs->y - 1 >= 0) {
                    gs->y--;
                    wmove(gamewin, gs->y, gs->x);
                    wrefresh(gamewin);
                }
                break;
            case 'l':
            case KEY_RIGHT:
                if(gs->x + 1 < gs->b->w) {
                    gs->x++;
                    wmove(gamewin, gs->y, gs->x);
                    wrefresh(gamewin);
                }
                break;
            case 'j':
            case KEY_DOWN:
                if(gs->y + 1 < gs->b->h) {
                    gs->y++;
                    wmove(gamewin, gs->y, gs->x);
                    wrefresh(gamewin);
                }
                break;
            case 'h':
            case KEY_LEFT:
                if(gs->x - 1 >= 0) {
                    gs->x--;
                    wmove(gamewin, gs->y, gs->x);
                    wrefresh(gamewin);
                }
                break;
            case 'q':
                exit(0);
                break;
            case 'f':
                if(gs->started) {
                    ccell = gs->b->b + gs->x + gs->y * gs->b->w;
                    if(ccell->flag == 0) {
                        ccell->flag = 1;
                        gs->mines_found++;
                        *should_drawheader = 1;
                    } else if(ccell->flag == 1) {
                        ccell->flag = 2;
                        gs->mines_found--;
                        *should_drawheader = 1;
                    } else if(ccell->flag == 2) {
                        ccell->flag = 0;
                    }
                    *should_drawgame = 1;
                }
                break;
            case ' ':
            case '\n':
                if(!gs->started) {
                    long w = gs->b->w, h = gs->b->h;
                    int nmines = gs->b->nmines;
                    gs->b = board_new(w, h, gs->x, gs->y, nmines);
                    gs->stime = time(NULL);
                    gs->started = 1;
                }
                if(board_uncover(gs->b, gs->x, gs->y) == 2) {
                    *endgame = 2;
                } else {
                    *should_drawgame = 1;
                }
                break;
        }

        /* Check if win state */
        if(gs->mines_found == gs->b->nmines) {
            char win = 1;
            long i;
            for(i = 0; i < gs->b->w * gs->b->h; i++) {
                if(gs->b->b[i].val != CELL_MINE && gs->b->b[i].flag) {
                    win = 0;
                    break;
                }
            }
            if(win)
                *endgame = 1;
        }
    }

    if(gs->stime != 0) {
        time_t curtime = time(NULL);
        if(curtime - gs->stime > gs->gtime) {
            gs->gtime = curtime - gs->stime;
            *should_drawheader = 1;
        }
    }
}

void gamestate_del(struct gamestate_t *gs) {
    if(gs->b) board_del(gs->b);
    free(gs);
}
