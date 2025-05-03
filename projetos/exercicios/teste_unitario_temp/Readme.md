# 🖥️ Teste unitário com temperatura interna do RP2040

## 🎯 Objetivo

Desenvolver um programa em linguagem C, utilizando o microcontrolador RP2040 da Raspberry Pi Pico W, com foco em arquitetura modular e testes unitários. O sistema:

- Lê a temperatura interna da MCU via ADC embutido
- Converte a leitura do ADC para graus Celsius com base na fórmula oficial da Raspberry Pi
- Organiza o código em camadas: app, hal, drivers e include
- Inclui teste unitário com a biblioteca Unity para validar a conversão

---

##📁 Estrutura do Projeto

```
projeto/
├── app/                  # Lógica da aplicação
│   └── main.c
├── drivers/              # Interface com o hardware
│   └── adc_temp.c
├── hal/                  # Camada de abstração
│   └── hal_temp.c
├── include/              # Headers (.h)
│   ├── adc_temp.h
│   └── hal_temp.h
├── tests/                # Testes unitários
│   └── test_adc.c
├── lib/                  # Biblioteca Unity
│   └── unity/
└── CMakeLists.txt        # Arquivo de build
```

## 🛠️ Etapas de Desenvolvimento

### ✅ Passo 1: Inicialização do ADC

```
adc_init();
adc_set_temp_sensor_enabled(true);
adc_select_input(4);
```

### ✅ Passo 2: Conversão ADC → Celsius

```
float voltage = (adc_val / 4095.0f) * 3.3f;
float temperature = 27.0f - ((voltage - 0.706f) / 0.001721f);
```

### ✅ Passo 3: Camada HAL

```
float hal_temp_read_celsius();
```

### ✅ Passo 4: Aplicação Principal

```
int main() {
    stdio_init_all();
    hal_temp_init();

    while (true) {
        float temp = hal_temp_read_celsius();
        printf("Temperatura: %.2f °C\n", temp);
        sleep_ms(1000);
    }
}
```

### ✅ Passo 5: Teste Unitário com Unity

```
void test_adc_to_celsius_known_value() {
    uint16_t adc_simulado = 876; // Aproximadamente 0.706V
    float temp = adc_to_celsius(adc_simulado);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 27.0, temp); // Margem de erro aceitável
}
```

---

## Compilar e rodar

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