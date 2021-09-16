/*
 * it.c
 *
 *  Created on: 29 Jul 2021
 *      Author: OBED
 */
#include "it.h"

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htimer2;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer2);
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);

}

