/*
 * main.c
 *
 *	Application Point of Entry
 *
 *  Created on: Dec 17, 2023
 *      Author: Unmesh
 */

#include "stm32f4xx.h"
#include <stdio.h>
#include "Clock_config.h"
#include "UART_Data.h"

int main(void) {
    Clock_Init();
    UART2_Init();

    processElement((uint8_t*)nitro, "Nitrogen");
    processElement((uint8_t*)phos, "Phosphorus");
    processElement((uint8_t*)pota, "Potassium");

    while (1) {
    }
}

