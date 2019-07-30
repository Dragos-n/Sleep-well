
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
	u8 lenght_1_u8, lenght_2_u8;
    HAL_Delay(5U);
	HAL_UART_Receive_IT(&huart1, rx_buffer_u8, 16U);

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
					SetTaskPending(Task_2);
				}
			 }
			else
			{
				if((rx_buffer_u8[lenght_2_u8 - 2] == 0x41) && (rx_buffer_u8[lenght_2_u8 - 1] == 0x4B))
				{
					data_request_u8 = STD_ON;
					memset(rx_buffer_u8, 0, 8U);
					SetTaskPending(Task_2);
				}
			}
		}
}
void Task2(void){
	HAL_ADC_Start_IT(&hadc1);
	HAL_Delay(DELAY_1_MS);
	if ((co2_measurement_counter_u16 == NUMBER_OF_MEASUREMENTS) || (STD_ON == data_request_u8))
	   {
		  count_of_complete_measurements_u16 = co2_measurement_counter_u16;
	      co2_measurement_counter_u16 = 0U;
		  SetTaskPending(Task_3);
		}
}

void Task3(void)
{
	u8 uart_buffer_u8[10];
	u8 count =0;
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

	memset(uart_buffer_u8, 0, sizeof(uart_buffer_u8));
	sprintf(uart_buffer_u8,"%d", CO2_measurement_st.measured_value_u16);
	HAL_UART_Transmit_IT(&huart1,"CO2 V value= ", 13U);
	while(uart_buffer_u8[count])
		count++;
	HAL_Delay(DELAY_5_MS);
	HAL_UART_Transmit_IT(&huart1,uart_buffer_u8,count);
	HAL_Delay(DELAY_1_MS);
	HAL_UART_Transmit_IT(&huart1,uart_new_line_u8, 2U);

	memset(uart_buffer_u8, 0, sizeof(uart_buffer_u8));
	sprintf(uart_buffer_u8,"%d", CO2_measurement_st.measurmenet_counter_u16);
	HAL_Delay(DELAY_1_MS);
	HAL_UART_Transmit_IT(&huart1,"Counter= ", 9U);
	count =0;
	while(uart_buffer_u8[count])
		count++;
	HAL_Delay(DELAY_5_MS);
	HAL_UART_Transmit_IT(&huart1,uart_buffer_u8, count);
	HAL_Delay(DELAY_1_MS);
	HAL_UART_Transmit_IT(&huart1,uart_new_line_u8, 2U);

/*
	if (CO2_measurement_st.measured_value_u16 >= GREEN_AIR_LEVEL_MIN)
		{
			HAL_UART_Transmit(&huart1, "GREEN AIR\r\n\n", 12U, HAL_MAX_DELAY);
		}
		else
			if ((CO2_measurement_st.measured_value_u16 >= GOOD_AIR_LEVEL_MIN) && (CO2_measurement_st.measured_value_u16 <= GOOD_AIR_LEVEL_MAX))
			{
				HAL_UART_Transmit(&huart1,"GOOD AIR\r\n\n", 11U, HAL_MAX_DELAY);
			}
			else
				if ((CO2_measurement_st.measured_value_u16 >= BAD_AIR_LEVEL_MIN) && (CO2_measurement_st.measured_value_u16 <= BAD_AIR_LEVEL_MAX))
				{
					HAL_UART_Transmit(&huart1,"BAD AIR\r\n\n", 10U, HAL_MAX_DELAY);
				}
				else
					if  (CO2_measurement_st.measured_value_u16 <= WORST_AIR_LEVEL_MAX)
					{
						HAL_UART_Transmit(&huart1,"VERY BAD AIR! GO OUT!\r\n\n", 24U, HAL_MAX_DELAY);
					}*/

}

void Task4(void)
{
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
		           //Prio,		Periodic,	Period,		TicksToRun,	Pending, 	Running
		/*1*/  		{0,			1,  		  10,	            0,			0,			0}, //UART data process by interrupt
		/*2*/		{0,			1,			  TASK_TIME(10),	0,			0,			0}, //ADC read
		/*3*/		{0,			0,			  0,	   			0,			0,			0}, //Average & send data to UART
		/*4*/		{0,			0,			  0,	   	  		0,			0,			0}, //
};

//=================================The schedular for the 17 task's==========================//
void os_start( void )
{
	uint8_t     TaskCounter, PrioCounter;
	while (1)
  {
		//HAL_UART_Receive_IT(&huart1, rx_buffer_u8, 16U);
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


int main(void)
{
	HAL_Init();
	SystemClock_Config();

	CO2_measurement_st.measured_value_u16 = 0U;
	CO2_measurement_st.measurmenet_counter_u16 = 0U;
	data_request_u8 = STD_OFF;
	calc_adc_val_u32 = 0U;
	co2_measurement_counter_u16 = 0U;

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

