#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

// Função para ler temperatura interna do RP2040
float read_internal_temperature() {
    const float conversion_factor = 3.3f / (1 << 12);
    uint16_t raw = adc_read();
    float voltage = raw * conversion_factor;
    return 27.0f - (voltage - 0.706f) / 0.001721f;
}

int main()
{
    stdio_init_all();   // Inicializa os tipos stdio padrão presentes ligados ao binário

    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);
    uint8_t ssd[ssd1306_buffer_length];

    // Inicializa o ADC e sensor interno
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);
   
    char buffer[32];

    while (true) {
        float temp = read_internal_temperature();

        // Limpa a tela
        memset(ssd, 0, ssd1306_buffer_length);

        // Mensagem 
        ssd1306_draw_string(ssd, 20, 10, "Temperatura:");

        // Escreve a temperatura convertida
        snprintf(buffer, sizeof(buffer), "  %.2f C", temp);
        
        // Centraliza o texto da temperatura (tamanho estimado 8 chars → 8*6=48px)
        int text_width = strlen(buffer) * 6;
        int x_centered = (128 - text_width) / 2;

        // Escreve temperatura centralizada, mais abaixo (espaçamento vertical)
        ssd1306_draw_string(ssd, x_centered, 30, buffer);

        render_on_display(ssd, &frame_area);

        sleep_ms(1000);
    }


    return 0;
}
