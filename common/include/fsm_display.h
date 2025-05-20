/**
 * @file fsm_display.h
 * @brief Header for fsm_display.c file.
 * @author Eneko Emilio Sendín Gallastegi
 * @author Rodrigo Gutiérrez Fontán
 * @date 2025-05-20
 */

#ifndef FSM_DISPLAY_SYSTEM_H_
#define FSM_DISPLAY_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "fsm.h"
/* Standard C includes */

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
/**
* @brief estados de la máquina de estados
*/
enum  FSM_DISPLAY_SYSTEM {
  WAIT_DISPLAY = 0,
  SET_DISPLAY
};
/* Defines  ----------------------------------------------------------*/
/**
* @brief distancia a las que cambia la máquina de estados
*/
#define DANGER_MIN_CM 0
#define WARNING_MIN_CM 25
#define NO_PROBLEM_MIN_CM 50
#define INFO_MIN_CM 150
#define OK_MIN_CM 175
#define OK_MAX_CM 200
/* Typedefs --------------------------------------------------------------------*/
typedef struct fsm_display_t fsm_display_t;
/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Crea una nueva FSM display
 *
 * @param display_id ID del display a crear
 * 
 * @return devuelve la estructura fsm
 */
fsm_display_t * 	fsm_display_new (uint32_t display_id);

/**
 * @brief Destruye una FSM display
 *
 * @param p_fsm fsm display a destruir
 */
void 	fsm_display_destroy (fsm_display_t *p_fsm);

/**
 * @brief Configura el sistema de visualización para mostrar la distancia en cm.
 *
 * @param p_fsm objeto fsm display
 * @param distance_cm distancia en centimetros del ultrasound
 */
void 	fsm_display_set_distance (fsm_display_t *p_fsm, uint32_t distance_cm);

/**
 * @brief Inicializa el display
 *
 * @param p_fsm Estructura del display
 */
 
void 	fsm_display_fire (fsm_display_t *p_fsm);

/**
 * @brief Devuelve el estado de la maquina de estados
 *
 * @param p_fsm Estructura del display
 * @return El estado de la maquina de estados
 */
bool 	fsm_display_get_status (fsm_display_t *p_fsm);

/**
 * @brief Establece el estado actual del display
 *
 * @param p_fsm Estructura de ultrasonidos
 * @param status Estado de pausa
 */
void 	fsm_display_set_status (fsm_display_t *p_fsm, bool pause);

/**
 * @brief Comprueba si el display esta activo
 *
 * @param p_fsm Estructura de display
 * @return Si el display esta activo
 */
bool 	fsm_display_check_activity (fsm_display_t *p_fsm);

/**
 * @brief Devuelve el FSM del ultrasonidos
 *
 * @param p_fsm Estructura del display
 * @return El FSM del display
 */
fsm_t * 	fsm_display_get_inner_fsm (fsm_display_t *p_fsm);

/**
 * @brief Devuelve el estado del display
 *
 * @param p_fsm Estructura de display
 * @return El estado del display
 */
uint32_t 	fsm_display_get_state (fsm_display_t *p_fsm);

/**
 * @brief Establece el estado del display
 *
 * @param p_fsm Estructura de display
 * @param state Estado que se quiere establecer
 */ 
void 	fsm_display_set_state (fsm_display_t *p_fsm, int8_t state);

/**
 * @brief Devuelve la ultima distancia detectada
 *
 * @param p_fsm Estructura del display
 * @return La ultima distancia detectada
 */
uint32_t 	fsm_display_get_distance (fsm_display_t *p_fsm);

#endif /* FSM_DISPLAY_SYSTEM_H_ */
