/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************


 Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

#include "extra.h"
#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "init.h"
#include "stdio.h"
#include "inttypes.h"
#include "generic.h"



void debug_check(u32 *var_for_debug_pu32);
void get_co2_adc(Measured_value_tst *CO2_measurement_st);
void send_data_uart(Measured_value_tst * Measured_value_pst);
void air_quality_calc(Measured_value_tst * Measured_value_pst);

typedef enum
{
    Task_0 = 0,
    Task_1,
    Task_2,
    Task_3,

}_E_Task_Name;

#define countof(a)   (sizeof(a) / sizeof(*(a)))
/* Uncomment the line below if you will use the SPI peripheral as a Master */


#define MAX_TASK		                                            17
#define MAX_TASK_PARAMS	                                            6
#define MAX_TASK_PRIO	                                            2//1		//0 is highest

#define TASK_PRIO		                                            0
#define TASK_PERIODIC	                                            1
#define TASK_PERIOD		                                            2
#define TASK_TTR		                                            3		//TicksToRun
#define TASK_PENDING	                                            4
#define TASK_RUNNING	                                            5

#define TICK	                                                    1

//-------------------Variable definitions------------------------------------------------------------//
static __IO u32 TimingDelay_u32 = STD_ON;
u16 TaskParams_u16[MAX_TASK][MAX_TASK_PARAMS];
/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
