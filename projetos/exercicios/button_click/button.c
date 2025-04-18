/**
 * Embarcatech 
 * Exemplo Botão com a BitDogLab
 */

 #include <stdio.h>
 #include <string.h>
 #include "pico/stdlib.h"
 #include "hardware/i2c.h"
 #include "inc/ssd1306.h"

 #define BUTTON_A 5
 #define I2C_SDA 14
 #define I2C_SCL 15

int main() {
    stdio_init_all();

    // Configuração do GPIO do Botão A como entrada com pull-up interno
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Inicializa I2C e display
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

    int click_count = 0;
    bool last_button_state = true;

    // Exibe mensagem inicial
    memset(ssd, 0, ssd1306_buffer_length);
    ssd1306_draw_string(ssd, 30, 0, "Contador:");
    snprintf(buffer, sizeof(buffer), "%d cliques", click_count);
    ssd1306_draw_string(ssd, 25, 16, buffer);
    render_on_display(ssd, &frame_area);

    while (true) {
        bool current_state = gpio_get(BUTTON_A);

        // Detecta borda de descida (botão pressionado)
        if (!current_state && last_button_state) {
            click_count++;

            // Atualiza display
            memset(ssd, 0, ssd1306_buffer_length);
            ssd1306_draw_string(ssd, 30, 0, "Contador:");
            snprintf(buffer, sizeof(buffer), "%d cliques", click_count);
            ssd1306_draw_string(ssd, 25, 16, buffer);
            render_on_display(ssd, &frame_area);

            sleep_ms(200);  // debounce
        }

        last_button_state = current_state;
        sleep_ms(10);  // delay curto para loop estável
    }

    return 0;
}

