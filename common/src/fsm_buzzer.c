/**
 * @file fsm_buzzer.c
 * @brief Display system FSM main file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdio.h>
#include "port_buzzer.h"
#include "port_system.h"
#include "fsm.h"
#include "fsm_buzzer.h"

/* HW dependent includes */
struct  fsm_buzzer_t
{
	fsm_t 	f;
	int32_t 	distance_cm;
	bool 	new_nota;
	bool 	status;
	bool 	idle;
	uint32_t 	buzzer_id;
	uint32_t 	counter;
	uint32_t	max;
};
/* Project includes */

/* Typedefs --------------------------------------------------------------------*/

/* Private functions -----------------------------------------------------------*/
void 	_compute_buzzer_levels (buzzer_t *p_nota,uint32_t *max, int32_t distance_cm){
	if (distance_cm>= DANGER_MIN_CM && distance_cm<=WARNING_MIN_CM){
		*p_nota = (buzzer_t){DO,1};
		*max = 0;
		return;
	}
	if (distance_cm> WARNING_MIN_CM && distance_cm<=NO_PROBLEM_MIN_CM){
		*p_nota = (buzzer_t){RE,1};
		*max = 100;
		return;
	}
	if (distance_cm> NO_PROBLEM_MIN_CM && distance_cm<=INFO_MIN_CM){
		*p_nota = (buzzer_t){MI,1};
		*max = 1000;
		return;
	}
	if (distance_cm> INFO_MIN_CM && distance_cm<=OK_MIN_CM){
		*p_nota = (buzzer_t){FA,1};
		*max = 16;
		return;
	}
	if (distance_cm> OK_MIN_CM && distance_cm<=OK_MAX_CM){
		*p_nota = (buzzer_t){SOL,1};
		*max = 20;
		return;
	}
	*p_nota = BUZZER_OFF;
}
/* State machine input or transition functions */
static bool check_buzzer_active (fsm_t *p_this){
	fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
	return p_fsm -> status;
}
static bool check_buzzer_set_new_nota (fsm_t *p_this){
	fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
	return p_fsm -> new_nota;
}
 
static bool check_buzzer_off (fsm_t *p_this){
	fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
	return !(p_fsm -> status);
}

static bool check_buzzer_off_time(fsm_t *p_this){
	fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
	p_fsm->counter+=get_port_buzzer_counter(p_fsm->buzzer_id);
	if (p_fsm->counter>(p_fsm->max)){
		port_buzzer_counter_reset(p_fsm->buzzer_id);
		//printf("ON");
		return true;
	}
	return false;
}

static bool check_buzzer_on_time (fsm_t *p_this){
	fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
	p_fsm->counter+=get_port_buzzer_counter(p_fsm->buzzer_id);
	if ((p_fsm->max!=0)&&(p_fsm->counter>(p_fsm->max))){
		port_buzzer_counter_reset(p_fsm->buzzer_id);
		//printf("ON");
		return true;
	}
	return false;
}

/* State machine output or action functions */
static void 	do_buzzer_set_on (fsm_t *p_this){
	fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
	port_buzzer_set_freq(p_fsm->buzzer_id,BUZZER_OFF);
}
 
static void 	do_buzzer_set_nota (fsm_t *p_this){
	fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
	buzzer_t nota;
	uint32_t max_pl;
	_compute_buzzer_levels(&nota,&max_pl,p_fsm->distance_cm);
	port_buzzer_set_freq(p_fsm->buzzer_id,nota);
	p_fsm->max = max_pl;
	p_fsm->new_nota = false;
	p_fsm->idle = true;
}
 
static void 	do_buzzer_set_off (fsm_t *p_this){
	fsm_buzzer_t *p_fsm = (fsm_buzzer_t *)(p_this);
	port_buzzer_set_freq(p_fsm->buzzer_id,BUZZER_OFF);
	p_fsm->idle = false;
}


/* Other auxiliary functions */
static fsm_trans_t 	fsm_trans_buzzer [] = {
	{QUIETO_PARAO_BUZZER,check_buzzer_active,PIPIPIPI_BUZZER,do_buzzer_set_on},
	{PIPIPIPI_BUZZER,check_buzzer_on_time,CALLAITO_BUZZER,do_buzzer_set_on},
	{PIPIPIPI_BUZZER,check_buzzer_set_new_nota,PIPIPIPI_BUZZER,do_buzzer_set_nota},
	{PIPIPIPI_BUZZER,check_buzzer_off,QUIETO_PARAO_BUZZER,do_buzzer_set_off},
	{CALLAITO_BUZZER,check_buzzer_off_time,PIPIPIPI_BUZZER,do_buzzer_set_nota},
	{CALLAITO_BUZZER,check_buzzer_off,QUIETO_PARAO_BUZZER,do_buzzer_set_off},
	{-1,NULL,-1,NULL}
};

/* Public functions -----------------------------------------------------------*/

static void 	fsm_buzzer_init (fsm_buzzer_t *p_fsm_buzzer, uint32_t buzzer_id){
	fsm_init((fsm_t *)p_fsm_buzzer,fsm_trans_buzzer);
	p_fsm_buzzer ->buzzer_id = buzzer_id;
	p_fsm_buzzer ->distance_cm = -1;
	p_fsm_buzzer ->idle = false;
	p_fsm_buzzer ->status = false;
	p_fsm_buzzer ->new_nota = false;
	p_fsm_buzzer ->max = 10;
	p_fsm_buzzer ->counter = 0;
	port_buzzer_init(buzzer_id);
}
 
void 	fsm_buzzer_destroy (fsm_buzzer_t *p_fsm){
	free(&p_fsm->f);
}

void 	fsm_buzzer_fire (fsm_buzzer_t *p_fsm){
	fsm_fire(&p_fsm->f);
}

fsm_t * 	fsm_buzzer_get_inner_fsm (fsm_buzzer_t *p_fsm){
	return &(p_fsm->f);
}

uint32_t 	fsm_buzzer_get_state (fsm_buzzer_t *p_fsm){
	return p_fsm -> f.current_state;
}

uint32_t 	fsm_buzzer_get_distance (fsm_buzzer_t *p_fsm){
	return p_fsm->distance_cm;
}

void 	fsm_buzzer_set_distance (fsm_buzzer_t *p_fsm, uint32_t distance_cm){
	p_fsm->distance_cm = distance_cm;
	p_fsm -> new_nota = true;
}

 
bool 	fsm_buzzer_get_status (fsm_buzzer_t *p_fsm){
	return p_fsm->status;
}


void 	fsm_buzzer_set_status (fsm_buzzer_t *p_fsm, bool status){
	p_fsm->status = status;
}

bool 	fsm_buzzer_check_activity (fsm_buzzer_t *p_fsm){
	return (!(p_fsm->idle) && (check_buzzer_active((fsm_t*)p_fsm)));
}

void 	fsm_buzzer_set_state (fsm_buzzer_t *p_fsm, int8_t state){
	p_fsm -> f.current_state = state;
}

fsm_buzzer_t *fsm_buzzer_new(uint32_t buzzer_id)
{
    fsm_buzzer_t *p_fsm_buzzer = malloc(sizeof(fsm_buzzer_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_buzzer_init(p_fsm_buzzer, buzzer_id); /* Initialize the FSM */
    return p_fsm_buzzer;
}