/*
 * UART_Data.h
 *
 *	This header file defines the interface for UART communication functionalities in STM32F4xx microcontrollers.
 * 	It includes declarations for UART2 initialization, sending strings, handling data transmission and reception, and processing elements.
 * 	Additionally, it declares Modbus RTU requests for reading NPK (Nitrogen, Phosphorus, Potassium) values, crucial for applications in precision agriculture or similar fields.
 * 	These functions are fundamental for projects that require robust and efficient serial communication, such as sensor data transmission or remote command processing.
 *
 *  Created on: Dec 17, 2023
 *      Author: Unmesh
 */

#ifndef SRC_UART_DATA_H_
#define SRC_UART_DATA_H_

#include "stdint.h"

// Modbus RTU requests for reading NPK values
static uint8_t nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
static uint8_t phos[]  = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
static uint8_t pota[]  = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

void UART2_Init(void);

void UART2_SendString(char *str);

void USART2_IRQHandler(void);

void UART2_SendData(uint8_t *data, uint8_t length);

void UART2_ReceiveData(uint8_t *data, uint8_t length);

void processElement(uint8_t *element, const char *elementName);

#endif /* SRC_UART_DATA_H_ */
