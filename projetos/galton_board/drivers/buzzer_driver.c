#include "buzzer_driver.h"
#include "buzzer_hal.h"

#define BUZZER_PIN 21

void buzzer_init() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_wrap(slice, 12500);
    pwm_set_chan_level(slice, PWM_CHAN_A, 6250);
    pwm_set_enabled(slice, true);
}

void buzzer_beep() {
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_enabled(slice, true);
    sleep_ms(50);
    pwm_set_enabled(slice, false);
}
