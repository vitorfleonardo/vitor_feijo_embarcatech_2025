#ifndef BALL_H
#define BALL_H

#include "galton.h"

void bola_inicializar(Bola* b);
void bola_atualizar(Bola* b);
uint8_t bola_colidiu_com_pino(Bola* b);
uint8_t bola_chegou_fundo(Bola* b);

#endif // BALL_H
