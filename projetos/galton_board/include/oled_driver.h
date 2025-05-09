#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

#include <stdbool.h>

// Inicializa o display OLED via I2C
void oled_init();

// Limpa o conteúdo do display
void oled_clear();

// Desenha um círculo no display
void oled_draw_circle(int x, int y, int r, bool fill);

// Desenha uma string simulada no display (substituto temporário)
void oled_draw_string(int x, int y, const char* str);

// Atualiza o display para refletir as alterações no buffer
void oled_show();

#endif
