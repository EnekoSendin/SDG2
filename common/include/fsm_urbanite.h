/**
 * @file fsm_ultrasound.h
 * @brief
 * @author Rodrigo Gutierrez
 * @author Eneko Emilio Sendin
 * @date 2025-03-18
 */

#ifndef FSM_URBANITE_H_
#define FSM_URBANITE_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */

#include <stdint.h>
#include "fsm_button.h"
#include "fsm_display.h"
#include "fsm_ultrasound.h"

/* Defines and enums ----------------------------------------------------------*/


/**
 * @brief Estados de la maquina de estados
 *
 * @attention Debe estar siempre al inicio del archivo
 *
 */

 enum  	FSM_URBANITE {
	OFF = 0,
	MEASURE,
	SLEEP_WHILE_OFF,
	SLEEP_WHILE_ON
  };

/* Typedefs --------------------------------------------------------------------*/

/**
 * @brief Se define la estructura 
 */

 typedef struct fsm_urbanite_t 	fsm_urbanite_t;

/* Function prototypes and explanation -------------------------------------------------*/

fsm_urbanite_t * 	fsm_urbanite_new (fsm_button_t *p_fsm_button, uint32_t on_off_press_time_ms, uint32_t pause_display_time_ms, fsm_ultrasound_t *p_fsm_ultrasound_rear, fsm_display_t *p_fsm_display_rear);
 	//Create a new Urbanite FSM.
 
void 	fsm_urbanite_fire (fsm_urbanite_t *p_fsm);
 	//Fire the Urbanite FSM.
 
void 	fsm_urbanite_destroy (fsm_urbanite_t *p_fsm);
 	//Destroy an Urbanite FSM.


#endif /* FSM_URBANITE_H_ */