/**
 * @file stm32f4_button.h
 * @brief Header for stm32f4_button.c file.
 * @author Eneko Emilio Sendín Gallastegi
 * @author Rodrigo Gutierrez Fontán
 * @date 2025-03-18
 */
#ifndef STM32F4_BUTTON_H_
#define STM32F4_BUTTON_H_
/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>

/* HW dependent includes */
#include "stm32f4xx.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define STM32F4_PARKING_BUTTON_GPIO GPIOC /*!< GPIO del boton */
#define STM32F4_PARKING_BUTTON_PIN 13 /*!< PIN del boton */


/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Funcion auxiliar para modificar el puerto y pin del botón.
 * @param button_id ID del boton a cambiar.
 * @param p_port Nuevo puerto GPIO para el botón.
 * @param pin Nuevo pin GPIO para el botón.
 *
 */
void stm32f4_button_set_new_gpio(uint32_t button_id, GPIO_TypeDef *p_port, uint8_t pin);

#endif /* STM32F4_BUTTON_H_ */