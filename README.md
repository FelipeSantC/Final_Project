# README - Simulação de Controle de Motor com Joystick

## Descrição

Este projeto utiliza um microcontrolador Raspberry Pi Pico para controlar a direção e a intensidade de um motor simulado, com a entrada de controle sendo realizada através de um joystick. O controle do motor é simulado utilizando LEDs PWM (Pulse Width Modulation), e um display OLED é usado para mostrar o status do motor e as configurações de direção. O projeto é uma simulação de controle remoto de motores, ideal para aplicação em ambientes industriais ou comerciais, como portões automáticos, esteiras, e outros sistemas de automação.

## Funcionalidades

- **Controle de direção e intensidade do motor:** Usando um joystick (representado por dois potenciômetros), é possível ajustar a intensidade do motor e definir a direção (horário ou anti-horário).
- **Feedback visual:** O display OLED exibe informações sobre o status do motor, incluindo "Motor ON/OFF" e a direção do movimento ("Sent Horario" ou "Sent Ant.Hor").
- **Interrupções de botão:** O sistema responde a pressões de botões para ligar/desligar o motor e alterar o estado do controle de intensidade.
- **Controle PWM:** O motor é simulado utilizando LEDs com controle de brilho proporcional à posição do joystick.
- **Debouncing de botões:** Interruptores de botão possuem debouncing para evitar múltiplas leituras rápidas do mesmo evento.

## Componentes Necessários

- **Raspberry Pi Pico** (ou qualquer microcontrolador compatível com a biblioteca `pico/stdlib`).
- **Joystick** (dois potenciômetros para o eixo X e Y e um botão).
- **LEDs** (para simulação do motor e controle de direção e intensidade).
- **Display OLED SSD1306** (para exibir o status do sistema).
- **Fios de conexão e alimentação**.

## Configuração de Hardware

### Pinos

- **Joystick**:
  - **VRX_PIN** (Pino 26) - Controle do eixo X.
  - **VRY_PIN** (Pino 27) - Controle do eixo Y.
  - **SW_PIN** (Pino 22) - Botão do joystick.

- **LEDs**:
  - **LED1_PIN** (Pino 11) - LED indicador do status do motor.
  - **LED2_PIN** (Pino 13) - LED simulando a intensidade do motor (PWM).
  - **LED3_PIN** (Pino 12) - LED simulando a direção do motor (PWM).

- **Botões**:
  - **button_A** (Pino 5) - Botão para acionar ou alterar o estado do motor.
  - **button_B** (Pino 6) - Botão para parar o motor e resetar a direção.

- **Display OLED**:
  - **I2C SDA** (Pino 14).
  - **I2C SCL** (Pino 15).

## Instalação e Execução

### Pré-requisitos

- Raspberry Pi Pico configurado e com a biblioteca `pico-sdk` instalada.
- As bibliotecas necessárias para o controle de I2C e PWM, como `hardware/i2c.h` e `hardware/pwm.h`.
- Conexão com o display OLED SSD1306, utilizando o protocolo I2C.
- Editor de código compatível com a programação do Raspberry Pi Pico, como o VSCode ou Thonny.

### Passos para Execução

1. **Conectar o hardware:**
   - Conecte o joystick, LEDs e display OLED aos pinos correspondentes no Raspberry Pi Pico, conforme descrito acima.

2. **Compilar e carregar o código no Raspberry Pi Pico:**
   - Compile o código utilizando o SDK do Raspberry Pi Pico.
   - Carregue o código no dispositivo.

3. **Executar o código:**
   - O código irá iniciar automaticamente ao ser carregado.
   - O display OLED será inicializado e mostrará o status do motor.
   - O joystick controlará a intensidade e direção do motor (simulado com LEDs PWM).
   - O botão A e o botão B permitirão ligar/desligar o motor e alterar a direção.

## Explicação do Código

### Funções principais

- **`eixo_x()`**: Responsável por ler o valor do eixo X do joystick e ajustar a intensidade do motor (simulada com PWM). O brilho do LED2 e LED3 é ajustado com base na posição do joystick.

- **`display()`**: Atualiza o display OLED com o status atual do motor e a direção selecionada. Exibe "Motor ON/OFF" e "Sent Horario" ou "Sent Ant.Hor" dependendo da direção.

- **`gpio_irq_button()`**: Função de interrupção que lida com os botões. Quando o botão A é pressionado, alterna entre os estados de intensidade e direção do motor. Quando o botão B é pressionado, desliga o motor e reseta a direção.

### PWM e controle de intensidade

O código utiliza o recurso de PWM para simular o controle de intensidade do motor. O valor lido do joystick determina o brilho dos LEDs, simulando a variação da intensidade do motor. A direção do motor é controlada pela posição do joystick (esquerda/direita) e é refletida no controle de direção dos LEDs.

### Interrupções de botões

O sistema utiliza interrupções para gerenciar a pressão dos botões. Isso garante que os botões sejam lidos de maneira eficiente e sem múltiplos acionamentos rápidos, utilizando a técnica de *debouncing*.

