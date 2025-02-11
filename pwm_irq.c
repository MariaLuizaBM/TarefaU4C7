#include <stdio.h> //biblioteca padrão da linguagem C
#include "pico/stdlib.h" //subconjunto central de bibliotecas do SDK Pico
#include "pico/time.h" //biblioteca para gerenciamento de tempo
#include "hardware/irq.h" //biblioteca para gerenciamento de interrupções
#include "hardware/pwm.h" //biblioteca para controlar o hardware de PWM

#define LEDPin 12 //pino do LED conectado a GPIO como PWM
#define PWM_PIN 22 // Pino do PWM (GPIO 22)

// Configuração do PWM com interrupção
uint pwm_setup(uint pin){

    gpio_set_function(pin, GPIO_FUNC_PWM); //habilitar o pino GPIO como PWM
    uint sliceNum = pwm_gpio_to_slice_num(pin); //obter o canal PWM da GPIO

    pwm_clear_irq(sliceNum); //resetar o flag de interrupção para o slice
    pwm_set_irq_enabled(sliceNum, true); //habilitar a interrupção de PWM para um dado slice

    pwm_config config = pwm_get_default_config(); //obtem a configuração padrão para o PWM
    pwm_config_set_clkdiv(&config, 64.f); //define o divisor de clock do PWM
    pwm_init(sliceNum, &config, true); //inicializa o PWM com as configurações do objeto

    return sliceNum;
}

int main(){

    // Inicializar GPIO e PWM
    stdio_init_all();
    uint sliceNumPWM = pwm_setup(PWM_PIN); //função que inicializa o PWM para o pino PWM 22

    // Configurar frequencia do PWM para aproximadamente 50Hz
    pwm_set_wrap(sliceNumPWM, 40000); // 50Hz (20ms período)

    // Definir o ciclo de trabalho para 2.400µs (0.12%) para 180 graus
    uint16_t dutyCycle_2400 = (2400 * 40000) / 20000;
    pwm_set_gpio_level(PWM_PIN, dutyCycle_2400);

    // Aguardar 5 segundos na posição de 180 graus
    sleep_ms(5000);

    // Definir o ciclo de trabalho para 1.470µs (0.0735%) para 90 graus
    uint16_t dutyCycle_1470 = (1470 * 40000) / 20000;
    pwm_set_gpio_level(PWM_PIN, dutyCycle_1470);

    // Aguardar 5 segundos na posição de 90 graus
    sleep_ms(5000);

    // Definir o ciclo de trabalho para 500µs (0.025%) para 0 graus
    uint16_t dutyCycle_500 = (500 * 40000) / 20000;
    pwm_set_gpio_level(PWM_PIN, dutyCycle_500);

    // Aguardar 5 segundos na posição de 0 graus
    sleep_ms(5000);

    while(1){
        // Movimentação suave entre 0 e 180 graus
        for (uint16_t dc = 500; dc <= 2400; dc += 5) {
            pwm_set_gpio_level(PWM_PIN, (dc * 40000) / 20000);
            sleep_ms(10);
        }
        for (uint16_t dc = 2400; dc >= 500; dc -= 5) {
            pwm_set_gpio_level(PWM_PIN, (dc * 40000) / 20000);
            sleep_ms(10);
        }
    }
}
