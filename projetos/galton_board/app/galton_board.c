#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
#include "oled_driver.h"
#include "buzzer_driver.h"
#include "galton_board.h"
#include "ssd1306.h" // Buffer com o triângulo fixo


#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define PEG_RADIUS 2
#define BALL_RADIUS 1
#define GRAVITY 0.75
#define BOUNCINESS 0.5
#define PEG_H_SPACING (OLED_WIDTH / (2 * (PEG_ROWS + 1)))
#define PEG_V_SPACING (OLED_HEIGHT / (2 * (PEG_ROWS + 1)))
#define PEG_ROWS 5
#define MAX_BALLS 80
#define BALL_START_Y 0
#define TRIANGLE_START_Y 20  // 5 pixels abaixo do topo da tela


static uint8_t base_buffer[1024]; 
bool ball_has_fallen = false;

typedef struct { float x, y, vx, vy; } Ball;
typedef struct { int x, y; } Peg;

Ball ball;
Peg pegs[PEG_ROWS][PEG_ROWS];
int peg_count[PEG_ROWS];

static void setup_pegs() {
    for (int row = 0; row < PEG_ROWS; row++) {
        peg_count[row] = row + 1;
        int start_x = (OLED_WIDTH - (peg_count[row] - 1) * PEG_H_SPACING) / 2;
        int y = TRIANGLE_START_Y + row * PEG_V_SPACING;
        for (int col = 0; col < peg_count[row]; col++) {
            pegs[row][col].x = start_x + col * PEG_H_SPACING;
            pegs[row][col].y = y;
        }
    }
}

static void reset_ball() {
    ball.x = OLED_WIDTH / 2;
    ball.y = BALL_START_Y;
    ball.vx = ((rand() % 3) - 1) * 0.5f;
    ball.vy = 0;
}

static void check_collision() {
    for (int row = 0; row < PEG_ROWS; row++) {
        for (int col = 0; col < peg_count[row]; col++) {
            float dx = ball.x - pegs[row][col].x;
            float dy = ball.y - pegs[row][col].y;
            float distance = sqrtf(dx * dx + dy * dy);
            if (distance < (PEG_RADIUS + BALL_RADIUS)) {
                float nx = dx / distance;
                float ny = dy / distance;
                float dot = ball.vx * nx + ball.vy * ny;
                if (dot < 0) {
                    ball.vx -= 2 * dot * nx;
                    ball.vy -= 2 * dot * ny;
                    ball.vx *= BOUNCINESS;
                    ball.vy *= BOUNCINESS;
                    ball.x = pegs[row][col].x + nx * (PEG_RADIUS + BALL_RADIUS + 1);
                    ball.y = pegs[row][col].y + ny * (PEG_RADIUS + BALL_RADIUS + 1);
                    buzzer_beep();
                }
            }
        }
    }
}

static void update_ball() {
    ball.vy += GRAVITY;
    ball.x += ball.vx;
    ball.y += ball.vy;
    if (ball.x < BALL_RADIUS || ball.x > OLED_WIDTH - BALL_RADIUS)
        ball.vx = -ball.vx;
    if (ball.y > OLED_HEIGHT && !ball_has_fallen) {
        ball_has_fallen = true;  // Marca que a bola caiu
    }
}

static void draw_pegs() {
    for (int row = 0; row < PEG_ROWS; row++) {
        for (int col = 0; col < peg_count[row]; col++) {
            oled_draw_circle(pegs[row][col].x, pegs[row][col].y, PEG_RADIUS, true);
        }
    }
}

static void draw_scene() {
    oled_clear();
    draw_pegs();
    oled_draw_circle((int)ball.x, (int)ball.y, BALL_RADIUS, true);
    oled_show();
}

void galton_board_run() {
    oled_init();
    buzzer_init();
    setup_pegs();

    oled_clear();
    draw_pegs();
    memcpy(base_buffer, ssd1306_buffer, 1024);  // Salva o triângulo fixo

    int ball_counter = 0;
    reset_ball();
    bool ball_has_fallen = false;

    while (ball_counter < MAX_BALLS) {
        check_collision();
        update_ball();

        // Atualiza a cena a partir do buffer base (triângulo)
        memcpy(ssd1306_buffer, base_buffer, 1024);
        oled_draw_circle((int)ball.x, (int)ball.y, BALL_RADIUS, true);
        oled_show();

        // Detecta se a bola caiu completamente
        if (ball.y > OLED_HEIGHT) {
            ball_counter++;
            reset_ball();
        }

        sleep_ms(30);
    }

    oled_clear();
    oled_draw_string(20, 30, "Fim da simulacao!");
    oled_show();
    while (1) sleep_ms(1000);
}