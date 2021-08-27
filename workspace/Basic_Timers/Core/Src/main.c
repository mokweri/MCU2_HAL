#include <string.h>
#include <stdio.h>
#include "main.h"

#define TRUE 1
#define FALSE 0

void Error_handler(void);
void SystemClockConfig(void);
void Timer6_Init(void);
void GPIO_Init(void);

TIM_HandleTypeDef timer6;

int main(void)
{

	HAL_Init();
	SystemClockConfig();
	GPIO_Init();
	Timer6_Init();

	//start timer
	HAL_TIM_Base_Start_IT(&timer6);

	while(1){
		//polling
//		//loop until the update event flag is set
//		while(! (TIM6->SR & TIM_SR_UIF));
//		TIM6->SR = 0;
//		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);

	}

	return 0;
}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef led;
	led.Mode = GPIO_MODE_OUTPUT_PP;
	led.Pin = GPIO_PIN_14;
	led.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOD, &led);
}
void SystemClockConfig(void)
{

}
void Error_handler(void)
{
	while(1);

}
void Timer6_Init(void)
{
	timer6.Instance = TIM6;
	timer6.Init.Prescaler = 24;
	timer6.Init.Period = 64000-1;
	if (HAL_TIM_Base_Init(&timer6) != HAL_OK)
	{
		Error_handler();
	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
}


