#include "stm32f4xx.h"
#include <stdio.h>

void UART2_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable clocks for GPIOA and USART2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1; // Set pins PA2 and PA3 in alternate function mode for USART2
	GPIOA->AFR[0] |= (0x7 << 8) | (0x7 << 12);

	USART2->BRR = 0x683;							// Baud Rate Configuration
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; // Enable transmitter, receiver, and USART2 module

	USART2->CR1 |= USART_CR1_RXNEIE; // Enable interrupt on receive data register
	NVIC_EnableIRQ(USART2_IRQn);					// Enable USART2 interrupt
}

void UART2_SendString(char *str)		// Send string via UART2
{
	while (*str) {
		while (!(USART2->SR & USART_SR_TXE))
			;	    // Wait until transmit data register is empty
		USART2->DR = *str++;
	}
}

//ADC_HandleTypeDef AdcHandle;
void ADC_Init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= 0b11 << GPIO_MODER_MODE1_Pos;
	ADC1->SMPR2 |= (0b011 << ADC_SMPR2_SMP1_Pos);
	ADC1->SQR3 |= (1 << ADC_SQR3_SQ1_Pos);
	ADC1->CR2 |= ADC_CR2_ADON;
}

uint16_t Read_ADC(void) {
	ADC1->CR2 |= ADC_CR2_SWSTART;  // Start conversion
	while (!(ADC1->SR & ADC_SR_EOC))
		;  // Wait for conversion to complete
	return (uint16_t) ADC1->DR;  // Read conversion result
}

void GPIO_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable clock for GPIOB

	// Configure PB0, PB1, and PB2 as output
	GPIOB->MODER |= (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0
			| GPIO_MODER_MODER2_0);
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 | GPIO_OTYPER_OT2); // Push-pull
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1
			| GPIO_OSPEEDER_OSPEEDR2); // High speed
	GPIOB->PUPDR &=
			~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR2); // No pull-up, pull-down

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // Enable clock for GPIOD (for PD12)

	// Configure PD12 as output
	GPIOD->MODER |= GPIO_MODER_MODER12_0;
	GPIOD->OTYPER &= ~GPIO_OTYPER_OT12; // Push-pull
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12; // High speed
	GPIOD->PUPDR &= ~GPIO_PUPDR_PUPDR12; // No pull-up, pull-down
}

void LED_Control(uint16_t sensorValue) {
	// Turn off all LEDs
	GPIOB->BSRR = GPIO_BSRR_BR0 | GPIO_BSRR_BR1 | GPIO_BSRR_BR2;

	if (sensorValue >= 0 && sensorValue < 700) {
		// Turn on LED connected to PB0
		GPIOB->BSRR = GPIO_BSRR_BS0;
	} else if (sensorValue >= 700 && sensorValue < 1600) {
		// Turn on LED connected to PB1
		GPIOB->BSRR = GPIO_BSRR_BS1;
	} else {
		// Turn on LED connected to PB2
		GPIOB->BSRR = GPIO_BSRR_BS2;
	}

	GPIOD->BSRR = GPIO_BSRR_BR12;

	if (sensorValue > 1000) {
		// Turn on the LED connected to PD12
		GPIOD->BSRR = GPIO_BSRR_BS12;
	}
}

int main(void) {
	// Initialize ADC and UART
	UART2_Init();
	ADC_Init();
	GPIO_Init();

	char buffer[50];
	while (1) {
		// Read the value from the sensor
		uint16_t sensorValue = Read_ADC();
		// Convert sensor value to a string
		sprintf(buffer, "Read Value is %d\r\n", sensorValue);
		// Send the string over UART
		UART2_SendString(buffer);

		LED_Control(sensorValue);

		// Delay
		for (uint32_t i = 0; i < 500000; i++)
			;
	}
}
