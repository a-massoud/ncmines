#include <stdio.h>
#include <ncurses.h>

int main(int argc, char **argv) {
    if(!initscr()) {
        fprintf(stderr, "Failed to initialize ncurses\n");
        return 1;
    }

    erase();
    move(0, 0);
    printw("Hello, world!");
    getch();

    endwin();
    return 0;
}
