#include <string.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "main.h"

#define TRUE 1
#define FALSE 0

void UART2_Init(void);
void userGPIO_Init(void);
void Error_handler(void);

UART_HandleTypeDef huart2;


int main(void)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;
	char msg[100];


	HAL_Init();
	UART2_Init();
	userGPIO_Init();

	memset(&osc_init,0,sizeof(osc_init));
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	if(HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0WS) != HAL_OK)
	{
		Error_handler();
	}

	/*---------------------------- AFTER THIS LINE SYSCLK is SOURCED BY HSE------------------*/

	__HAL_RCC_HSI_DISABLE(); //Saves some power

	/* LETS REDO THE SYSTICK CONFIGURATION */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	UART2_Init();
	userGPIO_Init();

	memset(msg,0,sizeof(msg));
	sprintf(msg,"SYSCLK : %ldHz\r\n",HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"HCLK   : %ldHz\r\n",HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK1  : %ldHz\r\n",HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK2  : %ldHz\r\n",HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);


	while(1);


	return 0;
}

void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		//there is a problem
		Error_handler();
	}
}

void userGPIO_Init(void)
{
	 GPIO_InitTypeDef user_gpio;

	 __HAL_RCC_GPIOD_CLK_ENABLE();

	 //2 . Do the pin muxing configurations
	 user_gpio.Pin = GPIO_PIN_14; //LD5
	 user_gpio.Mode =GPIO_MODE_OUTPUT_PP;
	 user_gpio.Pull = GPIO_NOPULL;
	 user_gpio.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(GPIOD,&user_gpio);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{



}

void Error_handler(void)
{
	while(1);

}


