#include <stdio.h>
#include "pico/stdlib.h"
#include "galton_board.h"

int main() {
    stdio_init_all();
    galton_board_run();
    return 0;
}
