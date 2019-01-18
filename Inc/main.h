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
