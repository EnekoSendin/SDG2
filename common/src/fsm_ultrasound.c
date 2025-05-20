/**
 * @file fsm_ultrasound.c
 * @brief Ultrasound sensor FSM main file.
 * @author Eneko Emilio Sendin Gallastegi
 * @author Rodrigo Gutierrez Fontan
 * @date 2025-05-20
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "port_ultrasound.h"
#include "port_system.h"
#include "fsm.h"
#include "fsm_ultrasound.h"
/* HW dependent includes */
#include <stdio.h>
/* Project includes */

/* Typedefs --------------------------------------------------------------------*/
/**
* @brief tiene una fsm_t, la distancia medida, el estado del ultrasonidos, si hay una nueva medicion o no, el id, el array de distancias medidas y el indice el array
*/
struct  	fsm_ultrasound_t
{
	fsm_t 	f;
	uint32_t 	distance_cm;
	bool 	status;
	bool 	new_measurement;
	uint32_t 	ultrasound_id;
	uint32_t 	distance_arr [FSM_ULTRASOUND_NUM_MEASUREMENTS];
	uint32_t 	distance_idx;
};

/* Private functions -----------------------------------------------------------*/
// Comparison function for qsort
/**
* @brief hace la resta entre dos valores 
* @param a valor al que se resta
* @param b valor a restar
* @return la resta de ambos valores
*/
int _compare(const void *a, const void *b){
    return (*(uint32_t *)a - *(uint32_t *)b);
}

/* State machine input or transition functions */
/**
 * @brief Verifique si el sensor de ultrasonido está activo y listo para iniciar una nueva medición.
 *
 * @param p_this objeto fsm de maquina de estados
 * 
 * @return booleano con el estado de trigger_ready
 */
static bool 	check_on (fsm_t *p_this){
	fsm_ultrasound_t *p_fsm = (fsm_ultrasound_t *)(p_this);
	return  port_ultrasound_get_trigger_ready(p_fsm->ultrasound_id);
}

/**
 * @brief Verifique si el sensor de ultrasonido se ha configurado como inactivo (OFF).
 *
 * @param p_this objeto fsm de maquina de estados
 * 
 * @return booleano con el opuesto de la variable status
 */
static bool 	check_off (fsm_t *p_this){
	fsm_ultrasound_t *p_fsm = (fsm_ultrasound_t *)(p_this);
	return !(p_fsm->status);
}

/**
 * @brief Verifique si el sensor de ultrasonido ha finalizado la señal de disparo.
 *
 * @param p_this objeto fsm de maquina de estados
 * 
 * @return booleano con el estado de trigger_end
 */
static bool 	check_trigger_end (fsm_t *p_this){
	fsm_ultrasound_t *p_fsm = (fsm_ultrasound_t *)(p_this);
	return  port_ultrasound_get_trigger_end(p_fsm->ultrasound_id);
}

/**
 * @brief Verifique si el sensor de ultrasonidos ha recibido el init (flanco ascendente en la captura de entrada) de la señal de eco.
 *
 * @param p_this objeto fsm de maquina de estados
 * 
 * @return booleano si el echo_init es mayor que 0
 */
static bool 	check_echo_init (fsm_t *p_this){
	fsm_ultrasound_t *p_fsm = (fsm_ultrasound_t *)(p_this);
	return (port_ultrasound_get_echo_init_tick(p_fsm->ultrasound_id) > 0);
}

/**
 * @brief Verifique si el sensor de ultrasonidos ha recibido el final (flanco descendente en la captura de entrada) de la señal de eco.
 *
 * @param p_this objeto fsm de maquina de estados
 * 
 * @return booleano con el estado de echo_received
 */
static bool 	check_echo_received (fsm_t *p_this){
	fsm_ultrasound_t *p_fsm = (fsm_ultrasound_t *)(p_this);
	return port_ultrasound_get_echo_received(p_fsm->ultrasound_id);
}

/**
 * @brief Comprueba si una nueva medición está lista.
 *
 * @param p_this objeto fsm de maquina de estados
 */
static bool check_new_measurement (fsm_t *p_this){
	fsm_ultrasound_t *p_fsm = (fsm_ultrasound_t *)(p_this);
	return port_ultrasound_get_trigger_ready(p_fsm->ultrasound_id);
}

/* State machine output or action functions */

/**
 * @brief Inicie una medición del transceptor de ultrasonido por primera vez después de iniciar el FSM.
 *
 * @param p_this objeto fsm de maquina de estados
 */
static void 	do_start_measurement (fsm_t *p_this){
	fsm_ultrasound_t *p_fsm = (fsm_ultrasound_t *)(p_this);
	port_ultrasound_start_measurement(p_fsm->ultrasound_id);
} 

/**
 * @brief Iniciar una nueva medición del transceptor de ultrasonidos.
 *
 * @param p_this objeto fsm de maquina de estados
 */
static void 	do_start_new_measurement (fsm_t *p_this){
	do_start_measurement(p_this);
}

/**
 * @brief Detener la señal de disparo del sensor de ultrasonidos.
 * 
 * @param p_this objeto fsm de maquina de estados
 */
static void 	do_stop_trigger (fsm_t *p_this){
	fsm_ultrasound_t *p_fsm = (fsm_ultrasound_t *)(p_this);
	port_ultrasound_stop_trigger_timer(p_fsm->ultrasound_id);
	port_ultrasound_set_trigger_end(p_fsm->ultrasound_id,false);
}

/**
 * @brief Establezca la distancia medida por el sensor de ultrasonidos.
 * 
 * @param p_this objeto fsm de maquina de estados
 */
static void 	do_set_distance (fsm_t *p_this){
	fsm_ultrasound_t *p_fsm = (fsm_ultrasound_t *)(p_this);
	double init = (double)port_ultrasound_get_echo_init_tick(p_fsm->ultrasound_id);
	double end = (double)port_ultrasound_get_echo_end_tick(p_fsm->ultrasound_id);
	double over = (double)port_ultrasound_get_echo_overflows(p_fsm->ultrasound_id);

	//printf("init: %d end:%d over:%d\n", (int)init, (int)end, (int)over);

	double tiempo = (end + over*(65535+1)-init);
	double v_son = 343;

	double distancia = (uint32_t)round(tiempo*v_son/20000);
	
	p_fsm->distance_arr[p_fsm->distance_idx] = distancia;

	if ((p_fsm->distance_idx) == 4){
		qsort(p_fsm->distance_arr, FSM_ULTRASOUND_NUM_MEASUREMENTS, sizeof(uint32_t), _compare);

		p_fsm->distance_cm = p_fsm->distance_arr[2];
		p_fsm->new_measurement = true;
	}
	p_fsm->distance_idx = (p_fsm->distance_idx + 1) % FSM_ULTRASOUND_NUM_MEASUREMENTS;
	port_ultrasound_stop_echo_timer(p_fsm->ultrasound_id);
	port_ultrasound_reset_echo_ticks(p_fsm->ultrasound_id);
}

/**
 * @brief Detener el sensor de ultrasonido.
 * 
 * @param p_this objeto fsm de maquina de estados
 */
static void 	do_stop_measurement (fsm_t *p_this){
	fsm_ultrasound_t *p_fsm = (fsm_ultrasound_t *)(p_this);
	port_ultrasound_stop_ultrasound(p_fsm->ultrasound_id);
}

/* Other auxiliary functions */
/**
 * @brief Tabla de transiciones de maquina de estados.
 */
static fsm_trans_t 	fsm_trans_ultrasound [] = {
	{WAIT_START,check_on,TRIGGER_START,do_start_measurement},
	{TRIGGER_START,check_trigger_end,WAIT_ECHO_START,do_stop_trigger},
	{WAIT_ECHO_START,check_echo_init,WAIT_ECHO_END,NULL},
	{WAIT_ECHO_END,check_echo_received,SET_DISTANCE,do_set_distance},
	{SET_DISTANCE,check_new_measurement,TRIGGER_START,do_start_new_measurement},
	{SET_DISTANCE,check_off,WAIT_START,do_stop_measurement},
	{-1,NULL,-1,NULL}
};

/**
 * @brief Inicializa el sensor de ultrasonidos
 * 
 * @param p_fsm_ultrasound Estructura del sensor de ultrasonidos
 * @param ultrasound_id ID del sensor de ultrasonidos
 */

void fsm_ultrasound_init(fsm_ultrasound_t *p_fsm_ultrasound, uint32_t ultrasound_id)
{
    // Initialize the FSM
    fsm_init(&p_fsm_ultrasound->f, fsm_trans_ultrasound);

    /* TODO alumnos: */
	// Initialize the fields of the FSM structure
	p_fsm_ultrasound->ultrasound_id = ultrasound_id;
	p_fsm_ultrasound->distance_cm = 0;
	p_fsm_ultrasound->distance_idx = 0;
	memset(p_fsm_ultrasound->distance_arr,0,FSM_ULTRASOUND_NUM_MEASUREMENTS*sizeof(uint32_t));
	p_fsm_ultrasound->status = false;
	p_fsm_ultrasound->new_measurement = false;
    port_ultrasound_init(ultrasound_id);
}

void 	fsm_ultrasound_fire (fsm_ultrasound_t *p_fsm){
	fsm_fire(&p_fsm->f);
}

void 	fsm_ultrasound_destroy (fsm_ultrasound_t *p_fsm){
	free(&p_fsm->f);
}

fsm_t * 	fsm_ultrasound_get_inner_fsm (fsm_ultrasound_t *p_fsm){
	return &(p_fsm->f);
}

uint32_t 	fsm_ultrasound_get_state (fsm_ultrasound_t *p_fsm){
	return p_fsm->f.current_state;
}

uint32_t 	fsm_ultrasound_get_distance (fsm_ultrasound_t *p_fsm){
	uint32_t dist = p_fsm->distance_cm;
	p_fsm->new_measurement = false;
	return dist; 
}

void 	fsm_ultrasound_stop (fsm_ultrasound_t *p_fsm){
	p_fsm->status = false;
	port_ultrasound_stop_ultrasound(p_fsm->ultrasound_id);
}

void 	fsm_ultrasound_start (fsm_ultrasound_t *p_fsm){
	p_fsm->status = true;
	p_fsm->distance_idx = 0;
	p_fsm->distance_cm = 0;
	port_ultrasound_reset_echo_ticks(p_fsm->ultrasound_id);
	port_ultrasound_set_trigger_ready(p_fsm->ultrasound_id,true);
	port_ultrasound_start_new_measurement_timer();
}

bool 	fsm_ultrasound_get_status (fsm_ultrasound_t *p_fsm){
	return p_fsm->status;
}

void 	fsm_ultrasound_set_status (fsm_ultrasound_t *p_fsm, bool status){
	p_fsm->status = status;
}

bool 	fsm_ultrasound_get_ready (fsm_ultrasound_t *p_fsm){
	return port_ultrasound_get_trigger_ready(p_fsm->ultrasound_id);
}

bool 	fsm_ultrasound_get_new_measurement_ready (fsm_ultrasound_t *p_fsm){
	return p_fsm->new_measurement;
}

/* Public functions -----------------------------------------------------------*/
fsm_ultrasound_t *fsm_ultrasound_new(uint32_t ultrasound_id)
{
    fsm_ultrasound_t *p_fsm_ultrasound = malloc(sizeof(fsm_ultrasound_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_ultrasound_init(p_fsm_ultrasound, ultrasound_id);                  /* Initialize the FSM */
    return p_fsm_ultrasound;
}

// Other auxiliary functions
void fsm_ultrasound_set_state(fsm_ultrasound_t *p_fsm, int8_t state)
{
    p_fsm->f.current_state = state;
}

bool 	fsm_ultrasound_check_activity (fsm_ultrasound_t *p_fsm){
	return false;
}
