// drivers/led_embutido.c
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "led_embutido.h"

void led_embutido_init() {
    cyw43_arch_init();
}

void led_embutido_on() {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
}

void led_embutido_off() {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
}
