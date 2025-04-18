/**
 * Embarcatech 
 * Contador decrescente com eventos por interrupção (BitDogLab)
 */

 #include <stdio.h>
 #include <string.h>
 #include "pico/stdlib.h"
 #include "hardware/i2c.h"
 #include "hardware/gpio.h"
 #include "inc/ssd1306.h"
 
 #define BUTTON_A 5
 #define BUTTON_B 6
 #define I2C_SDA 14
 #define I2C_SCL 15
 
 volatile int countdown = 0;
 volatile int event_count = 0;
 volatile bool start_flag = false;
 volatile absolute_time_t last_event_time;
 
 uint8_t ssd[ssd1306_buffer_length];
 char buffer[32];
 struct render_area frame_area;
 
 // Callback geral de interrupções com debounce para botão B
 void gpio_callback(uint gpio, uint32_t events) {
     if (gpio == BUTTON_A) {
         countdown = 9;
         event_count = 0;
         start_flag = true;
     } else if (gpio == BUTTON_B && countdown > 0) {
         absolute_time_t now = get_absolute_time();
         if (absolute_time_diff_us(last_event_time, now) > 200000) {
             last_event_time = now;
             event_count++;
         }
     }
 }
 
 // Atualiza o display OLED com os valores atuais
 void update_display() {
     memset(ssd, 0, ssd1306_buffer_length);
 
     snprintf(buffer, sizeof(buffer), "Contagem: %d", countdown);
     ssd1306_draw_string(ssd, 10, 0, buffer);
 
     snprintf(buffer, sizeof(buffer), "Eventos: %d", event_count);
     ssd1306_draw_string(ssd, 10, 16, buffer);
 
     render_on_display(ssd, &frame_area);
 }
 
 int main() {
     stdio_init_all();
 
     // Configuração dos botões
     gpio_init(BUTTON_A);
     gpio_set_dir(BUTTON_A, GPIO_IN);
     gpio_pull_up(BUTTON_A);
 
     gpio_init(BUTTON_B);
     gpio_set_dir(BUTTON_B, GPIO_IN);
     gpio_pull_up(BUTTON_B);
 
     // Registra interrupções usando callback único
     gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
     gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);
 
     // Inicializa I2C e Display OLED
     i2c_init(i2c1, ssd1306_i2c_clock * 1000);
     gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
     gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
     gpio_pull_up(I2C_SDA);
     gpio_pull_up(I2C_SCL);
     ssd1306_init();
 
     // Define a área de renderização do display
     frame_area = (struct render_area){
         start_column : 0,
         end_column : ssd1306_width - 1,
         start_page : 0,
         end_page : ssd1306_n_pages - 1
     };
     calculate_render_area_buffer_length(&frame_area);
 
     // Inicializa estado
     countdown = 0;
     event_count = 0;
     last_event_time = get_absolute_time();  // inicializa controle de debounce
     update_display();
 
     while (true) {
         if (start_flag && countdown >= 0) {
             update_display();
             sleep_ms(1000);
             countdown--;
 
             if (countdown < 0) {
                 countdown = 0;
                 start_flag = false;
                 update_display();
             }
         } else {
             sleep_ms(50);
         }
     }
 
     return 0;
 }
 