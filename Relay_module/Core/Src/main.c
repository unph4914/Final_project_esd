#include "stm32f4xx.h"

void toggle() {
    GPIOD->ODR ^= GPIO_ODR_ODR_12;
}

// Delay function
void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms * 16000; i++) {
        __NOP();
    }
}

int main(void) {
    // Enable the clock for GPIOD
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    // Configure PD12 as output
    GPIOD->MODER |= GPIO_MODER_MODER12_0;
    GPIOD->MODER &= ~(GPIO_MODER_MODER12_1);

    while(1) {
        toggle();
        delay_ms(200); // Delay for 2 seconds
    }
}
