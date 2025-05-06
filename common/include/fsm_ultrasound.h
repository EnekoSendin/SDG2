/**
 * @file fsm_ultrasound.h
 * @brief Header for fsm_ultrasound.c file.
 * @author Rodrigo Gutierrez
 * @author Eneko Emilio Sendin
 * @date 2025-03-18
 */

#ifndef FSM_ULTRASOUND_H_
#define FSM_ULTRASOUND_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

#include "fsm.h"
/* Defines and enums ----------------------------------------------------------*/
/** 
 * @brief Numero de medidas del ultrasonidos
*/

#define 	FSM_ULTRASOUND_NUM_MEASUREMENTS   5

/**
 * @brief Estados de la maquina de estados
 *
 * @attention Debe estar siempre al inicio del archivo
 * 
 */

enum  FSM_ULTRASOUND {
	WAIT_START = 0,
	TRIGGER_START,
	WAIT_ECHO_START,
	WAIT_ECHO_END,
	SET_DISTANCE
  };

/* Typedefs --------------------------------------------------------------------*/

/**
 * @brief Se define la estructura fsm_button_t
 */

typedef struct fsm_ultrasound_t fsm_ultrasound_t;

/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Crea un nuevo ultrasonidos
 *
 * @param ultrasound_id ID del ultrasonidos a crear
 */

fsm_ultrasound_t * 	fsm_ultrasound_new (uint32_t ultrasound_id);

/**
 * @brief Destruye el ultrasonidos
 *
 * @param p_fsm Estructura de ultrasonidos
 */
 
void 	fsm_ultrasound_destroy (fsm_ultrasound_t *p_fsm);

/**
 * @brief Devuelve la ultima distancia detectada
 *
 * @param p_fsm Estructura de ultrasonidos
 * @return La ultima distancia detectada
 */
 
uint32_t 	fsm_ultrasound_get_distance (fsm_ultrasound_t *p_fsm);

/**
 * @brief Inicializa el ultrasonidos
 *
 * @param p_fsm Estructura de ultrasonidos
 */
 
void 	fsm_ultrasound_fire (fsm_ultrasound_t *p_fsm);

/**
 * @brief Devuelve el estado de la maquina de estados
 *
 * @param p_fsm Estructura de ultrasonidos
 * @return El estado de la maquina de estados
 */
 
bool 	fsm_ultrasound_get_status (fsm_ultrasound_t *p_fsm);

/**
 * @brief Establece el estado actual del ultrasonidos
 *
 * @param p_fsm Estructura de ultrasonidos
 * @param status Estado
 */
 
void 	fsm_ultrasound_set_status (fsm_ultrasound_t *p_fsm, bool status);

/**
 * @brief Devuelve el flag de preparacion del ultrasonidos
 *
 * @param p_fsm Estructura de ultrasonidos
 * @return El flag de preparacion del ultrasonidos
 */

bool 	fsm_ultrasound_get_ready (fsm_ultrasound_t *p_fsm);

/**
 * @brief Devuelve el flag que indica si hay una medicion nueva
 *
 * @param p_fsm Estructura de ultrasonidos
 * @return El flag que indica si hay una medicion nueva
 */
 
bool 	fsm_ultrasound_get_new_measurement_ready (fsm_ultrasound_t *p_fsm);

/**
 * @brief Para el ultrasonidos
 *
 * @param p_fsm Estructura de ultrasonidos
 */
 
void 	fsm_ultrasound_stop (fsm_ultrasound_t *p_fsm);
 
/**
 * @brief Inicia el ultrasonidos
 *
 * @param p_fsm Estructura de ultrasonidos
 */
 
void 	fsm_ultrasound_start (fsm_ultrasound_t *p_fsm);

/**
 * @brief Devuelve el FSM del ultrasonidos
 *
 * @param p_fsm Estructura de ultrasonidos
 * @return El FSM del ultrasonidos
 */
 
fsm_t * 	fsm_ultrasound_get_inner_fsm (fsm_ultrasound_t *p_fsm);

/**
 * @brief Devuelve el estado del ultrasonidos
 *
 * @param p_fsm Estructura de ultrasonidos
 * @return El estado del ultrasonidos
 */
 
uint32_t 	fsm_ultrasound_get_state (fsm_ultrasound_t *p_fsm);

/**
 * @brief Establece el estado del ultrasonidos
 *
 * @param p_fsm Estructura de ultrasonidos
 * @param state Estado que se quiere establecer
 */
 
void 	fsm_ultrasound_set_state (fsm_ultrasound_t *p_fsm, int8_t state);

/**
 * @brief Comprueba si el ultrasonidos esta activo
 *
 * @param p_fsm Estructura de ultrasonidos
 * @return Si el ultrasonidos esta activo
 */
 
bool 	fsm_ultrasound_check_activity (fsm_ultrasound_t *p_fsm);

#endif /* FSM_ULTRASOUND_H_ */
