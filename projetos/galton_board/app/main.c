#include "pico/stdlib.h"
#include "galton.h"
#include "hal_display.h"

static struct repeating_timer timer;

bool tick_callback(struct repeating_timer *t) {
    galton_tick();
    return true;
}

int main() {
    stdio_init_all();
    display_init();
    galton_init();

    // Inicia temporizador com 50ms (20 FPS)
    add_repeating_timer_ms(50, tick_callback, NULL, &timer);

    while (1) {
        tight_loop_contents(); // Idle loop
    }

    return 0;
}
