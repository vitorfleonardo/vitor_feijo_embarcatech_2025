#ifndef HAL_DISPLAY_H
#define HAL_DISPLAY_H

#include "galton.h"

void display_init(void);
void display_atualizar(Histograma* h, Bola* bolas, uint8_t n_bolas);

#endif // HAL_DISPLAY_H
