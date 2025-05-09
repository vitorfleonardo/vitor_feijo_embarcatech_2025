#ifndef GALTON_H
#define GALTON_H

#include <stdint.h>

#define NUM_CANALETAS 10
#define MAX_BOLAS 20

typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    uint8_t colisoes;
    uint8_t canal;
    uint8_t ativa;
} Bola;

typedef struct {
    uint32_t total_bolas;
    uint16_t canaletas[NUM_CANALETAS];
} Histograma;

void galton_init(void);
void galton_tick(void);
void galton_lancar_bola(void);
void galton_resetar(void);

#endif // GALTON_H
