
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "init.h"


/* Private variables ---------------------------------------------------------*/
extern huart1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void Delay(u32 nTime)
{
    TimingDelay = nTime;
    while(TimingDelay != 0)
    	{
    	u8 i = 0;
    	debug_check();
    	}
}

void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0)
    {
        TimingDelay--;
    }
}


u8 hex2str(u8 *in ,u8 len, char *out)
{
	u8 temp[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	u8 t,i,j = 0;

	for(i=0;i<len;i++)
	{
		t = (in[i] & 0xF0) >> 4;
		out[j++] = temp[t];
		t = (in[i] & 0x0F) >> 0;
		out[j++] = temp[t];
	}
	return j;
}

void debug_check(void)
{
    u8 debug_msg[10] = "Working!\r\n";
    HAL_UART_Transmit(&huart1, debug_msg, sizeof(debug_msg), HAL_MAX_DELAY);
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();

  char buffer[12] = "Board init!\r\n";

  HAL_UART_Transmit(&huart1, buffer, sizeof(buffer), HAL_MAX_DELAY);
  Delay(100);



  while (1)
  {
	  debug_check();
   //debug_msg = hex2str(TimingDelay,sizeof(TimingDelay),buf_out);
   //HAL_UART_Transmit(&huart1, debug_msg, sizeof(debug_msg), HAL_MAX_DELAY);
   //HAL_UART_Transmit(&huart1, "\r\n",2, HAL_MAX_DELAY);
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

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
