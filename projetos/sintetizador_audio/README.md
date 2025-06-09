# 🧠 O que o código faz

O código implementa um gravador de áudio simples e funcional com a BitDogLab (Raspberry Pi Pico W), usando:

- ADC para capturar o som do microfone onboard (ligado ao GPIO 28).
- DMA para armazenar as amostras rapidamente na RAM, sem sobrecarregar a CPU.
- LEDs RGB como indicadores visuais de estado.
- Botão A (GPIO 5) como gatilho para iniciar uma nova gravação.

# 🎯 Resumo do funcionamento

Pico inicializa e espera o botão A ser pressionado.

Quando você aperta o botão A:

1. Ele começa a gravar áudio por 2 segundos (8000 amostras por segundo).
2. Os valores são salvos na RAM interna da Pico.
3. Ao fim da gravação, os valores das amostras são impressos no terminal.
4. Depois disso, o sistema volta ao estado de espera, pronto para nova gravação.

# 💡 Significado das cores dos LEDs

| Cor do LED   | Estado do sistema         | GPIO utilizado |
| ------------ | ------------------------- | -------------- |
| **Azul**     | Aguardando botão A (idle) | GPIO 12        |
| **Vermelho** | Gravando áudio (ativo)    | GPIO 13        |
| **Verde**    | Gravação finalizada       | GPIO 11        |
