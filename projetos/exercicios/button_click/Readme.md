# 👆 Contador de Cliques no Botão – BitDogLab

## 🎯 Objetivo

Desenvolver um programa em **linguagem C** para a **BitDogLab**, utilizando o microcontrolador **RP2040**, que:

- Detecta **apertos sucessivos de um botão físico**
- Conta o número de vezes que o botão foi pressionado
- Exibe essa contagem em tempo real no **display OLED** via **I2C**

---

## 🛠️ Etapas de Desenvolvimento

### ✅ Passo 1: Configuração do Ambiente

Certifique-se de que o ambiente de desenvolvimento esteja corretamente preparado:

1. **Conecte** a placa BitDogLab ao computador
2. Utilize o **VS Code** com o SDK da Raspberry Pi Pico configurado
3. **Inclua as bibliotecas necessárias** no projeto:

| Biblioteca | Função |
|------------|--------|
| `ssd1306.h` | Interface com o display OLED |
| `ssd1306_font.h` | Tabela de fontes para texto no display |
| `ssd1306_i2c.h` | Funções de comunicação I2C |
| `ssd1306_i2c.c` | Implementação da comunicação com o display |

> 📂 Coloque todos os arquivos da biblioteca em uma pasta chamada `inc` no seu projeto.

---

### ✅ Passo 2: Conexão do Hardware

- O **display OLED** está conectado via **I2C**:

| Sinal | GPIO | Função |
|-------|------|--------|
| SDA   | 14   | Dados I2C |
| SCL   | 15   | Clock I2C |
| Endereço I2C | `0x3C` | Padrão SSD1306 |

- O **botão A** está conectado ao **GPIO5**

---

### ✅ Passo 3: Explicação do Código

#### 🔹 Leitura do botão físico

```c
gpio_init(BUTTON_A);
gpio_set_dir(BUTTON_A, GPIO_IN);
gpio_pull_up(BUTTON_A);
```

> O botão A é configurado como entrada com pull-up interno para garantir leitura estável.

#### 🔹 Detecção de clique (borda de descida)

```c
if (!current_state && last_button_state)
```

> Essa condição detecta o momento exato em que o botão é pressionado, evitando múltiplas leituras contínuas.

#### 🔹 Contador e exibição no display

```c
click_count++;
snprintf(buffer, sizeof(buffer), "%d cliques", click_count);
ssd1306_draw_string(ssd, 25, 16, buffer);
```

> A cada clique válido, o contador aumenta e a nova contagem é exibida na tela OLED.

#### 🔹  Debounce simples por software

```c
sleep_ms(200);
```

> Garante que apenas um clique real seja contado, ignorando ruídos (bouncing) do botão físico.

---

### ✅ Passo 4: Compilar e Rodar

compile o projeto:

```
mkdir build
cd build
cmake ..
make
```

Depois, copie o .uf2 para a placa:

```
cp button.uf2 /media/$USER/RPI-RP2/
```

---

### ✅ Passo 5: Resultado Esperado

- A cada clique no botão A, o número exibido no display aumenta:

```
Contador:
 3 cliques
```
- A contagem é atualizada em tempo real, com feedback visual no display OLED.