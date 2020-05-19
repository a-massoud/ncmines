#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <time.h>
#include "board.h"

enum difficultylevel {DIFFICULTY_EASY, DIFFICULTY_MEDIUM, DIFFICULTY_HARD};

struct gamestate_t {
    long x, y;
    long mines_found;
    struct board_t *b;
    time_t stime;
    time_t gtime;
    char started;
};

struct gamestate_t *gamestate_new(enum difficultylevel d);
void gamestate_update(struct gamestate_t *gs, char *should_drawgame,
        char *should_drawheader, char *endgame);
void gamestate_del(struct gamestate_t *gs);

#endif
