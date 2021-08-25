#include <string.h>
#include "main.h"

#define TRUE 1
#define FALSE 0

void SystemClockConfig(void);
void UART2_Init(void);
void userGPIO_Init(void);
void Error_handler(void);
uint8_t convert_to_upper(uint8_t data);

UART_HandleTypeDef huart2;

char *user_data = "The application is ok\r\n";

uint8_t data_buffer[100];
uint8_t recvd_data;
uint32_t count=0;
uint8_t reception_complete = FALSE;

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();
	userGPIO_Init();



	uint16_t len_of_data = strlen(user_data);
	HAL_UART_Transmit(&huart2, (uint8_t*)user_data,len_of_data, HAL_MAX_DELAY);


    while(reception_complete != TRUE)
    {

    	HAL_UART_Receive_IT(&huart2,&recvd_data,1);



    }

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

	 if(recvd_data == '\r')
	 {
		 reception_complete = TRUE;
		 data_buffer[count++]='\r';
		 HAL_UART_Transmit(huart,data_buffer,count,HAL_MAX_DELAY);
	 }
	 else
	 {
		// data_buffer[count++] = recvd_data;
		 data_buffer[count++]= convert_to_upper(recvd_data);
		 HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	 }


}

void Error_handler(void)
{
	while(1);

}

void SystemClockConfig()
{
	//special system clock configuration
	//use default
}

uint8_t convert_to_upper(uint8_t data)
{
	if(data >= 'a' && data <= 'z')
	{
		data = data - ('a' - 'A');
	}

	return data;
}
