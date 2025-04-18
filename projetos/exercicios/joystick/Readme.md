# 🎮 Leitura de Joystick com Display OLED – BitDogLab

## 🎯 Objetivo

Desenvolver um programa em **linguagem C** para a **BitDogLab**, utilizando o microcontrolador **RP2040**, que:

- Lê os valores dos eixos X e Y do **joystick analógico**
- Utiliza o **conversor analógico-digital (ADC)** interno do RP2040
- Exibe os valores em tempo real no **display OLED** via interface **I2C**

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

### ✅ Passo 2: Conexão do Display OLED e Joystick à BitDogLab

- O **display OLED** se comunica via **I2C**:
  
| Sinal | GPIO na BitDogLab | Função |
|-------|-------------------|--------|
| SDA   | GPIO14            | Dados |
| SCL   | GPIO15            | Clock |
| Endereço I2C | `0x3C`     | Padrão do SSD1306 |

- O **joystick analógico** utiliza dois canais ADC:
  
| Eixo | GPIO | Canal ADC |
|------|------|-----------|
| Y    | 26   | ADC0      |
| X    | 27   | ADC1      |

---

### ✅ Passo 3: Explicação detalhada do código

#### 🔹 Inicialização dos periféricos

```c
i2c_init(i2c1, 400 * 1000);
gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
ssd1306_init();
```

> Configura os pinos I2C e inicializa o display OLED.

```c
adc_init();
adc_gpio_init(26); // Eixo Y
adc_gpio_init(27); // Eixo X
```

> Ativa o ADC e configura os pinos do joystick como entrada analógica.

#### 🔹 Leitura dos valores do joystick

```c
adc_select_input(1);
uint16_t x_raw = adc_read();
adc_select_input(0);
uint16_t y_raw = adc_read();
```

> Lê os valores brutos dos canais ADC1 (X) e ADC0 (Y). Os valores vão de 0 a 4095.

#### 🔹 Exibição no display OLED

```c
ssd1306_draw_string(ssd, 35, 0, "Joystick:");
```

> Mostra o título “Joystick” na primeira linha.

```c
snprintf(buffer, sizeof(buffer), "X: %4d", x_raw);
ssd1306_draw_string(ssd, x_pos, 16, buffer);

snprintf(buffer, sizeof(buffer), "Y: %4d", y_raw);
ssd1306_draw_string(ssd, y_pos, 32, buffer);
```

> Os valores de X e Y são formatados, centralizados e exibidos em posições diferentes do display para melhor leitura.

---

### ✅ Passo 4: Compilar e Rodar

Dentro da pasta do projeto, utilize os seguintes comandos no terminal:

```
mkdir build
cd build
cmake ..
make
``` 

Copie o arquivo .uf2 para a placa conectada (modo BOOTSEL ativado):

```
cp joystick.uf2 /media/$USER/RPI-RP2/
```

---

### ✅ Passo 5: Resultado esperado

Dentro da pasta do projeto, utilize os seguintes comandos no terminal:

- Ao mover o joystick, os valores dos eixos X e Y mudam de forma suave
- O display OLED atualiza os valores a cada 100 milissegundos, como no exemplo abaixo:

```
Joystick:
 X: 2012
 Y: 1450
```