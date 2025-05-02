# 💡 Pisca LED com Arquitetura Modular

## 🎯 Objetivo

Desenvolver um programa em linguagem C, utilizando o microcontrolador RP2040 na Raspberry Pi Pico W, com foco em arquitetura modular de software. O sistema:

- Utiliza a estrutura modular com separação em drivers, HAL e aplicação
- Controla o LED embutido da Pico W
- Pisca o LED em intervalos fixos
- Organiza o código de forma escalável e reutilizável

---

## 📁 Estrutura do Projeto

```
projeto/
├── app/
│   └── main.c              # Lógica principal da aplicação
├── drivers/
│   └── led_embutido.c      # Controle direto do LED via cyw43_arch
├── hal/
│   └── hal_led.c           # Abstração da lógica de controle do LED
├── include/
│   ├── led_embutido.h      # Header do driver
│   └── hal_led.h           # Header da HAL
└── CMakeLists.txt          # Arquivo de build
```

---

## 🧪 Compilar e Executar

```
mkdir build
cd build
cmake ..
make
```

Depois:
```
cp arquitetura_modular.uf2 /media/$USER/RPI-RP2/
```

## Resultado Esperado

- O LED embutido da Pico W pisca a cada 500ms
- O código é organizado em camadas, facilitando testes e reuso
- Alterações futuras no hardware ou lógica do LED requerem apenas ajustes nos arquivos da HAL ou driver, sem afetar o main.c
