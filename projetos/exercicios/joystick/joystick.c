/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

int main() {
    stdio_init_all();
    adc_init();
   
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();

    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);
    uint8_t ssd[ssd1306_buffer_length];
    char buffer[32];

    // Inicializa ADC
    adc_init();
    adc_gpio_init(26); // Y
    adc_gpio_init(27); // X

    while (1) {
        // Lê valores dos eixos X e Y
        adc_select_input(1);  // GP27 → ADC1 → Eixo X
        uint16_t x_raw = adc_read();
        adc_select_input(0);  // GP26 → ADC0 → Eixo Y
        uint16_t y_raw = adc_read();

        // Limpa tela
        memset(ssd, 0, ssd1306_buffer_length);

        // Mostra valores
        ssd1306_draw_string(ssd, 35, 0, "Joystick:");

        // Valor de X
        snprintf(buffer, sizeof(buffer), "X: %4d", x_raw);
        int len_x = strlen(buffer) * 6;
        int x_pos = (128 - len_x) / 2;
        ssd1306_draw_string(ssd, x_pos, 16, buffer);

        // Valor de Y
        snprintf(buffer, sizeof(buffer), "Y: %4d", y_raw);
        int len_y = strlen(buffer) * 6;
        int y_pos = (128 - len_y) / 2;
        ssd1306_draw_string(ssd, y_pos, 32, buffer);

        render_on_display(ssd, &frame_area);

        sleep_ms(100);  // Atualiza a cada 100 ms
     }

     return 0;
}