/*
 * UART_Data.c
 *
 *	This file provides the implementation for UART communication functions on STM32F4xx microcontrollers.
 * 	It includes UART2_Init for setting up UART2, UART2_SendString for transmitting strings, and UART2_SendData and UART2_ReceiveData for handling data transmission and reception.
 * 	The USART2_IRQHandler function is used for handling USART2 interrupts, primarily for data reception.
 * 	Additionally, processElement function is provided for processing and printing data received through UART.
 * 	This module is crucial for projects involving serial communication, such as interfacing with sensors or other peripherals.
 *
 *  Created on: Dec 17, 2023
 *      Author: Unmesh
 */

#include "stm32f4xx.h"
#include "Clock_config.h"
#include "UART_Data.h"
#include "stdio.h"

uint8_t Rx_Data[32];
uint8_t Rx_Index = 0;
uint8_t Tx_Data[8];

uint8_t Data_stored[10];

void UART2_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;   // Enable clocks for GPIOD and USART2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	GPIOD->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1; // Set pins PD5 and PD6 in alternate function mode for USART2
	GPIOD->AFR[0] |= (0x7 << 20) | (0x7 << 24); // Setting alternate function 7 (USART2) for PD5 and PD6

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

void USART2_IRQHandler(void) {
    if (USART2->SR & USART_SR_RXNE) {   // If character received in USART2
        Rx_Data[Rx_Index++] = USART2->DR; // Read the character and store in Rx_Data

        if (Rx_Index >= 32) {
            Rx_Index = 0; // Reset index if buffer is full
        }
    }
}

void UART2_SendData(uint8_t *data, uint8_t length) {
	// Set PD3 before sending data
	GPIOD->MODER |= GPIO_MODER_MODER3_0;  // Set PD3 as output
	GPIOD->BSRR = GPIO_BSRR_BS_3;         // Set PD3 high
	for (uint8_t i = 0; i < length; i++) {
		while (!(USART2->SR & USART_SR_TXE))
			; // Wait until transmit data register is empty
		USART2->DR = data[i];
	}
	while (!(USART2->SR & USART_SR_TC))
		; // Wait for transmission to complete

	// Reset PD3 after sending data
	GPIOD->BSRR = GPIO_BSRR_BR_3;         // Set PD3 low
}

void UART2_ReceiveData(uint8_t *data, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        if (i < Rx_Index) {
            data[i] = Rx_Data[i];
        } else {
            break;
        }
    }
    Rx_Index = 0;
}

void processElement(uint8_t *element, const char *elementName) {
    uint8_t response[32];

    UART2_SendData(element, sizeof(element));
    delay_ms(50);
    UART2_ReceiveData(response, sizeof(response));

    if (response[1] == 0x03) {
        uint16_t element_value = (response[3] << 8) | response[4];
        printf("%s Value: 0x%04X\n", elementName, element_value);
    }

    // Reset Rx_Index for next reading
    Rx_Index = 0;
}
