/*
 * it.c
 *
 *  Created on: 29 Jul 2021
 *      Author: OBED
 */
#include "stm32f4xx_hal.h"

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

