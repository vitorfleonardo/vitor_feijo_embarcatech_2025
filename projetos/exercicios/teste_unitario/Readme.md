# 🖥️ Teste unitário para temperatura em RP2040

## 🎯 Objetivo

Desenvolver um teste unitário em **linguagem C** para a **BitDogLab**, utilizando o microcontrolador **RP2040**, que:

- Lê a temperatura interna da MCU pela função float adc_to_celsius(uint16_t adc_val);
- Exetuca uma função de teste unitário para testes se há falha no valor lido pelo ADC.

---

## 🛠️ Etapas de Desenvolvimento

### ✅ Passo 1: Configuração do Ambiente

Antes de iniciar, verifique se você:

1. Conectou a **BitDogLab** ao computador via cabo USB
2. Está usando o **Visual Studio Code** com o SDK C/C++ da Raspberry Pi Pico
3. Adicionou as bibliotecas necessárias para rodar este projeto:

**Bibliotecas necessárias:**

- [`ssd1306.h`](https://github.com/BitDogLab/BitDogLab-C/blob/main/display_oled/inc/ssd1306.h)
- [`ssd1306_font.h`](https://github.com/BitDogLab/BitDogLab-C/blob/main/display_oled/inc/ssd1306_font.h) 
- [`ssd1306_i2c.h`](https://github.com/BitDogLab/BitDogLab-C/blob/main/display_oled/inc/ssd1306_i2c.h) 
- [`ssd1306_i2c.c`](https://github.com/BitDogLab/BitDogLab-C/blob/main/display_oled/inc/ssd1306_i2c.c)

> 📂 Coloque esses arquivos em uma pasta chamada `inc` dentro do seu projeto.

---

### ✅ Passo 2: Conexão do Display OLED à BitDogLab

O display OLED utiliza o protocolo **I2C**, e está conectado aos seguintes pinos da BitDogLab:

| Sinal | GPIO na BitDogLab | Função |
|-------|-------------------|--------|
| SDA   | GPIO14            | Dados |
| SCL   | GPIO15            | Clock |
| Endereço I2C | `0x3C`     | Default do módulo SSD1306 |

> Esses pinos são configurados no código para usar a **I2C1** com velocidade de 400kHz.

---

### ✅ Passo 3: Explicação detalhada do código

#### 🔹 Inicialização dos periféricos

- Configura os pinos 14 (SDA) e 15 (SCL) como interface I2C
- Inicializa a biblioteca do display OLED (ssd1306_init())

```c
i2c_init(i2c1, 400 * 1000);
gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
ssd1306_init();
```

#### 🔹 Ativação do sensor interno do RP2040

- Ativa o conversor analógico-digital (ADC)
- Habilita o sensor de temperatura interno
- Seleciona o canal 4 (onde está conectado internamente o sensor)

```c
adc_init();
adc_set_temp_sensor_enabled(true);
adc_select_input(4);
```

#### 🔹 Conversão da leitura para temperatura

- Converte o valor digital lido do ADC para tensão
- Utiliza a fórmula oficial da Raspberry Pi para calcular a temperatura em Celsius

```c
float read_internal_temperature() {
    const float conversion_factor = 3.3f / (1 << 12);
    uint16_t raw = adc_read();
    float voltage = raw * conversion_factor;
    return 27.0f - (voltage - 0.706f) / 0.001721f;
}
```

#### 🔹 Atualização do Display

- Escreve o texto "Temperatura:" na parte superior
- Exibe a temperatura centralizada na parte inferior
- O display é atualizado a cada 1 segundo com sleep_ms(1000);



```c
ssd1306_draw_string(ssd, 20, 10, "Temperatura:");
ssd1306_draw_string(ssd, x_centered, 30, buffer);
```

---

### ✅ Passo 4: Compilar e Rodar

Dentro da pasta do projeto, utilize os seguintes comandos no terminal:

```
mkdir build
cd build
cmake ..
make
```

Copie o arquivo .uf2 para o dispositivo com o seguinte comando:

```
cp temperatura_rp2040.uf2 /media/$USER/RPI-RP2/
```

---

### ✅ Passo 5: Resultado esperado

- A temperatura exibida será atualizada automaticamente a cada segundo
- É esperado que exiba da seguinte maneira:

```
Temperatura:
   28.73 C
```