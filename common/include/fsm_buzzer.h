/**
 * @file fsm_buzzer.h
 * @brief Header for fsm_buzzer.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
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
fsm_buzzer_t * 	fsm_buzzer_new (uint32_t buzzer_id);
 	//Create a new buzzer FSM. 
 
void 	fsm_buzzer_destroy (fsm_buzzer_t *p_fsm);
 	//Destroy a buzzer FSM. 
 
void 	fsm_buzzer_set_distance (fsm_buzzer_t *p_fsm, uint32_t distance_cm);
 	//Set the buzzer system to show the distance in cm. 
 
void 	fsm_buzzer_fire (fsm_buzzer_t *p_fsm);
 	//Fire the buzzer FSM. 
 
bool 	fsm_buzzer_get_status (fsm_buzzer_t *p_fsm);
 	//Get the status of the buzzer FSM. 
 
void 	fsm_buzzer_set_status (fsm_buzzer_t *p_fsm, bool pause);
 	//Set the status of the buzzer FSM. 
 
bool 	fsm_buzzer_check_activity (fsm_buzzer_t *p_fsm);
 	//Check if the buzzer system is active. 
 
fsm_t * 	fsm_buzzer_get_inner_fsm (fsm_buzzer_t *p_fsm);
 	//Get the inner FSM of the buzzer. 
 
uint32_t 	fsm_buzzer_get_state (fsm_buzzer_t *p_fsm);
 	//Get the state of the buzzer FSM. 
 
void 	fsm_buzzer_set_state (fsm_buzzer_t *p_fsm, int8_t state);
 	//Set the state of the buzzer FSM. 

uint32_t 	fsm_buzzer_get_distance (fsm_buzzer_t *p_fsm);
#endif /* FSM_BUZZER_SYSTEM_H_ */