// app/main.c
#include "pico/stdlib.h"
#include "hal_temp.h"
#include <stdio.h>

int main() {
    stdio_init_all();
    hal_temp_init();

    while (true) {
        float temp = hal_temp_read_celsius();
        printf("Temperatura: %.2f °C\n", temp);
        sleep_ms(1000);
    }
}
