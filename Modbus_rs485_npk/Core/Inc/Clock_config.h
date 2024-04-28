/*
 * Clock_config.h
 *
 *	This header file declares functions for system clock configuration and implementing a delay mechanism.
 * 	It provides the interface for Clock_Init, which sets up the microcontroller's main clock, and delay_ms, a function for creating software delays.
 * 	These functions are crucial for initializing and managing timing functionalities in embedded systems, especially for STM32F4 series microcontrollers.
 *
 *  Created on: Dec 17, 2023
 *      Author: Unmesh
 */

#ifndef SRC_CLOCK_CONFIG_H_
#define SRC_CLOCK_CONFIG_H_

#include "stdint.h"

void Clock_Init(void);

void delay_ms(uint32_t ms);


#endif /* SRC_CLOCK_CONFIG_H_ */
