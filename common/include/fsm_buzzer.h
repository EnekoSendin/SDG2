/**
 * @file fsm_buzzer.h
 * @brief Header for fsm_buzzer.c file.
 * @author Eneko Emilio Sendín Gallastegi
 * @author Rodrigo Gutierrez Fontán
 * @date 2025-05-20
 */

#ifndef FSM_BUZZER_SYSTEM_H_
#define FSM_BUZZER_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "fsm.h"
/* Standard C includes */

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum  FSM_BUZZER_SYSTEM {
  QUIETO_PARAO_BUZZER = 0,
  PIPIPIPI_BUZZER,
  CALLAITO_BUZZER
};
/* Defines  ----------------------------------------------------------*/
#define DANGER_MIN_CM 0
#define WARNING_MIN_CM 25
#define NO_PROBLEM_MIN_CM 50
#define INFO_MIN_CM 150
#define OK_MIN_CM 175
#define OK_MAX_CM 200

/* Typedefs --------------------------------------------------------------------*/
typedef struct fsm_buzzer_t fsm_buzzer_t;
/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Crea un nuevo FSM buzzer
 *
 * @param buzzer_id ID del buzzer a crear
 * 
 * @return devuelve la estructura fsm
 */
fsm_buzzer_t * 	fsm_buzzer_new (uint32_t buzzer_id);
 	//Create a new buzzer FSM. 

/**
 * @brief Destruye un FSM buzzer
 *
 * @param p_fsm fsm buzzer a destruir
 */
void 	fsm_buzzer_destroy (fsm_buzzer_t *p_fsm);
 	//Destroy a buzzer FSM. 

/**
 * @brief Guarda la nueva distancia
 *
 * @param p_fsm fsm buzzer que va a guardar la distancia
 * 
 * @param distance_cm distancia que se guarda
 *
 */
void 	fsm_buzzer_set_distance (fsm_buzzer_t *p_fsm, uint32_t distance_cm);
 	//Set the buzzer system to show the distance in cm. 

/**
 * @brief Dispara la fsm del buzzer
 *
 * @param p_fsm fsm buzzer que va a dispararse
 * 
 */
void 	fsm_buzzer_fire (fsm_buzzer_t *p_fsm);
 	//Fire the buzzer FSM. 

/**
 * @brief Devuelve el estado de la fsm del buzzer
 *
 * @param p_fsm fsm buzzer 
 * @return devuelve el estado
 */
bool 	fsm_buzzer_get_status (fsm_buzzer_t *p_fsm);
 	//Get the status of the buzzer FSM. 

/**
 * @brief Actualiza el estado de la fsm del buzzer
 *
 * @param p_fsm fsm buzzer que va a actualizarse
 * @param pause indica si tiene que pararse o no
 * 
 */
void 	fsm_buzzer_set_status (fsm_buzzer_t *p_fsm, bool pause);
 	//Set the status of the buzzer FSM. 

/**
 * @brief Comprueba la actividad de la fsm del buzzer
 *
 * @param p_fsm fsm buzzer que va a dispararse
 * @return devuelve el estado actual de la fsm del buzzer
 */
bool 	fsm_buzzer_check_activity (fsm_buzzer_t *p_fsm);
 	//Check if the buzzer system is active. 
 /**
 * @brief Devuelve la fsm del buzzer
 *
 * @param p_fsm fsm buzzer
 * @return la fsm_t del buzzer
 */
fsm_t * 	fsm_buzzer_get_inner_fsm (fsm_buzzer_t *p_fsm);
 	//Get the inner FSM of the buzzer. 
 /**
 * @brief Devuelve el estado de la fsm del buzzer
 *
 * @param p_fsm fsm buzzer que va a comprobar
 * @return devuelve el estado
 */
uint32_t 	fsm_buzzer_get_state (fsm_buzzer_t *p_fsm);
 	//Get the state of the buzzer FSM. 
 /**
 * @brief Guarda el estado la fsm del buzzer
 *
 * @param p_fsm fsm buzzer que va a dispararse
 * @param state estado que va a guardarse
 */
void 	fsm_buzzer_set_state (fsm_buzzer_t *p_fsm, int8_t state);
 	//Set the state of the buzzer FSM. 
/**
 * @brief Devuelve la distancia de la fsm del buzzer
 *
 * @param p_fsm fsm buzzer
 * @return distancia que tiene guardada el buzzer
 */
uint32_t 	fsm_buzzer_get_distance (fsm_buzzer_t *p_fsm);
/**
 * @brief Cambia a pulso intermitente la fsm del buzzer
 *
 * @param p_fsm fsm buzzer que va a cambiar
 * 
 */
void fsm_buzzer_pulsed_state(fsm_buzzer_t *p_fsm);
/**
 * @brief Cambia a pulso continuo la fsm del buzzer
 *
 * @param p_fsm fsm buzzer que va a cambiar
 * 
 */
void fsm_buzzer_continuous_state(fsm_buzzer_t *p_fsm);

#endif /* FSM_BUZZER_SYSTEM_H_ */
