#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/dma.h"

// === Configurações ===
#define MIC_GPIO            28     // GPIO do microfone
#define MIC_ADC_CHANNEL     2      // Canal ADC correspondente

#define SAMPLE_RATE_HZ      8000   // Amostras por segundo
#define RECORD_DURATION_S   2      // Duração da gravação
#define BUFFER_SIZE         (SAMPLE_RATE_HZ * RECORD_DURATION_S)

#define ADC_CLOCK_DIV       (48000000.0f / SAMPLE_RATE_HZ)

// === LED RGB (BitDogLab) ===
#define LED_R               13
#define LED_G               11
#define LED_B               12

// === Botão A ===
#define BUTTON_A            5

// === Buffer e DMA ===
static uint16_t audio_buffer[BUFFER_SIZE];
static uint dma_chan;
static dma_channel_config dma_cfg;

void setup_leds() {
    gpio_init(LED_R); gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G); gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B); gpio_set_dir(LED_B, GPIO_OUT);
    gpio_put(LED_R, 0);
    gpio_put(LED_G, 0);
    gpio_put(LED_B, 0);
}

void led_set_rgb(bool r, bool g, bool b) {
    gpio_put(LED_R, r);
    gpio_put(LED_G, g);
    gpio_put(LED_B, b);
}

void init_adc_dma() {
    // === ADC ===
    adc_gpio_init(MIC_GPIO);
    adc_init();
    adc_select_input(MIC_ADC_CHANNEL);
    adc_fifo_setup(true, true, 1, false, false);
    adc_set_clkdiv(ADC_CLOCK_DIV);

    // === DMA ===
    dma_chan = dma_claim_unused_channel(true);
    dma_cfg = dma_channel_get_default_config(dma_chan);
    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&dma_cfg, false);
    channel_config_set_write_increment(&dma_cfg, true);
    channel_config_set_dreq(&dma_cfg, DREQ_ADC);
}

void gravar_audio() {
    printf("Iniciando gravação...\n");
    led_set_rgb(true, false, false); // LED vermelho

    adc_fifo_drain();
    adc_run(false);

    dma_channel_configure(
        dma_chan,
        &dma_cfg,
        audio_buffer,
        &adc_hw->fifo,
        BUFFER_SIZE,
        true
    );

    adc_run(true);
    dma_channel_wait_for_finish_blocking(dma_chan);
    adc_run(false);

    led_set_rgb(false, true, false); // LED verde
    printf("Gravação concluída.\n");
}

void print_amostras() {
    printf("Amostras capturadas:\n");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d\n", audio_buffer[i]);
    }
}

void setup_botao() {
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A); // Botão A é pull-down na BitDogLab
}

bool botao_apertado() {
    return !gpio_get(BUTTON_A); // Ativo em nível baixo
}

int main() {
    stdio_init_all();
    sleep_ms(3000);

    setup_leds();
    setup_botao();
    init_adc_dma();

    printf("Aguardando botão A para iniciar gravação...\n");

    while (true) {
        led_set_rgb(false, false, true); // Azul: aguardando

        // Espera o botão ser pressionado
        while (!botao_apertado()) {
            tight_loop_contents();
        }

        // Aguarda o botão ser solto (debounce simples)
        sleep_ms(100);
        while (botao_apertado()) {
            tight_loop_contents();
        }

        gravar_audio();
        print_amostras();

        printf("Aguardando nova gravação...\n");
    }

    return 0;
}
