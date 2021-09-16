#include <string.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "main.h"

#define TRUE 1
#define FALSE 0

void SystemClock_Config(uint8_t clock_freq );
void MCO_configuration(void);
void UART2_Init(void);
void Timer2_Init(void);
void Timer6_Init(void);

void userGPIO_Init(void);
void Error_handler(void);

UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer2;
TIM_HandleTypeDef htimer6;

uint32_t input_captures[2] = {0};
uint8_t count=1;
volatile uint8_t is_capture_done = FALSE;



int main(void)
{
	uint32_t capture_difference =0;
	double timer2_cnt_freq=0;
	double timer2_cnt_res=0;
	double user_signal_time_period =0;
	double user_signal_freq=0;
	char usr_msg[100];

	HAL_Init();

	SystemClock_Config(SYS_CLOCK_FREQ_50_MHZ);
	MCO_configuration();

	UART2_Init();
	Timer2_Init();
	Timer6_Init();

	userGPIO_Init();

	HAL_TIM_Base_Start_IT(&htimer6);
	HAL_TIM_IC_Start_IT(&htimer2,TIM_CHANNEL_1);

	char *user_data = "The application started\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t*)user_data, strlen(user_data), HAL_MAX_DELAY);

	while(1)
	{
		if(is_capture_done)
		{
			if(input_captures[1] > input_captures[0])
				capture_difference = input_captures[1] - input_captures[0];
			else
				capture_difference = (0XFFFFFFFF -input_captures[0]) + input_captures[1];


			timer2_cnt_freq = (HAL_RCC_GetPCLK1Freq() * 2 ) / (htimer2.Init.Prescaler + 1);
			timer2_cnt_res = 1/ timer2_cnt_freq;
			user_signal_time_period = capture_difference * timer2_cnt_res;
			user_signal_freq = 1/user_signal_time_period ;

			sprintf(usr_msg,"Frequency = %.2f Hz\r\n",user_signal_freq );
			HAL_UART_Transmit(&huart2,(uint8_t*)usr_msg,strlen(usr_msg),HAL_MAX_DELAY);

			is_capture_done = FALSE;

		}
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

void MCO_configuration(void)
{
//	RCC_OscInitTypeDef osc_init;
//
//	osc_init.OscillatorType = RCC_OSCILLATORTYPE_LSE;
//	osc_init.LSEState = RCC_LSE_ON;
//
//	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK){
//			Error_handler();
//	}

	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_HSI, RCC_MCODIV_4);

}

void Timer2_Init(void)
{
	TIM_IC_InitTypeDef timer2IC_COnfig;

	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.Prescaler = 1; //hence 25mhz
	if (HAL_TIM_IC_Init(&htimer2) != HAL_OK){
		Error_handler();
	}

	timer2IC_COnfig.ICFilter = 0;
	timer2IC_COnfig.ICPolarity = TIM_ICPOLARITY_RISING;
	timer2IC_COnfig.ICPrescaler = TIM_ICPSC_DIV1;
	timer2IC_COnfig.ICSelection = TIM_ICSELECTION_DIRECTTI;

	if (HAL_TIM_IC_ConfigChannel(&htimer2, &timer2IC_COnfig, TIM_CHANNEL_1) != HAL_OK){
		Error_handler();
	}

}
void Timer6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 9;
	htimer6.Init.Period = 50-1;
	if( HAL_TIM_Base_Init(&htimer6) != HAL_OK )
	{
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


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
#if 1
	if(! is_capture_done)
	{
		if(count == 1){
			input_captures[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count++;
		}else if(count == 2){
			input_captures[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count = 1;
			is_capture_done = TRUE;
		}
	}
#endif
}

void Error_handler(void)
{
	while(1);

}


