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

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	//enable the clock for timer6
	__HAL_RCC_TIM6_CLK_ENABLE();
	//enable irq of timer 6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	//set up the priority
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);

}
