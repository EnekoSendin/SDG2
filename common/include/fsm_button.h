/**
 * @file fsm_button.h
 * @brief Header for fsm_button.c file.
 * @author Rodrigo Gutierrez
 * @author Eneko Emilio Sendin
 * @date 2025-03-18
 */

#ifndef FSM_BUTTON_H_
#define FSM_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
/**
 * @brief Estados de la maquina de estados
 *
 * @attention Debe estar siempre al inicio del archivo
 * 
 */
enum  	FSM_BUTTON {
	BUTTON_RELEASED = 0,
	BUTTON_RELEASED_WAIT,
	BUTTON_PRESSED,
	BUTTON_PRESSED_WAIT
};

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Se define la estructura fsm_button_t
 */
typedef struct fsm_button_t fsm_button_t;

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Crea un nuevo boton FSM
 *
 * @param debounce_time_ms Tiempo que deja de mirarse si se ha activado el boton para evitar que haya rebotes
 * @param button_id ID del boton a crear
 */
fsm_button_t * 	fsm_button_new (uint32_t debounce_time_ms, uint32_t button_id);
 
/**
 * @brief Destruye un boton
 *
 * @param p_fsm Estructura de boton
 */
 
void 	fsm_button_destroy (fsm_button_t *p_fsm);

/**
 * @brief Dispara un boton
 *
 * @param p_fsm Estructura de boton
 */
 
void 	fsm_button_fire (fsm_button_t *p_fsm);

/**
 * @brief Devuelve una estructura de boton
 *
 * @param p_fsm Estructura de boton
 * @return Estructura de boton
 */
 
fsm_t * 	fsm_button_get_inner_fsm (fsm_button_t *p_fsm);

/**
 * @brief Devuelve el estado de la maquina de estados
 *
 * @param p_fsm Estructura de boton
 * @return Estado de la maquina de estados
 */
 
uint32_t 	fsm_button_get_state (fsm_button_t *p_fsm);

/**
 * @brief Devuelve la duracion de la ultima pulsacion del boton
 *
 * @param p_fsm Estructura de boton
 * @return Duracion del boton
 */
 
uint32_t 	fsm_button_get_duration (fsm_button_t *p_fsm);

/**
 * @brief Resetea la duracion de la ultima pulsacion del boton
 *
 * @param p_fsm Estructura de boton
 */
 
void 	fsm_button_reset_duration (fsm_button_t *p_fsm);

/**
 * @brief Devuelve la duracion del tiempo para evitar que haya rebotes en el boton (debounce time)
 *
 * @param p_fsm Estructura de boton
 * @return Debounce time
 */
 
uint32_t 	fsm_button_get_debounce_time_ms (fsm_button_t *p_fsm);

/**
 * @brief Comprueba si el boton esta activo
 *
 * @param p_fsm Estructura de boton
 * @return True si el boton esta activo, false si no
 */
 
bool 	fsm_button_check_activity (fsm_button_t *p_fsm);
 
#endif