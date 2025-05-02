/**
 * @file port_button.h
 * @brief Header for the portable functions to interact with the HW of the buttons. The functions must be implemented in the platform-specific code.
 * @author Eneko Emilio Sendín Gallastegi
 * @author Rodrigo Gutierrez Fontán
 * @date 2025-03-18
 */

#ifndef PORT_BUTTON_H_
#define PORT_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
/** 
 * @brief ID del boton
*/
#define PORT_PARKING_BUTTON_ID 0

/** 
 * @brief Tiempo que espera el boton para que no haya rebotes
*/
#define PORT_PARKING_BUTTON_DEBOUNCE_TIME_MS 150
// Define here all the button identifiers that are used in the system

/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Inicialice el boton
 *
 * @param p_fsm ID del boton
 */

void 	port_button_init (uint32_t button_id);

/**
 * @brief Devuelve si el boton esta pulsado
 *
 * @param p_fsm ID del boton
 * @return Si el boton esta pulsado
 */
 
bool 	port_button_get_pressed (uint32_t button_id); 

/**
 * @brief Devuelve el valor del boton
 *
 * @param p_fsm ID del boton
 * @return El valor del boton
 */
 
bool 	port_button_get_value (uint32_t button_id);

/**
 * @brief Establece si se ha pulsado el boton
 *
 * @param p_fsm ID del boton
 * @param pressed true si el boton se ha pulsado, false si se ha dejado de pulsar
 */
 
void 	port_button_set_pressed (uint32_t button_id, bool pressed);

/**
 * @brief Devuelve si el boton tiene una interrupcion pendiente
 *
 * @param p_fsm ID del boton
 * @return Si el boton tiene una interrupcion pendiente
 */
 
bool 	port_button_get_pending_interrupt (uint32_t button_id);

/**
 * @brief Establece que el boton no tiene interrupciones pendientes
 *
 * @param p_fsm ID del boton
 */
 
void 	port_button_clear_pending_interrupt (uint32_t button_id);

/**
 * @brief Establece que el boton no puede ser afectado por interrupciones
 *
 * @param p_fsm ID del boton
 */
 
void 	port_button_disable_interrupts (uint32_t button_id);
#endif