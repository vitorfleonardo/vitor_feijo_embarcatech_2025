# ⏱️ Contador Decrescente com Registro de Eventos – BitDogLab

## 🎯 Objetivo

Desenvolver um programa em **linguagem C** para a **BitDogLab**, utilizando o microcontrolador **RP2040**, que:

- Inicia uma **contagem regressiva de 9 até 0** ao pressionar um botão
- Durante essa contagem, registra quantas vezes outro botão é pressionado
- Exibe os valores da **contagem** e dos **eventos** em tempo real no **display OLED**
- Utiliza **interrupções com debounce** para capturar os cliques de forma confiável

---

## 🛠️ Etapas de Desenvolvimento

### ✅ Passo 1: Configuração do Ambiente

1. Conecte a **BitDogLab** ao computador com cabo USB
2. Configure o **VS Code** com o SDK da Raspberry Pi Pico
3. Inclua as bibliotecas do display OLED:

| Biblioteca | Função |
|------------|--------|
| `ssd1306.h` | Interface com o display OLED |
| `ssd1306_font.h` | Tabela de caracteres |
| `ssd1306_i2c.h` | Controle da comunicação I2C |
| `ssd1306_i2c.c` | Implementação da lógica do display |

> 💡 Organize essas bibliotecas na pasta `inc` do seu projeto.

---

### ✅ Passo 2: Conexão dos Componentes

#### 🧩 Display OLED (I2C)

| Sinal | GPIO | Função |
|-------|------|--------|
| SDA   | 14   | Dados I2C |
| SCL   | 15   | Clock I2C |
| Endereço | `0x3C` | Padrão SSD1306 |

#### 🔘 Botões

| Botão | GPIO | Função |
|-------|------|--------|
| A     | 5    | Inicia a contagem |
| B     | 6    | Registra eventos durante a contagem |

---

### ✅ Passo 3: Lógica do Programa

#### 🔹 O que acontece ao apertar o botão A:

- O contador reinicia de 9
- O número de eventos é zerado
- A contagem decresce a cada 1 segundo até 0

#### 🔹 Durante a contagem (9 → 0):

- Cada clique no botão B é registrado como evento (com **filtro de debounce** de 200ms)
- A quantidade de eventos e o valor atual do contador são mostrados no display

#### 🔹 Quando chega em 0:

- O sistema para
- Não registra mais eventos
- Aguarda novo clique no botão A para reiniciar

---

### ✅ Passo 4: Explicação detalhada do código

#### 🔹 Inicialização dos GPIOs e periféricos

```c
gpio_init(BUTTON_A);
gpio_set_dir(BUTTON_A, GPIO_IN);
gpio_pull_up(BUTTON_A);
```

> Configura o botão A como entrada com pull-up interno. O mesmo é feito para o botão B. Isso garante que o valor lido seja 1 quando o botão não está pressionado, e 0 quando está.

```c
i2c_init(i2c1, ssd1306_i2c_clock * 1000);
ssd1306_init();
```

> Inicializa a comunicação I2C com o display OLED e prepara o display SSD1306 para uso.

#### 🔹 Lógica da contagem e estado

```c
volatile int countdown = 0;
volatile int event_count = 0;
volatile bool start_flag = false;
```

> Essas variáveis armazenam: O valor atual da contagem decrescente; A quantidade de cliques no botão B; Um sinal de que a contagem foi iniciada.

#### 🔹 Função de Interrupção – Callback do GPIO

```c
void gpio_callback(uint gpio, uint32_t events)
```

> Esta função é automaticamente chamada quando os botões A ou B são pressionados. A lógica interna faz o seguinte:

Botão A (inicia a contagem):

```c
countdown = 9;
event_count = 0;
start_flag = true;
```

Botão B (conta eventos se countdown > 0):

```c
absolute_time_t now = get_absolute_time();
if (absolute_time_diff_us(last_event_time, now) > 200000)
```

> Aplica debounce por tempo, ignorando cliques repetidos em menos de 200 milissegundos.

#### 🔹 Laço principal (main)

```c
if (start_flag && countdown >= 0) {
    update_display();
    sleep_ms(1000);
    countdown--;
```

> Quando a contagem está ativa (start_flag), atualiza o display, aguarda 1 segundo, e decrementa o contador. Quando chega a zero:

```c
countdown = 0;
start_flag = false;
update_display();
```

> A contagem para e os eventos não são mais contabilizados até que o botão A seja pressionado novamente.

---

### ✅ Passo 5: Compilar e Rodar

Dentro da pasta do projeto, utilize os seguintes comandos no terminal:

```
mkdir build
cd build
cmake ..
make
``` 

Copie o arquivo .uf2 para a placa conectada (modo BOOTSEL ativado):

```
cp button.uf2 /media/$USER/RPI-RP2/
```

---

### ✅ Passo 6: Resultado esperado

Dentro da pasta do projeto, utilize os seguintes comandos no terminal:

- Pressione o botão A → Contagem inicia em 9
- Durante a contagem, pressione o botão B algumas vezes
- Quando o contador chega a 0, ele congela
- A contagem e os eventos ficam fixos na tela até que o botão A seja pressionado novamente