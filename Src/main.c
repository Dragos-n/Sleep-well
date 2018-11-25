
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

#include "main.h"
#include "stdint.h"
#include "init.h"

volatile u8 counter= 1U;
volatile u16 a = 0U;

void Delay(u32 nTime)
{
    TimingDelay = nTime;
    while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0)
    {
        --TimingDelay;
    }
}


void debug_check(u16 printed_var)
{
	//u8 debug_msg[20] = "Working!\r\n";

	u8 debug_msg[20] = "";
	u8 uart_new_line[2] = "\r\n";
	sprintf(debug_msg, "%d", printed_var);
	HAL_UART_Transmit(&huart1,debug_msg, 6U, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1,uart_new_line, 2U, HAL_MAX_DELAY);

	sprintf(debug_msg, "%d", counter);
	HAL_UART_Transmit(&huart1,debug_msg, 6U, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1,uart_new_line, 2U, HAL_MAX_DELAY);
	++counter;

	LED_ON
	Delay(DELAY_TEST);
	LED_OFF
	Delay(DELAY_TEST);
}



int main(void)
{
	HAL_Init();
	SystemClock_Config();

#if (STD_ON == GPIO_ON)
	MX_GPIO_Init();
#endif

#if (STD_ON == ADC_1)
	MX_ADC1_Init();
#endif

#if (STD_ON == I2C_1)
	MX_I2C1_Init();
#endif

#if (STD_ON == UART_1)
	MX_USART1_UART_Init();
	Board_init();
#endif


HAL_ADC_Start(&hadc1);


	while (1)
	{
		//if (HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK)

	a = HAL_ADC_GetValue(&hadc1);

#if (STD_ON == DEBUG_MODE)
	debug_check(a);
#endif
	}

}


void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

