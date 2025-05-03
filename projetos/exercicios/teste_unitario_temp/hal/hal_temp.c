// hal/hal_temp.c
#include "hardware/adc.h"
#include "hal_temp.h"
#include "adc_temp.h"

void hal_temp_init() {
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4); // Canal interno de temperatura
}

float hal_temp_read_celsius() {
    uint16_t raw = adc_read();
    return adc_to_celsius(raw);
}
