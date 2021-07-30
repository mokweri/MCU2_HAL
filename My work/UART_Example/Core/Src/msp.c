/*
 * msp.c
 *
 *  Created on: 29 Jul 2021
 *      Author: OBED
 */
#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
  /*
   * Here we do the low level processor specific inits
   */
	//1. Set up the priority grouping of the ARM cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the required system exceptions of the ARM Cortex processor
	SCB->SHCSR |= 0x7 << 16; //USGFAULTENA, BUSFAULTENA, MEMFAULTENA

	//3. Configure the priority for the system exceptions
		//0 priority by default
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0,0);
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	HAL_NVIC_SetPriority(BusFault_IRQn,0,0);

}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart2;
	//Low level initialization of the USART2 peripheral
	//1. Enable the clock for the USART and GPIOA peripheral
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//2. Do the pin muxing configurations
	gpio_uart2.Pin = GPIO_PIN_2;
	gpio_uart2.Mode = GPIO_MODE_AF_PP;
	gpio_uart2.Pull = GPIO_PULLUP;
	gpio_uart2.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart2.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &gpio_uart2);	//TX

	gpio_uart2.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio_uart2);	//RX

	//3. Enable the IRQ and set the priority (NVIC settings)
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
}