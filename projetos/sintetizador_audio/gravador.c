#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

// === Configurações ===
#define MIC_GPIO            28
#define MIC_ADC_CHANNEL     2
#define SAMPLE_RATE_HZ      8000
#define RECORD_DURATION_S   2
#define BUFFER_SIZE         (SAMPLE_RATE_HZ * RECORD_DURATION_S)
#define ADC_CLOCK_DIV       (48000000.0f / SAMPLE_RATE_HZ)

// === LEDs RGB ===
#define LED_R 13
#define LED_G 11
#define LED_B 12

// === Botões ===
#define BUTTON_A 5
#define BUTTON_B 6

// === Buzzer ===
#define BUZZER_PIN 21

// === Buffer de áudio e DMA ===
static uint16_t audio_buffer[BUFFER_SIZE];
static uint dma_chan;
static dma_channel_config dma_cfg;

void setup_leds() {
    gpio_init(LED_R); gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G); gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B); gpio_set_dir(LED_B, GPIO_OUT);
    gpio_put(LED_R, 0); gpio_put(LED_G, 0); gpio_put(LED_B, 0);
}

void led_set_rgb(bool r, bool g, bool b) {
    gpio_put(LED_R, r); gpio_put(LED_G, g); gpio_put(LED_B, b);
}

void setup_botoes() {
    gpio_init(BUTTON_A); gpio_set_dir(BUTTON_A, GPIO_IN); gpio_pull_up(BUTTON_A);
    gpio_init(BUTTON_B); gpio_set_dir(BUTTON_B, GPIO_IN); gpio_pull_up(BUTTON_B);
}

bool botao_apertado(uint gpio) {
    return !gpio_get(gpio); // Pull-up: ativo em LOW
}

void init_adc_dma() {
    adc_gpio_init(MIC_GPIO);
    adc_init();
    adc_select_input(MIC_ADC_CHANNEL);
    adc_fifo_setup(true, true, 1, false, false);
    adc_set_clkdiv(ADC_CLOCK_DIV);

    dma_chan = dma_claim_unused_channel(true);
    dma_cfg = dma_channel_get_default_config(dma_chan);
    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&dma_cfg, false);
    channel_config_set_write_increment(&dma_cfg, true);
    channel_config_set_dreq(&dma_cfg, DREQ_ADC);
}

void gravar_audio() {
    printf("Gravando áudio...\n");
    led_set_rgb(true, false, false); // Vermelho

    adc_fifo_drain();
    adc_run(false);

    dma_channel_configure(
        dma_chan, &dma_cfg,
        audio_buffer, &adc_hw->fifo,
        BUFFER_SIZE, true
    );

    adc_run(true);
    dma_channel_wait_for_finish_blocking(dma_chan);
    adc_run(false);

    led_set_rgb(false, true, false); // Verde
    printf("Gravação concluída.\n");
}

void setup_pwm_buzzer() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_wrap(slice, 255); // Resolução de 8 bits
    pwm_set_enabled(slice, true);
}

void reproduzir_audio() {
    printf("Reproduzindo áudio...\n");
    led_set_rgb(false, false, true); // Azul

    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);

    for (int i = 0; i < BUFFER_SIZE; i++) {
        uint16_t amostra = audio_buffer[i];
        uint8_t duty = amostra >> 4; // De 12 bits (0–4095) para 8 bits (0–255)
        pwm_set_gpio_level(BUZZER_PIN, duty);
        sleep_us(1000000 / SAMPLE_RATE_HZ); // Tempo entre amostras
    }

    pwm_set_gpio_level(BUZZER_PIN, 0); // Desliga som
    printf("Reprodução finalizada.\n");
}

int main() {
    stdio_init_all();
    sleep_ms(3000);

    setup_leds();
    setup_botoes();
    init_adc_dma();
    setup_pwm_buzzer();

    printf("Pressione Botão A para gravar. Botão B para reproduzir.\n");

    while (true) {
        led_set_rgb(false, false, false);

        if (botao_apertado(BUTTON_A)) {
            while (botao_apertado(BUTTON_A)) tight_loop_contents();
            gravar_audio();
        }

        if (botao_apertado(BUTTON_B)) {
            while (botao_apertado(BUTTON_B)) tight_loop_contents();
            reproduzir_audio();
        }

        tight_loop_contents();
    }

    return 0;
}
