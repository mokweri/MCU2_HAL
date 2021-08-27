/*
 * it.c
 *
 *  Created on: 29 Jul 2021
 *      Author: OBED
 */
#include "it.h"

extern TIM_HandleTypeDef timer6;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM6_DAC_IRQHandler(void)
{
	//int processing API
	HAL_TIM_IRQHandler(&timer6);
}
