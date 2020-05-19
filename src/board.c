#include <stdlib.h>
#include "draw.h"
#include "board.h"

struct board_t *board_new(long w, long h, long sx, long sy, int nmines) {
    struct board_t *board = malloc(sizeof(struct board_t));
    if(!board) {
        exit_with_error(1, "Failed to allocate board");
    }
    board->w = w;
    board->h = h;
    board->nmines = nmines;

    board->b = malloc(sizeof(struct board_cell_t) * w * h);
    if(!board->b) {
        free(board);
        exit_with_error(1, "Failed to allocate cells");
    }

    long x, y;
    for(x = 0; x < w; x++) {
        for(y = 0; y < h; y++) {
            board->b[x + y * w].val = CELL_EMPTY;
            board->b[x + y * w].uncovered = 0;
            board->b[x + y * w].flag = 0;
        }
    }

    int i, j, k;
    for(i = 0; i < nmines; i++) {
        do {
            x = rand() % (int)w;
            y = rand() % (int)h;
        } while(board->b[x + y * w].val == 9 ||
               ((x - sx >= -1 && x - sx <= 1) &&
                (y - sy >= -1 && y - sy <= 1)));
        board->b[x + y * w].val = 9;
        for(j = -1; j < 2; j++) {
            for(k = -1; k < 2; k++) {
                if(j == 0 && k == 0) continue;
                if(x + j < 0 || x + j >= w || y + k < 0 || y + k >= h)
                    continue;
                if(board->b[(x + j) + (y + k) * w].val == CELL_MINE) continue;
                board->b[(x + j) + (y + k) * w].val++;
            }
        }
    }

    return board;
}

int board_uncover(struct board_t *board, long x, long y) {
    if(x < 0 || x >= board->w || y < 0 || y >= board->h) return 1;
    if(board->b[x + y * board->w].uncovered) return 1;
    if(board->b[x + y * board->w].flag) return 1;

    board->b[x + y * board->w].uncovered = 1;
    if(board->b[x + y * board->w].val == CELL_MINE) return 2;
    if(board->b[x + y * board->w].val != 0) return 0;

    int j, k;
    for(j = -1; j <= 1; j++) {
        for(k = -1; k <= 1; k++) {
            if(j == 0 && k == 0) continue;
            board_uncover(board, x + j, y + k);
        }
    }

    return 0;
}

void board_del(struct board_t *board) {
    free(board->b);
    free(board);
}
