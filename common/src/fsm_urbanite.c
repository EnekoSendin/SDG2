/**
 * @file fsm_ultrasound.c
 * @brief Ultrasound sensor FSM main file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdio.h>
#include "port_system.h"
#include "fsm.h"
#include "fsm_urbanite.h"
/* HW dependent includes */

/* Project includes */

/* Typedefs --------------------------------------------------------------------*/
struct fsm_urbanite_t
{
	fsm_t f;
	fsm_button_t *p_fsm_button;
	uint32_t on_off_press_time_ms;
	uint32_t pause_display_time_ms;
	bool is_paused;
	fsm_ultrasound_t *p_fsm_ultrasound_rear;
	fsm_display_t *p_fsm_display_rear;
	fsm_buzzer_t *p_fsm_buzzer_rear;
};

/* Private functions -----------------------------------------------------------*/

static bool 	check_activity (fsm_t *p_this){
	fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
	if(fsm_button_check_activity(p_fsm->p_fsm_button) ||
	 fsm_ultrasound_check_activity(p_fsm->p_fsm_ultrasound_rear)||
	 fsm_display_check_activity(p_fsm->p_fsm_display_rear)||
	 fsm_buzzer_check_activity(p_fsm->p_fsm_buzzer_rear))
		return true;
	return false;
}//Check if any of the elements of the system is active.

static bool 	check_on (fsm_t *p_this){
	fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
	uint32_t duration = fsm_button_get_duration(p_fsm->p_fsm_button);
	if (duration>0 && duration>(p_fsm->on_off_press_time_ms))
		return true;
	return false;
}//Check if the button has been pressed for the required time to turn ON the Urbanite system.
 
static bool 	check_off (fsm_t *p_this){
	return check_on(p_this);
}//Check if the button has been pressed for the required time to turn OFF the system.

static bool 	check_new_measure (fsm_t *p_this){
	fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
	return fsm_ultrasound_get_new_measurement_ready(p_fsm->p_fsm_ultrasound_rear);
}//Check if a new measurement is ready.
 
static bool 	check_pause_display (fsm_t *p_this){
	fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
	uint32_t duration = fsm_button_get_duration(p_fsm->p_fsm_button);
	if (duration>0 && duration<(p_fsm->on_off_press_time_ms) && duration>(p_fsm->pause_display_time_ms))
		return true;
	return false;
}//Check if it has been required to pause the display.
 
static bool 	check_no_activity (fsm_t *p_this){
	return !check_activity(p_this);
}//Check if all the elements of the system are inactive.
 
static bool 	check_activity_in_measure (fsm_t *p_this){
	return check_new_measure(p_this);
}//Check if any a new measurement is ready while the system is in low power mode.
 
static void 	do_start_up_measure (fsm_t *p_this){
	fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
	fsm_button_reset_duration(p_fsm->p_fsm_button);
	fsm_ultrasound_start(p_fsm->p_fsm_ultrasound_rear);
	fsm_display_set_status(p_fsm->p_fsm_display_rear,true);
	fsm_buzzer_set_status(p_fsm->p_fsm_buzzer_rear,true);
	printf("[URBANITE][%ld] Urbanite system ON\n", port_system_get_millis());
}//Turn the Urbanite system ON.
 
static void 	do_display_distance (fsm_t *p_this){
	fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
	uint32_t distance_cm =  fsm_ultrasound_get_distance(p_fsm->p_fsm_ultrasound_rear);
	if(p_fsm->is_paused){
		if(distance_cm < WARNING_MIN_CM/2){
			fsm_display_set_distance(p_fsm->p_fsm_display_rear,distance_cm);
			fsm_buzzer_set_distance(p_fsm->p_fsm_buzzer_rear,distance_cm);
			fsm_display_set_status(p_fsm->p_fsm_display_rear,true);
			fsm_buzzer_set_status(p_fsm->p_fsm_buzzer_rear,true);
		}else{
			fsm_display_set_status(p_fsm->p_fsm_display_rear,false);
			fsm_buzzer_set_status(p_fsm->p_fsm_buzzer_rear,false);
		}
	}else{
		fsm_display_set_distance(p_fsm->p_fsm_display_rear,distance_cm);
		fsm_buzzer_set_distance(p_fsm->p_fsm_buzzer_rear,distance_cm);
	}
	printf("[URBANITE][%ld] Distance: %ld cm\n", port_system_get_millis(), distance_cm);
}//Display the distance measured by the ultrasound sensor.
 
static void 	do_pause_display (fsm_t *p_this){
	fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
	fsm_button_reset_duration(p_fsm->p_fsm_button);
	p_fsm->is_paused = !(p_fsm->is_paused);
	fsm_display_set_status(p_fsm->p_fsm_display_rear,!(p_fsm->is_paused));
	fsm_buzzer_set_status(p_fsm->p_fsm_buzzer_rear,!(p_fsm->is_paused));
	if (p_fsm->is_paused)
		printf("[URBANITE][%ld] Urbanite system display PAUSE\n", port_system_get_millis());
	else
		printf("[URBANITE][%ld] Urbanite system display RESUME\n", port_system_get_millis());
}//Pause or resume the display system.
 
static void 	do_stop_urbanite (fsm_t *p_this){
	fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
	fsm_button_reset_duration(p_fsm->p_fsm_button);
	fsm_ultrasound_stop(p_fsm->p_fsm_ultrasound_rear);
	fsm_display_set_status(p_fsm->p_fsm_display_rear,false);
	fsm_buzzer_set_status(p_fsm->p_fsm_buzzer_rear,false);
	p_fsm->is_paused = false;
	printf("[URBANITE][%ld] Urbanite system OFF\n", port_system_get_millis());
}//Turn the Urbanite system OFF.

static void 	do_sleep_off (fsm_t *p_this){
	port_system_sleep();
}//Start the low power mode while the Urbanite is OFF.

static void 	do_sleep_while_off (fsm_t *p_this){
	port_system_sleep();
}//Start the low power mode while the Urbanite is awakened by a debug breakpoint or similar in the SLEEP_WHILE_OFF state.
 
static void 	do_sleep_while_on (fsm_t *p_this){
	port_system_sleep();
}//Start the low power mode while the Urbanite is awakened by a debug breakpoint or similar in the SLEEP_WHILE_ON state.
 
static void 	do_sleep_while_measure (fsm_t *p_this){
	port_system_sleep();
}//Start the low power mode while the Urbanite is measuring the distance and it is waiting for a new measurement.

fsm_trans_t 	fsm_trans_urbanite [] = {
	{OFF,check_on,MEASURE,do_start_up_measure},
	{MEASURE,check_off,OFF,do_stop_urbanite},
	{MEASURE,check_pause_display,MEASURE,do_pause_display},
	{MEASURE,check_new_measure,MEASURE,do_display_distance},
	{MEASURE,check_no_activity,SLEEP_WHILE_ON,do_sleep_while_measure},
	{SLEEP_WHILE_ON,check_activity_in_measure,MEASURE,NULL},
	{SLEEP_WHILE_ON,check_no_activity,SLEEP_WHILE_ON,do_sleep_while_on},
	{OFF,check_no_activity,SLEEP_WHILE_OFF,do_sleep_off},
	{SLEEP_WHILE_OFF,check_activity,OFF,NULL},
	{SLEEP_WHILE_OFF,check_no_activity,SLEEP_WHILE_OFF,do_sleep_while_off},
	{-1,NULL,-1,NULL}
};
 
static void 	fsm_urbanite_init (fsm_urbanite_t *p_fsm_urbanite, fsm_button_t *p_fsm_button, uint32_t on_off_press_time_ms, uint32_t pause_display_time_ms, fsm_ultrasound_t *p_fsm_ultrasound_rear, fsm_display_t *p_fsm_display_rear, fsm_buzzer_t *p_fsm_buzzer_rear){
	fsm_init(&p_fsm_urbanite->f, fsm_trans_urbanite);
	p_fsm_urbanite->p_fsm_button = p_fsm_button;
	p_fsm_urbanite->on_off_press_time_ms = on_off_press_time_ms;
	p_fsm_urbanite->pause_display_time_ms = pause_display_time_ms;
	p_fsm_urbanite->p_fsm_ultrasound_rear = p_fsm_ultrasound_rear;
	p_fsm_urbanite->p_fsm_display_rear = p_fsm_display_rear;
	p_fsm_urbanite->p_fsm_buzzer_rear = p_fsm_buzzer_rear;
	p_fsm_urbanite->is_paused = false;
}//Create a new Urbanite FSM.
 
fsm_urbanite_t * 	fsm_urbanite_new (fsm_button_t *p_fsm_button, uint32_t on_off_press_time_ms, uint32_t pause_display_time_ms, fsm_ultrasound_t *p_fsm_ultrasound_rear, fsm_display_t *p_fsm_display_rear,fsm_buzzer_t *p_fsm_buzzer_rear){
	fsm_urbanite_t *p_fsm_urbanite = malloc(sizeof(fsm_urbanite_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_urbanite_init(p_fsm_urbanite,p_fsm_button,on_off_press_time_ms,pause_display_time_ms,p_fsm_ultrasound_rear,p_fsm_display_rear,p_fsm_buzzer_rear);                  /* Initialize the FSM */
    return p_fsm_urbanite;
}//Create a new Urbanite FSM.
 
void 	fsm_urbanite_fire (fsm_urbanite_t *p_fsm_urbanite){
	fsm_fire(&p_fsm_urbanite->f);
}//Fire the Urbanite FSM.
 
void 	fsm_urbanite_destroy (fsm_urbanite_t *p_fsm){
	free(&p_fsm->f);
}//Destroy an Urbanite FSM.