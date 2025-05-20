/**
 * @file stm32f4_ultrasound.h
 * @brief Header for stm32f4_ultrasound.c file.
 * @author Eneko Emilio Sendín Gallastegi
 * @author Rodrigo Gutierrez Fontán
 * @date 2025-05-20
 */
#ifndef STM32F4_ULTRASOUND_H_
#define STM32F4_ULTRASOUND_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f4xx.h"
/* Standard C includes */

/* HW dependent includes */

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define 	STM32F4_REAR_PARKING_SENSOR_TRIGGER_GPIO  GPIOB /*!< PUERTO del trigger*/
 
#define 	STM32F4_REAR_PARKING_SENSOR_TRIGGER_PIN 0 /*!< PIN del trigger*/
 
#define 	STM32F4_REAR_PARKING_SENSOR_ECHO_GPIO GPIOA /*!< PUERTO del echo*/
 
#define 	STM32F4_REAR_PARKING_SENSOR_ECHO_PIN 1 /*!< PIN del echo*/

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Funcion auxiliar para modificar el puerto y pin del trigger del ultrasound.
 *
 * @param ultrasound_id ID de la señal trigger a cambiar.
 * @param p_port Nuevo puerto GPIO para la señal de trigger.
 * @param pin Nuevo pin GPIO para la señal de trigger.
 *
 */
void stm32f4_ultrasound_set_new_trigger_gpio(uint32_t ultrasound_id, GPIO_TypeDef *p_port, uint8_t pin);

/**
 * @brief Funcion auxiliar para modificar el puerto y pin del echo del ultrasound.
 *
 * @param ultrasound_id ID de la señal echo a cambiar.
 * @param p_port Nuevo puerto GPIO para la señal de echo.
 * @param pin Nuevo pin GPIO para la señal de echo.
 *
 */
void stm32f4_ultrasound_set_new_echo_gpio(uint32_t ultrasound_id, GPIO_TypeDef *p_port, uint8_t pin);

#endif /* STM32F4_ULTRASOUND_H_ */
