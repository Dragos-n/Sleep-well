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


void Delay(u32);
void TimingDelay_Decrement(void);
void debug_check(u16 );

volatile u32 TimingDelay;


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
