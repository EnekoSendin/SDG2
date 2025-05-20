/**
 * @file stm32f4_display.h
 * @brief Header for stm32f4_display.c file.
 * @author Eneko Emilio Sendín Gallastegi
 * @author Rodrigo Gutiérrez Fontán
 * @date 2025-05-20
 */
#ifndef STM32F4_DISPLAY_SYSTEM_H_
#define STM32F4_DISPLAY_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include "stm32f4xx.h"
/* HW dependent includes */

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define STM32F4_REAR_PARKING_DISPLAY_RGB_R_GPIO GPIOB /*!< PUERTO del led rojo*/
 
#define STM32F4_REAR_PARKING_DISPLAY_RGB_R_PIN 6 /*!< PIN del led rojo*/
 
#define STM32F4_REAR_PARKING_DISPLAY_RGB_G_GPIO GPIOB /*!< PUERTO del led verde*/
 
#define STM32F4_REAR_PARKING_DISPLAY_RGB_G_PIN 8 /*!< PIN del led verde*/
 
#define STM32F4_REAR_PARKING_DISPLAY_RGB_B_GPIO GPIOB /*!< PUERTO del led azul*/
 
#define STM32F4_REAR_PARKING_DISPLAY_RGB_B_PIN 9 /*!< PIN del led azul*/

#endif /* STM32F4_DISPLAY_SYSTEM_H_ */
