/**
 * @file fsm_urbanite.h
 * @brief Header for fsm_urbanite.c file.
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
#include "fsm_buzzer.h"
#include "fsm_ultrasound.h"

/* Defines and enums ----------------------------------------------------------*/
/**
* @brief número de estados de funcionamiento
*/
#define NUM_STATES 3

/**
* @brief estado pausado
*/
#define STATE_PAUSED 0

/**
* @brief estado de pulso discreto
*/
#define STATE_PULSED 1

/**
* @brief estado de pulso continuo
*/
#define STATE_CONTINUOUS 2

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
/**
* @brief crea un nuevo fsm urbanite
* @param p_fsm_button fsm del boton
* @param on_off_press_time_ms tiempo de pulsacion para apagar o encender
* @param pause_display_time_ms tiempo de pulsacion para parar las medidas
* @param p_fsm_ultrasound_rear fsm del ultrasonidos
* @param p_fsm_display_rear fsm del display
* @param p_fsm_buzzer_rear fsm del buzzer
* @return fsm urbanite
*/
fsm_urbanite_t * 	fsm_urbanite_new (fsm_button_t *p_fsm_button, uint32_t on_off_press_time_ms, uint32_t pause_display_time_ms, fsm_ultrasound_t *p_fsm_ultrasound_rear, fsm_display_t *p_fsm_display_rear, fsm_buzzer_t *p_fsm_buzzer_rear);


/**
* @brief dispara el fsm del urbanite
* @param p_fsm fsm urbanite
*/
void 	fsm_urbanite_fire (fsm_urbanite_t *p_fsm);


/**
* @brief destruye el fsm del urbanite
* @param p_fsm fsm urbanite
*/
void 	fsm_urbanite_destroy (fsm_urbanite_t *p_fsm);


#endif /* FSM_URBANITE_H_ */
