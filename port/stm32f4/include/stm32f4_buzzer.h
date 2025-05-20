/**
 * @file stm32f4_buzzer.h
 * @brief Header for stm32f4_buzzer.c file.
 * @author Eneko Emilio Sendín Gallastegi
 * @author Rodrigo Gutiérrez Fontán 
 * @date 2025-05-20
 */
#ifndef STM32F4_BUZZER_SYSTEM_H_
#define STM32F4_BUZZER_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include "stm32f4xx.h"
/* HW dependent includes */

/* Defines and enums ----------------------------------------------------------*/
/* Defines */

//TIM8 channel2 PC7
#define STM32F4_PARKING_BUZZER_GPIO GPIOC /*!< PUERTO del buzzer */
#define STM32F4_PARKING_BUZZER_PIN 7 /*!< PIN del buzzer */

#endif /* STM32F4_BUZZER_SYSTEM_H_ */
