#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "board.h"
#include "gamestate.h"
#include "draw.h"

int main(int argc, char **argv) {
    srand(time(NULL));
    if(argc < 2) {
        exit_with_error(1, "Usage: %s <easy|medium|hard>", argv[0]);
    }
    enum difficultylevel d;
    for(int i = 0; i < strlen(argv[1]); i++)
        if(argv[1][i] >= 'A' && argv[1][i] <= 'Z')
            argv[1][i] = argv[1][i] + 'a' - 'A';
    if(!strcmp(argv[1], "easy"))
        d = DIFFICULTY_EASY;
    else if(!strcmp(argv[1], "medium"))
        d = DIFFICULTY_MEDIUM;
    else if(!strcmp(argv[1], "hard"))
        d = DIFFICULTY_HARD;

    struct gamestate_t *gs;
    gs = gamestate_new(d);
    init(gs->b->w, gs->b->h + 1);

    char should_drawgame, should_drawheader, endgame;
    drawheader(gs);
    drawgame(gs);
    while(1) {
        gamestate_update(gs, &should_drawgame, &should_drawheader, &endgame);
        if(endgame == 1) {
            drawend(gs, "You win!");
            exit(0);
        } else if(endgame == 2) {
            drawend(gs, "You lose!");
            exit(0);
        }

        if(should_drawgame)
            drawgame(gs);
        if(should_drawheader)
            drawheader(gs);
    }

    return 0;
}
