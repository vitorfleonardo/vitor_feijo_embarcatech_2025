// drivers/adc_temp.c
#include <stdint.h>
#include "adc_temp.h"

float adc_to_celsius(uint16_t adc_val) {
    // Conversão conforme documentação oficial do RP2040
    float voltage = (adc_val / 4095.0f) * 3.3f;
    float temperature = 27.0f - ((voltage - 0.706f) / 0.001721f);
    return temperature;
}
