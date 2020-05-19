#ifndef DRAW_H_
#define DRAW_H_

#include <ncurses.h>
#include "gamestate.h"

#define FLAG_COLORS 1
#define MINE_COLORS 2
#define WARN_COLORS 3

extern WINDOW *gamewin;
extern WINDOW *titlewin;

extern char use_color;
extern char should_endwin;

void exit_with_error(int c, const char *format, ...);
void init(long w, long h);
void drawgame(struct gamestate_t *gs);
void drawheader(struct gamestate_t *gs);
void drawend(struct gamestate_t *gs, char *msg);

#endif
