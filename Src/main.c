
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
#include "Generic.h"


Measured_value_tst * CO2_measurement_pst = NULL;
u32 calc_adc_val_u32 = 0U;
u16 local_counter_u16 = 0U;
u8 uart_new_line_u8[2] = "\r\n";


void debug_check(u32 *var_for_debug_pu32)
{
    LED_ON
	HAL_Delay(DELAY_DEBUG);
#if (STD_ON == UART_1)
	u8 debug_msg_u8[10] = "";
	sprintf(debug_msg_u8,"%d", var_for_debug_pu32);
	HAL_UART_Transmit(&huart1,debug_msg_u8, 6U, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1,uart_new_line_u8, 2U, HAL_MAX_DELAY);
#endif
	HAL_Delay(DELAY_DEBUG);
	LED_OFF
}


void get_co2_adc(Measured_value_tst * Measured_value_pst)
{
	HAL_ADC_Start(&hadc1);
	HAL_Delay(DELAY_DEBUG);
	calc_adc_val_u32  += HAL_ADC_GetValue(&hadc1);
	if (local_counter_u16 == NUMBER_OF_MEASUREMENTS)
	   {
	      LED_ON
	      calc_adc_val_u32 = (calc_adc_val_u32 / NUMBER_OF_MEASUREMENTS);
	      Measured_value_pst->measured_value_u16 = SUBSTRACT25PROCENT(calc_adc_val_u32);
	      ++Measured_value_pst->measurmenet_counter_u16;
	      calc_adc_val_u32 = 0U;
	      local_counter_u16 = 0U;
	      LED_OFF
#if (STD_ON == UART_1)
	      send_data_uart(&CO2_measurement_pst);
	      air_quality_calc(&CO2_measurement_pst);
#endif
		}
	else
	{
	      ++local_counter_u16;
	      HAL_Delay(DELAY_DEBUG);
	}


}
#if (STD_ON == UART_1)
void air_quality_calc(Measured_value_tst * Measured_value_pst)
{


	if (Measured_value_pst->measured_value_u16 >= GREEN_AIR_LEVEL_MIN)
	{
		HAL_UART_Transmit(&huart1, "GREEN AIR\r\n\n", 12U, HAL_MAX_DELAY);
	}
	else
		if ((Measured_value_pst->measured_value_u16 >= GOOD_AIR_LEVEL_MIN) && (Measured_value_pst->measured_value_u16 <= GOOD_AIR_LEVEL_MAX))
		{
			HAL_UART_Transmit(&huart1,"GOOD AIR\r\n\n", 11U, HAL_MAX_DELAY);
		}
		else
			if ((Measured_value_pst->measured_value_u16 >= BAD_AIR_LEVEL_MIN) && (Measured_value_pst->measured_value_u16 <= BAD_AIR_LEVEL_MAX))
			{
				HAL_UART_Transmit(&huart1,"BAD AIR\r\n\n", 10U, HAL_MAX_DELAY);
			}
			else
				if  (Measured_value_pst->measured_value_u16 <= WORST_AIR_LEVEL_MAX)
				{
					HAL_UART_Transmit(&huart1,"VERY BAD AIR! GO OUT!\r\n\n", 24U, HAL_MAX_DELAY);
				}


}
void send_data_uart(Measured_value_tst * Measured_value_pst)
{
	u8 uart_buffer_u8[10];
	u8 count =0;
	memset(uart_buffer_u8, 0, sizeof(uart_buffer_u8));
	sprintf(uart_buffer_u8,"%d", Measured_value_pst->measured_value_u16);
	HAL_UART_Transmit(&huart1,"CO2 V value= ", 13U, HAL_MAX_DELAY);
	while(uart_buffer_u8[count])
		count++;
	HAL_UART_Transmit(&huart1,uart_buffer_u8,count, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1,uart_new_line_u8, 2U, HAL_MAX_DELAY);
	memset(uart_buffer_u8, 0, sizeof(uart_buffer_u8));
	sprintf(uart_buffer_u8,"%d", Measured_value_pst->measurmenet_counter_u16);
	HAL_UART_Transmit(&huart1,"Counter= ", 9U, HAL_MAX_DELAY);
	count =0;
	while(uart_buffer_u8[count])
		count++;
	HAL_UART_Transmit(&huart1,uart_buffer_u8, count, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1,uart_new_line_u8, 2U, HAL_MAX_DELAY);

}
#endif

void Task1(void){}
void Task2(void){}
void Task3(void){}


void (*TaskList[MAX_TASK])(void)={
		//			Task function	Task id		Task prio
		Task1,
		Task2,
		Task3,

};

void SetTaskPending (_E_Task_Name TaskNr)
{
    if(TaskNr < MAX_TASK)
    TaskParams_u8[TaskNr-1][TASK_PENDING]=1;
}


uint8_t TaskParams_u8[MAX_TASK][MAX_TASK_PARAMS]={
		           //Prio,		Periodic,	Period,		TicksToRun,	Pending, 	Running
		/*1*/  		{0,			0,  		250,		0,			0,			0}, //Process the data in AFRxBuffer
		/*2*/		{0,			0,			250,		0,			0,			0}, //Compact and save the data in flash (has to be executed after the task that process the data from AFRxBuffer
		/*3*/		{0,			0,			255,	   	0,			0,			0}, //Watchdog Reload

};

//=================================The schedular for the 17 task's==========================//
void os_start( void )
{
	uint8_t     TaskCounter, PrioCounter;
	while (1)
	{
		getRstSource();
		//==================================Update Task list==============================//
		for(TaskCounter=0; TaskCounter<MAX_TASK; TaskCounter++)
		{
			if(TaskParams_u8[TaskCounter][TASK_PERIODIC])
			{
				if(TaskParams_u8[TaskCounter][TASK_PERIOD] && TaskParams_u8[TaskCounter][TASK_TTR])
				{
					--TaskParams_u8[TaskCounter][TASK_TTR];
				}
				else
				{
					TaskParams_u8[TaskCounter][TASK_TTR] = TaskParams_u8[TaskCounter][TASK_PERIOD];
					TaskParams_u8[TaskCounter][TASK_PENDING] = 1;
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
				if(TaskParams_u8[TaskCounter][TASK_PRIO]==PrioCounter && TaskParams_u8[TaskCounter][TASK_PENDING])
				{
					TaskParams_u8[TaskCounter][TASK_PENDING]=0;
					TaskParams_u8[TaskCounter][TASK_RUNNING]=1;
					(*TaskList[TaskCounter])();
					TaskParams_u8[TaskCounter][TASK_RUNNING]=0;
				}
			}
		}

	}
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

	while(1)
	   {
	      get_co2_adc(&CO2_measurement_pst);
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

