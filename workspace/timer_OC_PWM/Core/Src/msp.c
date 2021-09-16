/*
 * msp.c
 *
 *  Created on: 29 Jul 2021
 *      Author: OBED
 */
#include "main.h"

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
	 GPIO_InitTypeDef gpio_uart;
	 //here we are going to do the low level inits. of the USART2 peripheral

	 //1. enable the clock for the USART2 peripheral as well as for GPIOA peripheral
	 __HAL_RCC_USART2_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();

	 //2 . Do the pin muxing configurations
	 gpio_uart.Pin = GPIO_PIN_2;
	 gpio_uart.Mode =GPIO_MODE_AF_PP;
	 gpio_uart.Pull = GPIO_PULLUP;
	 gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	 gpio_uart.Alternate =  GPIO_AF7_USART2; //UART2_TX
	 HAL_GPIO_Init(GPIOA,&gpio_uart);

	 gpio_uart.Pin = GPIO_PIN_3; //UART2_RX
	 gpio_uart.Pull = GPIO_NOPULL;
	 HAL_GPIO_Init(GPIOA,&gpio_uart);
	 //3 . Enable the IRQ and set up the priority (NVIC settings )
	 HAL_NVIC_EnableIRQ(USART2_IRQn);
	 HAL_NVIC_SetPriority(USART2_IRQn,15,0);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	 GPIO_InitTypeDef tim2OC_ch_gpio;
	 //1. enable the peripheral clock for the timer2 peripheral
	 __HAL_RCC_TIM2_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	 __HAL_RCC_GPIOB_CLK_ENABLE();

	 //2. Configure a gpio to behave as timer2 channel 1
	 //PA0 --> tim2ch1
	 //PA1 --> tim2ch2
	 //PB10 --> tim2ch3
	 //PB11 --> tim2ch4

	 tim2OC_ch_gpio.Pin = GPIO_PIN_0|GPIO_PIN_1;
	 tim2OC_ch_gpio.Mode = GPIO_MODE_AF_PP;
	 tim2OC_ch_gpio.Pull = GPIO_NOPULL;
	 tim2OC_ch_gpio.Speed = GPIO_SPEED_FREQ_LOW;
	 tim2OC_ch_gpio.Alternate = GPIO_AF1_TIM2;
	 HAL_GPIO_Init(GPIOA,&tim2OC_ch_gpio);

	 tim2OC_ch_gpio.Pin = GPIO_PIN_10|GPIO_PIN_11;
	 tim2OC_ch_gpio.Mode = GPIO_MODE_AF_PP;
	 tim2OC_ch_gpio.Pull = GPIO_NOPULL;
	 tim2OC_ch_gpio.Speed = GPIO_SPEED_FREQ_LOW;
	 tim2OC_ch_gpio.Alternate = GPIO_AF1_TIM2;
	 HAL_GPIO_Init(GPIOB,&tim2OC_ch_gpio);

	 //3. nvic settings
	 HAL_NVIC_SetPriority(TIM2_IRQn,15,0);
	 HAL_NVIC_EnableIRQ(TIM2_IRQn);
}


