/*
 * Clock_config.c
 *
 *	This file contains the implementation for the system clock configuration and a simple delay function.
 * 	Clock_Init configures the system clock using the High-Speed External (HSE) oscillator and sets up the PLL for desired frequency.
 * 	The delay_ms function provides a simple software delay mechanism.
 * 	Essential for setting up the system clock for STM32F4 series microcontrollers to ensure optimal performance and timing accuracy.
 *
 *  Created on: Dec 17, 2023
 *      Author: Unmesh
 */

#include "stm32f4xx.h"
#include <stdio.h>
#include "Clock_config.h"

void Clock_Init(void) {
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY))
		;
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;
	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_3WS;
	RCC->PLLCFGR = (RCC_PLLCFGR_PLLSRC_HSE | 12 | (192 << 6) | (1 << 16) | (4 << 24));

	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY))
		;
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
		;
}

void delay_ms(uint32_t ms) {
    volatile uint32_t count = 24000 * ms;
    while (count--) {
    }
}
