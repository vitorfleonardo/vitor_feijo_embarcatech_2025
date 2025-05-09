#include "hal_display.h"
#include <stdio.h>

void display_init(void) {
    printf("Display inicializado\n");
}

void display_atualizar(Histograma* h, Bola* bolas, uint8_t n_bolas) {
    printf("Total bolas: %lu\n", h->total_bolas);
    for (int i = 0; i < NUM_CANALETAS; i++) {
        printf("Canal %d: %d\n", i, h->canaletas[i]);
    }
}
