#include "oled_driver.h"
#include "oled_hal.h"
#include "ssd1306.h"

// Configuração do I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// Inicializa o display
void oled_init() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();
}

// Limpa a tela
void oled_clear() {
    extern uint8_t ssd1306_buffer[]; // Aponta para o buffer global
    for (int i = 0; i < 1024; i++) {
        ssd1306_buffer[i] = 0x00;
    }
    ssd1306_send_buffer(ssd1306_buffer, 1024);
}

// Desenha um círculo com checagem de limites
void oled_draw_circle(int x, int y, int r, bool fill) {
    extern uint8_t ssd1306_buffer[];
    for (int dx = -r; dx <= r; dx++) {
        for (int dy = -r; dy <= r; dy++) {
            if (dx * dx + dy * dy <= r * r) {
                int px = x + dx;
                int py = y + dy;
                if (px >= 0 && px < OLED_WIDTH && py >= 0 && py < OLED_HEIGHT) {
                    ssd1306_set_pixel(ssd1306_buffer, px, py, true);
                }
            }
        }
    }
    ssd1306_send_buffer(ssd1306_buffer, 1024);
}

// Desenha uma string usando a função nativa do driver
void oled_draw_string(int x, int y, const char* str) {
    extern uint8_t ssd1306_buffer[];
    ssd1306_draw_string(ssd1306_buffer, x, y, (char*)str);
    ssd1306_send_buffer(ssd1306_buffer, 1024);
}

// Atualiza o display enviando o buffer atual
void oled_show() {
    extern uint8_t ssd1306_buffer[];
    ssd1306_send_buffer(ssd1306_buffer, 1024);
}
