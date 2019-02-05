#ifndef __INIT_H__
#define __INIT_H__

#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "generic.h"


void SystemClock_Config(void);
#if (STD_ON == GPIO_ON)
void MX_GPIO_Init(void);
#endif

#if (STD_ON == ADC_1)
ADC_HandleTypeDef hadc1;
void MX_ADC1_Init(void);
#endif

#if (STD_ON == I2C_1)
I2C_HandleTypeDef hi2c1;
void MX_I2C1_Init(void);
#endif

#if (STD_ON == UART_1)
UART_HandleTypeDef huart1;
void MX_USART1_UART_Init(void);
void Board_init(void);
#endif

#if (STD_ON == UART_2)
UART_HandleTypeDef huart2;
void MX_USART2_UART_Init(void);
#endif

#endif /* __INIT_H__ */
