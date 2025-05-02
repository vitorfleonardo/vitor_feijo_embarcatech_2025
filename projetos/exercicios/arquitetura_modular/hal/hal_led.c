// hal/hal_led.c
#include "hal_led.h"
#include "led_embutido.h"
#include "pico/stdlib.h"

void hal_led_init() {
    led_embutido_init();
}

void hal_led_toggle() {
    static bool estado = false;
    if (estado) {
        led_embutido_off();
    } else {
        led_embutido_on();
    }
    estado = !estado;
}
