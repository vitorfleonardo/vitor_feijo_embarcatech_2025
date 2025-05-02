#include <stdio.h>
#include "hardware/adc.h"

// Função para ler temperatura interna do RP2040
float adc_to_celsius(uint16_t adc_val) {
    float voltage = (adc_val / 4095.0) * 3.3;
    float temperature = 27.0f - ((voltage - 0.706f) / 0.001721f);

    return temperature;
}

int main()
{
    stdio_init_all();   
    adc_init(); 
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    while(1) {
        uint16_t adc_val = adc_read();
        float temperature = adc_to_celsius(adc_val);
        printf("ADC Value: %d, Temperature: %.2f C\n", adc_val, temperature);
        sleep_ms(1000); // Aguarda 1 segundo antes de ler novamente
    }

    return 0;
}
