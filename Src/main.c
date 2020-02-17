
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdint.h"
#include "init.h"
#include "Generic.h"

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/


Measured_value_tst __IO CO2_measurement_st;

u16 count_of_complete_measurements_u16 = 0;
u8 uart_new_line_u8[2] = "\r\n";

void Task1(void)
{
   PIN_A_6_ON
   HAL_Delay(DELAY_5_MS);
#if (STD_ON == UART_1)
   HAL_UART_Receive_IT(&huart1, rx_buffer_u8, 10U);
#endif
   PIN_A_6_OFF
}

void Task2(void)
{
   PIN_A_7_ON
   u8 lenght_1_u8, lenght_2_u8;
   if((rx_buffer_u8[0] == 0x47) && (rx_buffer_u8[1] == 0x54))
   {
      lenght_1_u8 = rx_buffer_u8[2] - '0';
      lenght_2_u8 = rx_buffer_u8[3] - '0';
      if (0U != lenght_1_u8)
      {
        if((rx_buffer_u8[lenght_1_u8] == 0x41) && (rx_buffer_u8[lenght_2_u8] == 0x4B))
        {
           data_request_u8 = STD_ON;
           memset(rx_buffer_u8, 0, 8U);
           SetTaskPending(Task_3);
        }
      }
      else
      {
         if((rx_buffer_u8[lenght_2_u8 - 2] == 0x41) && (rx_buffer_u8[lenght_2_u8 - 1] == 0x4B))
         {
           data_request_u8 = STD_ON;
           memset(rx_buffer_u8, 0, 8U);
           SetTaskPending(Task_3);
         }
      }
   }
   PIN_A_7_OFF
}

void Task3(void)
{
   PIN_A_11_ON
   HAL_Delay(DELAY_5_MS);
   HAL_ADC_Start_IT(&hadc1);
   HAL_Delay(DELAY_5_MS);

      if ((co2_measurement_counter_u16 == NUMBER_OF_MEASUREMENTS) || (STD_ON == data_request_u8))
         {
           count_of_complete_measurements_u16 = co2_measurement_counter_u16;
           co2_measurement_counter_u16 = 0U;
           SetTaskPending(Task_4);
         }
  PIN_A_11_OFF
}

void Task4(void)
{
      LED_ON
#if (STD_ON == DEBUG_MODE)
      u8 uart_buffer_u8[10];
      u8 count =0;
#endif

      if (STD_OFF == data_request_u8)
      {
         calc_adc_val_u32 = (calc_adc_val_u32/NUMBER_OF_MEASUREMENTS);
      }
         else
         {
            calc_adc_val_u32 = (calc_adc_val_u32 / count_of_complete_measurements_u16);
            data_request_u8 = STD_OFF;
         }
       CO2_measurement_st.measured_value_u16  = (81 * calc_adc_val_u32) / 100U;
       ++CO2_measurement_st.measurmenet_counter_u16;
       calc_adc_val_u32 = 0U;

#if (STD_ON == DEBUG_MODE)
      memset(uart_buffer_u8, 0, sizeof(uart_buffer_u8));
      sprintf(uart_buffer_u8,"%d", CO2_measurement_st.measured_value_u16);
      HAL_UART_Transmit_IT(&huart1,(u8*)"CO2 V value= ", 13U);
      HAL_UART_Transmit_IT(&huart2,(u8*)"CO2 V value= ", 13U);
      while(uart_buffer_u8[count])
         count++;
      HAL_Delay(DELAY_5_MS);
      HAL_UART_Transmit_IT(&huart1,uart_buffer_u8,count);
      HAL_UART_Transmit_IT(&huart2,uart_buffer_u8,count);
      HAL_Delay(DELAY_1_MS);
      HAL_UART_Transmit_IT(&huart1,uart_new_line_u8, 2U);
      HAL_UART_Transmit_IT(&huart2,uart_new_line_u8, 2U);
#endif
      LED_OFF
}

void (*TaskList[MAX_TASK])(void)={
		//			Task function	Task id		Task prio
		Task1,
		Task2,
		Task3,
		Task4,

};

void SetTaskPending (_E_Task_Name TaskNr)
{
    if(TaskNr < MAX_TASK)
    TaskParams_u16[TaskNr-1][TASK_PENDING]=1;
}


u16 TaskParams_u16[MAX_TASK][MAX_TASK_PARAMS]={
		         //Prio,     Periodic,	    Period,		        TicksToRun,	Pending, 	Running
		/*1*/  	    {0,			1,  	     TASK_TIME(1),        0,			0,			0}, //UART data process by interrupt
		/*2*/		{0,			0,			  0,           	       0,			0,			0}, //RX buffer process
		/*3*/		{0,			1,			 TASK_TIME(3),	   0,			0,			0}, //ADC read
		/*4*/		{0,			0,			  0,         	       0,			0,			0}, //Average & send data to UART
};

//=================================The schedular for the 17 task's==========================//
void os_start( void )
{
	uint8_t     TaskCounter, PrioCounter;
	while (1)
  {
		//==================================Update Task list==============================//
		for(TaskCounter=0; TaskCounter<MAX_TASK; TaskCounter++)
		{
			if(TaskParams_u16[TaskCounter][TASK_PERIODIC])
			{
				if(TaskParams_u16[TaskCounter][TASK_PERIOD] && TaskParams_u16[TaskCounter][TASK_TTR])
				{
					--TaskParams_u16[TaskCounter][TASK_TTR];
				}
				else
				{
					TaskParams_u16[TaskCounter][TASK_TTR] = TaskParams_u16[TaskCounter][TASK_PERIOD];
					TaskParams_u16[TaskCounter][TASK_PENDING] = 1;
				}
			}
		}
		//=========================================================================//

		//====================================Tasker=================================//
		PrioCounter = 0;
		TaskCounter = 0;
		for(PrioCounter=0; PrioCounter<MAX_TASK_PRIO; PrioCounter++)
		{
			for(TaskCounter=0; TaskCounter<MAX_TASK; TaskCounter++)
			{
				if(TaskParams_u16[TaskCounter][TASK_PRIO]==PrioCounter && TaskParams_u16[TaskCounter][TASK_PENDING])
				{
					TaskParams_u16[TaskCounter][TASK_PENDING]=0;
					TaskParams_u16[TaskCounter][TASK_RUNNING]=1;
					(*TaskList[TaskCounter])();
					TaskParams_u16[TaskCounter][TASK_RUNNING]=0;
				}
			}
		}
	}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   SetTaskPending(Task_2);
}

void Var_Init(void)
{
CO2_measurement_st.measured_value_u16 = 0U;
CO2_measurement_st.measurmenet_counter_u16 = 0U;
data_request_u8 = STD_OFF;
calc_adc_val_u32 = 0U;
co2_measurement_counter_u16 = 0U;
}

int main(void)
{
   SystemClock_Config();
   HAL_Init();
   Var_Init();
   MX_NVIC_Init();

#if (STD_ON == GPIO_ON)
   MX_GPIO_Init();
#endif

#if (STD_ON == ADC_1)
   MX_ADC1_Init();
#endif

#if (STD_ON == I2C_1)
   MX_I2C1_Init();
#endif

#if (STD_ON == UART_2)
   MX_USART2_UART_Init();
#endif

#if (STD_ON == UART_1)
   MX_USART1_UART_Init();
   Board_init();
#endif

	os_start();
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

