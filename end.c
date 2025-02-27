#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"


// Display
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

#define VRX_PIN 26
#define VRY_PIN 27
#define SW_PIN 22
#define LED1_PIN 11
#define LED2_PIN 13
#define LED3_PIN 12
#define button_A 5
#define button_B 6



// Número de LEDs
#define NUM_PIXELS 25

// Pino de saída
#define OUT_PIN 9

uint32_t last_print_time = 0;
ssd1306_t ssd;


// Variáveis globais para controlar o estado do botão e o LED
bool led1_state = false;   // Estado do LED1
bool last_sw_value = true; // Estado anterior do botão (inicialmente pressionado)
bool start = false;
bool stop = true;
bool led2_enabled = false; // Controle do LED2 (PWM)
bool led3_enabled = false; // Controle do LED3 (PWM)

bool cor = true;

int direcao = 0;

// Prototipação da função de interrupção
static void gpio_irq_button(uint gpio, uint32_t events);

void display()
{

    if (start == false && direcao == 0)
    {
        ssd1306_fill(&ssd, !cor);
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);
        ssd1306_draw_string(&ssd, "Motor OFF", 8, 10);
        ssd1306_draw_string(&ssd, "Set dir", 8, 20);
        ssd1306_send_data(&ssd);
    } 
    else if (start == false && direcao == 0)
    {
        ssd1306_fill(&ssd, !cor);
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);
        ssd1306_draw_string(&ssd, "Motor OFF", 8, 10);
        ssd1306_draw_string(&ssd, "Set Sentido", 8, 20);
        ssd1306_send_data(&ssd);
    } 
    else if (start == false && direcao == 12)
    {
        ssd1306_fill(&ssd, !cor);
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);
        ssd1306_draw_string(&ssd, "Motor OFF", 8, 10);
        ssd1306_draw_string(&ssd, "Sent Horario", 8, 20);
        ssd1306_send_data(&ssd);
    }
    else if (start == false && direcao == 13)
    {
        ssd1306_fill(&ssd, !cor);
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);
        ssd1306_draw_string(&ssd, "Motor OFF", 8, 10);
        ssd1306_draw_string(&ssd, "Sent Ant.Hor", 8, 20);
        ssd1306_send_data(&ssd);
    }
    else if (start == true && direcao == 12)
    {
        ssd1306_fill(&ssd, !cor);
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);
        ssd1306_draw_string(&ssd, "Motor ON", 8, 10);
        ssd1306_draw_string(&ssd, "Sent Horario", 8, 20);
        ssd1306_send_data(&ssd);
    }
    else if (start == true && direcao == 13)
    {
        ssd1306_fill(&ssd, !cor);
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);
        ssd1306_draw_string(&ssd, "Motor ON", 8, 10);
        ssd1306_draw_string(&ssd, "Sent Ant.Hor", 8, 20);
        ssd1306_send_data(&ssd);
    }
    
}

void eixo_x()
{
    if (led3_enabled == false)
    {
        adc_select_input(1);
        uint16_t vrx_value = adc_read();

        float duty_cycle = (vrx_value / 4095.0) * 100;

        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        if (current_time - last_print_time >= 1000)
        {
            printf("VRX: %u\n", vrx_value);
            printf("Duty Cycle LED: %.2f%%\n", duty_cycle);
            last_print_time = current_time;
        }
        // Controle do brilho do LED
        int led_brightness = 0;

        // Se o joystick se mover para a esquerda (vrx_value < 2048), o LED acende
        if (vrx_value < 1300)
        {
            // O brilho aumenta à medida que o joystick se move para a esquerda
            led_brightness = (2048 - vrx_value) * 4095 / 2048; // Brilho proporcional, máximo no extremo esquerdo
            direcao = LED2_PIN;
        }
        // Se o joystick se mover para a direita (vrx_value > 2048), o LED também acende
        else if (vrx_value > 2500)
        {
            // O brilho aumenta à medida que o joystick se move para a direita
            led_brightness = (vrx_value - 2048) * 4095 / 2048; // Brilho proporcional, máximo no extremo direito
            direcao = LED3_PIN;
        }

        // O brilho é limitado entre 0 e 4095
        if (led_brightness > 4095)
        {
            led_brightness = 4095;
        }
        else if (led_brightness < 0)
        {
            led_brightness = 0;
        }

        // Define o nível do PWM para o LED com base no cálculo de brilho
        pwm_set_gpio_level(direcao, led_brightness);

        printf("VRX: %u", vrx_value);
    }
}

uint pwm_init_gpio(uint gpio, uint wrap)
{
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true);
    return slice_num;
}

int main()
{

    stdio_init_all();
    adc_init();

    adc_gpio_init(VRX_PIN);
    adc_gpio_init(VRY_PIN);
    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);
    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_put(LED1_PIN, false);
    gpio_init(LED2_PIN);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
    gpio_put(LED2_PIN, false);
    gpio_init(LED3_PIN);
    gpio_set_dir(LED3_PIN, GPIO_OUT);
    gpio_put(LED3_PIN, false);
    gpio_init(button_A);
    gpio_set_dir(button_A, GPIO_IN);
    gpio_pull_up(button_A);
    gpio_init(button_B);
    gpio_set_dir(button_B, GPIO_IN);
    gpio_pull_up(button_B);


    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, 0x3C, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    // Limpa o display
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // PWM
    uint pwm_wrap = 4096;
    uint pwm_slice_x = pwm_init_gpio(LED2_PIN, pwm_wrap);
    uint pwm_slice_y = pwm_init_gpio(LED3_PIN, pwm_wrap);

    bool sw_value = gpio_get(SW_PIN) == 0;

    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(button_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_button);
    gpio_set_irq_enabled_with_callback(button_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_button);
    gpio_set_irq_enabled_with_callback(SW_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_button);

    while (true)
    {
        eixo_x();
        display();
        printf(" SW: %d\n", sw_value);

        if (direcao != 0 && start == true )
        {
            gpio_put(LED1_PIN, true);
        } else {
            gpio_put(LED1_PIN, false);
        }

        sleep_ms(500);
    }
}

// Função de interrupção com debouncing
void gpio_irq_button(uint gpio, uint32_t events)
{
    if (gpio == button_A)
    {

        // Obtém o tempo atual em microssegundos
        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        if (current_time - last_print_time >= 200)
        {
            last_print_time = current_time;

            
            if (direcao != 0)
            {
                start = true;
                led2_enabled = !led2_enabled;
                led3_enabled = !led3_enabled;
            } 
            
            printf("Button  A Pressed, LED2 state: %d\n", led2_enabled);
        }
    }
    else if (gpio == button_B)
    {
        // Obtém o tempo atual em microssegundos
        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        if (current_time - last_print_time >= 200)
        {
            last_print_time = current_time;

            led2_enabled = !led2_enabled;
            led3_enabled = !led3_enabled;
            start = false;
            direcao = 0;

            printf("Button  B Pressed, LED2 state: %d\n", led2_enabled);
        }
    }
}
