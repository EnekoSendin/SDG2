/**
 * @file fsm_display.h
 * @brief Header for fsm_display.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
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
enum  FSM_DISPLAY_SYSTEM {
  WAIT_DISPLAY = 0,
  SET_DISPLAY
};
/* Defines  ----------------------------------------------------------*/
#define DANGER_MIN_CM 0
#define WARNING_MIN_CM 25
#define NO_PROBLEM_MIN_CM 50
#define INFO_MIN_CM 150
#define OK_MIN_CM 175
#define OK_MAX_CM 200
/* Typedefs --------------------------------------------------------------------*/
typedef struct fsm_display_t fsm_display_t;
/* Function prototypes and explanation -------------------------------------------------*/
fsm_display_t * 	fsm_display_new (uint32_t display_id);
 	//Create a new display FSM. More...
 
void 	fsm_display_destroy (fsm_display_t *p_fsm);
 	//Destroy a display FSM. More...
 
void 	fsm_display_set_distance (fsm_display_t *p_fsm, uint32_t distance_cm);
 	//Set the display system to show the distance in cm. More...
 
void 	fsm_display_fire (fsm_display_t *p_fsm);
 	//Fire the display FSM. More...
 
bool 	fsm_display_get_status (fsm_display_t *p_fsm);
 	//Get the status of the display FSM. More...
 
void 	fsm_display_set_status (fsm_display_t *p_fsm, bool pause);
 	//Set the status of the display FSM. More...
 
bool 	fsm_display_check_activity (fsm_display_t *p_fsm);
 	//Check if the display system is active. More...
 
fsm_t * 	fsm_display_get_inner_fsm (fsm_display_t *p_fsm);
 	//Get the inner FSM of the display. More...
 
uint32_t 	fsm_display_get_state (fsm_display_t *p_fsm);
 	//Get the state of the display FSM. More...
 
void 	fsm_display_set_state (fsm_display_t *p_fsm, int8_t state);
 	//Set the state of the display FSM. More...

uint32_t 	fsm_display_get_distance (fsm_display_t *p_fsm);
#endif /* FSM_DISPLAY_SYSTEM_H_ */