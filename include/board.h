#ifndef BOARD_H_
#define BOARD_H_

#define CELL_EMPTY 0
#define CELL_MINE 9

/* Type for each cell of board */
struct board_cell_t {
    char val;
    char uncovered;
    char flag;
};

/* Type for board itself, so more than one can be made and it can be saved */
struct board_t {
    long w, h;
    int nmines;
    struct board_cell_t *b;
};

/* New board with given size */
struct board_t *board_new(long w, long h, long sx, long sy, int nmines);
/* Uncover specific location on board and uncover neighbors of it
 * Returns 0 on uncovering cell and neighbors, 1 on cell not being uncoverable,
 * 2 on cell being mine */
int board_uncover(struct board_t *board, long x, long y);
/* Destructor */
void board_del(struct board_t *board);

#endif
