#ifndef __GENERIC_H__
#define __GENERIC_H__


#define GPIOA               		((GPIO_TypeDef *) GPIOA_BASE)
#define STD_LOW         			(0x00U)
#define STD_HIGH        			(0x01U)
#define STD_OFF       				(0x00U)
#define STD_ON          			(0x01U)

#define LED_ON 						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, STD_HIGH);
#define LED_OFF						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, STD_LOW);

#define DELAY_1_MS 					(1U)
#define DELAY_10_MS 				(10U)
#define DELAY_100_MS 				(100U)
#define DELAY_1_S 					(1000U)
#define DELAY_DEBUG 				(5U)

#define DEBUG_MODE					(STD_ON)
#define GPIO_ON						(STD_ON)
#define UART_1						(STD_ON)
#define ADC_1						(STD_ON)
#define I2C_1						(STD_OFF)

#define NUMBER_OF_MEASUREMENTS      (1000U)
#define GREEN_AIR_LEVEL_MAX			(400U)
#define GREEN_AIR_LEVEL_MIN			(370U)
#define GOOD_AIR_LEVEL_MAX			(369U)
#define GOOD_AIR_LEVEL_MIN			(340U)
#define BAD_AIR_LEVEL_MAX			(339U)
#define BAD_AIR_LEVEL_MIN			(313U)
#define WORST_AIR_LEVEL_MAX			(312U)
#define WORST_AIR_LEVEL_MIN			(290U)



#define SUBSTRACT25PROCENT(number_u16) number_u16 = number_u16 - (25 * number_u16)/100


/* sint8 * \brief  Type defining an 8 bit signed integer. The possible values interval is [-128, +127].
   uint8 *  \brief  Type defining an 8 bit unsigned integer. The possible values interval is [0, +255].
   sint16 * \brief  Type defining a 16 bit signed integer. The possible values interval is [-32,768, +32,767].
   uint16 * \brief  Type defining a 16 bit unsigned integer. The possible values interval is [0, +65,535].
   sint32 * \brief  Type defining a 32 bit signed integer. The possible values interval is [-2,147,483,648, +2,147,483,647].
   uint32 * \brief  Type defining a 32 bit unsigned integer. The possible values interval is [0, +4,294,967,295].
   boolean * \brief  Type defining the boolean logic. The possible values are {FALSE, TRUE}. */
typedef signed    char           	(sint8);
typedef unsigned  char          	(uint8);
typedef signed    short          	(sint16);
typedef unsigned  short          	(uint16);
typedef signed    long           	(sint32);
typedef unsigned  long           	(uint32);
typedef unsigned  char           	(boolean);

typedef uint8_t 					(u8);
typedef uint16_t 					(u16);
typedef uint32_t 					(u32);

typedef struct Masured_value{
	__IOM u16 measured_value_u16;
	__IOM u16 measurmenet_counter_u16;
} Measured_value_tst;

#endif
