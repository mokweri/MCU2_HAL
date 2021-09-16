#include <string.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "main.h"

#define TRUE 1
#define FALSE 0

void SystemClock_Config(uint8_t clock_freq );
void UART2_Init(void);
void Timer2_Init(void);


void userGPIO_Init(void);
void Error_handler(void);

UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer2;


uint32_t pulse1_value = 25000; //500Hz
uint32_t pulse2_value = 12500; //1000Hz
uint32_t pulse3_value = 6250; //2000Hz
uint32_t pulse4_value = 3125; //4000Hz

uint32_t ccr_content;


int main(void)
{
	HAL_Init();

	SystemClock_Config(SYS_CLOCK_FREQ_50_MHZ);

	UART2_Init();
	Timer2_Init();

	userGPIO_Init();

	if (HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_1) != HAL_OK){
		Error_handler();
	}
	if (HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_2) != HAL_OK){
		Error_handler();
	}
	if (HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_3) != HAL_OK){
		Error_handler();
	}
	if (HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_4) != HAL_OK){
		Error_handler();
	}

	char *user_data = "The application started\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t*)user_data, strlen(user_data), HAL_MAX_DELAY);

	while(1)
	{

	}



	return 0;
}

void SystemClock_Config(uint8_t clock_freq )
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	uint32_t FLatency =0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_HSI;
	osc_init.HSEState = RCC_HSE_ON;
	osc_init.HSIState = RCC_HSI_ON;
	//osc_init.HSICalibrationValue = 16;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(clock_freq)
	{
		case SYS_CLOCK_FREQ_50_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 100;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
								RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_1WS;
			break;
		}
		case SYS_CLOCK_FREQ_84_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 168;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;


			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			 FLatency = FLASH_ACR_LATENCY_2WS;

			break;
		}
		case SYS_CLOCK_FREQ_120_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 240;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_3WS;
			break;

		}
		case SYS_CLOCK_FREQ_180_MHZ:
		{
			//enablw=e the clock for the power controller
			__HAL_RCC_PWR_CLK_ENABLE();
			//set regulator scale as 1
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
			//turn overdrive mode

			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 360;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_5WS;
			break;

		}

		default:
			return;
	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK){
		Error_handler();
	}

	if(HAL_RCC_ClockConfig(&clk_init,FLatency) != HAL_OK){
		Error_handler();
	}

	//Systick configuration
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}


void Timer2_Init(void)
{
	TIM_OC_InitTypeDef tim2OC_init;

	htimer2.Instance = TIM2;
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.Prescaler = 1;

	if (HAL_TIM_OC_Init(&htimer2) != HAL_OK){
		Error_handler();
	}

	tim2OC_init.OCMode = TIM_OCMODE_TOGGLE;
	tim2OC_init.OCPolarity = TIM_OCPOLARITY_HIGH;

	tim2OC_init.Pulse = pulse1_value ; ///calculated based on freq
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_1) != HAL_OK){
		Error_handler();
	}

	tim2OC_init.Pulse = pulse2_value ; ///calculated based on freq
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_2) != HAL_OK){
		Error_handler();
	}

	tim2OC_init.Pulse = pulse3_value ; ///calculated based on freq
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_3) != HAL_OK){
		Error_handler();
	}

	tim2OC_init.Pulse = pulse4_value ; ///calculated based on freq
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_4) != HAL_OK){
		Error_handler();
	}

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


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* TIM2_CH1 toggling with frequency = 500Hz */
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, ccr_content+pulse1_value);
	}

	/* TIM2_CH2 toggling with frequency = 1000Hz */
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, ccr_content+pulse2_value);
	}

	/* TIM2_CH3 toggling with frequency = 2000Hz */
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, ccr_content+pulse3_value);
	}

	/* TIM2_CH4 toggling with frequency = 4000Hz */
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, ccr_content+pulse4_value);
	}
}

void Error_handler(void)
{
	while(1);

}


