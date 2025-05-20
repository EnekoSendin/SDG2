/**
 * @file stm32f4_ultrasound.c
 * @brief Portable functions to interact with the ultrasound FSM library. All portable functions must be implemented in this file.
 * @author Eneko Emilio Sendín Gallastegi
 * @author Rodrigo Gutierrez Fontán
 * @date 2025-05-20
 */

/* Standard C includes */
#include <stdio.h>
#include <math.h>
#include "port_ultrasound.h"
#include "port_system.h"
#include "stm32f4_system.h"
#include "stm32f4_ultrasound.h"
/* HW dependent includes */

/* Microcontroller dependent includes */

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Esta estructura tiene: El puerto y pin de la patilla echo y el trigger, la funcion alternativa del echo, parametro trigger_ready y trigger_end que indican en el estado del trigger
 * y parametros echo_init_tick,echo_end_tick,echo_overflows que se encargan de guardar el tiempo del pulso recivido por el echo, comienzo, final y cuantas veces se ha llegado hasta el 
 * máximo del registro.
 * 
 */
typedef struct{
	GPIO_TypeDef * 	p_trigger_port;
	GPIO_TypeDef * 	p_echo_port;
	uint8_t 	trigger_pin;
	uint8_t 	echo_pin;
	uint8_t 	echo_alt_fun;
	bool 	trigger_ready;
	bool 	trigger_end;
	bool 	echo_received;
	uint32_t 	echo_init_tick;
	uint32_t 	echo_end_tick;
	uint32_t 	echo_overflows;
} stm32f4_ultrasound_hw_t;

/* Global variables */
static stm32f4_ultrasound_hw_t ultrasounds_arr[] = {
	[PORT_REAR_PARKING_SENSOR_ID] = {
		.p_trigger_port = STM32F4_REAR_PARKING_SENSOR_TRIGGER_GPIO,
		.p_echo_port = STM32F4_REAR_PARKING_SENSOR_ECHO_GPIO,
		.trigger_pin = STM32F4_REAR_PARKING_SENSOR_TRIGGER_PIN,
		.echo_pin = STM32F4_REAR_PARKING_SENSOR_ECHO_PIN,
		.echo_alt_fun = 1,
		.trigger_ready = false,
		.trigger_end = false,
		.echo_received = false,
		.echo_init_tick = 0,
		.echo_end_tick = 0,
		.echo_overflows = 0,
	},
};
/* Private functions ----------------------------------------------------------*/

/**
 * @brief Devuelve el objeto ultrasound a partir del ID
 * 
 * @param ultrasound_id ID del ultrasound
 * @return El objeto ultrasound
 */

stm32f4_ultrasound_hw_t * 	_stm32f4_ultrasound_get (uint32_t ultrasound_id){
	if (ultrasound_id < sizeof(ultrasounds_arr) / sizeof(ultrasounds_arr[0])){
        return &ultrasounds_arr[ultrasound_id];
    }
    else{
        return NULL;
    }
}

/**
 * @brief Prepara el timer del trigger
 * 
 * @note Calcula el prescaler y el arr para configurar la frecuencia del trigger
 */

static void 	_timer_trigger_setup (){
	RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3 -> CR1 &= ~TIM_CR1_CEN;
	TIM3 -> CR1 |= TIM_CR1_ARPE;

	TIM3 -> CNT = 0;

	double sys_core_clk = (double)SystemCoreClock;

	double psc = round((((sys_core_clk/1000000.0)*PORT_PARKING_SENSOR_TRIGGER_UP_US)/(65535.0+1.0))-1.0);
	double arr = round((((sys_core_clk/1000000.0)*PORT_PARKING_SENSOR_TRIGGER_UP_US)/(psc+1.0))-1.0);
	if (arr > 65535.0){
		psc += 1.0;
		arr = round((((sys_core_clk/1000000.0)*PORT_PARKING_SENSOR_TRIGGER_UP_US)/(psc+1.0))-1.0);
	}

	TIM3 -> PSC = (uint32_t)psc;
	TIM3 -> ARR = (uint32_t)arr;

	TIM3 -> EGR |= TIM_EGR_UG;
	TIM3 -> SR &= ~TIM_SR_UIF;
	TIM3 -> DIER |= TIM_DIER_UIE;

	NVIC_SetPriority(TIM3_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),4,0));

}

/**
 * @brief Prepara el timer del timeout
 * 
 * @note Calcula el prescaler y el arr para configurar el timeout
 */

static void 	_timer_new_measurement_setup (){
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

	TIM5 -> CR1 = 0x0000;

	TIM5 -> CR1 |= TIM_CR1_ARPE;

	TIM5 -> CNT = 0;

	double sys_core_clk = (double)SystemCoreClock;

	double psc = round(((sys_core_clk/1000.0)*PORT_PARKING_SENSOR_TIMEOUT_MS)/(65535.0+1.0)-1.0);
	double arr = round(((sys_core_clk/1000.0)*PORT_PARKING_SENSOR_TIMEOUT_MS)/(psc+1.0)-1.0);
	if (arr>65535.0){
		psc+=1.0;
		arr = round(((sys_core_clk/1000.0)*PORT_PARKING_SENSOR_TIMEOUT_MS)/(psc+1.0)-1.0);
	}

	TIM5 -> PSC = (uint32_t)psc;
	TIM5 -> ARR = (uint32_t)arr;

	TIM5 -> EGR |= TIM_EGR_UG;
	TIM5 -> SR &= ~TIM_SR_UIF;
	TIM5 -> DIER |= TIM_DIER_UIE;

	NVIC_SetPriority(TIM5_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));

}

/**
 * @brief Prepara el timer del echo
 * 
 * @note Calcula el prescaler y el arr para configurar el periodo del echo
 */

static void _timer_echo_setup(uint32_t ultrasound_id){
	if(ultrasound_id == PORT_REAR_PARKING_SENSOR_ID){
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
		
		TIM2 -> CR1 &= ~TIM_CR1_CEN;

		TIM2->CR1 |= TIM_CR1_ARPE;

		double sys_core_clk = (double)SystemCoreClock;

		double psc = round((((sys_core_clk/1000000.0)*65536.0)/(65535.0+1.0))-1.0);
		double arr = round((((sys_core_clk/1000000.0)*65536.0)/(psc+1.0))-1.0);
		if (arr > 65535.0){
			psc += 1.0;
			arr = round((((sys_core_clk/1000000.0)*65536.0)/(psc+1.0))-1.0);
		}

		TIM2 -> PSC = (uint32_t)psc;
		TIM2 -> ARR = (uint32_t)arr;

		
		TIM2->EGR |= TIM_EGR_UG;
	
		TIM2-> CCMR1 &= ~ TIM_CCMR1_CC2S ; /* Limpiamos para asegurar que esta a 0 */
		TIM2-> CCMR1 |= (0x1 << TIM_CCMR1_CC2S_Pos ) ;
		TIM2 -> CCMR1 &= ~ TIM_CCMR1_IC2F;
	
		TIM2 -> CCER |= (1 << TIM_CCER_CC2P_Pos | 1 << TIM_CCER_CC2NP_Pos);
	
		TIM2 -> CCMR1 &= ~(TIM_CCMR1_IC2PSC);
		TIM2 -> CCER |= TIM_CCER_CC2E ;
	
		TIM2 -> DIER |= TIM_DIER_CC2IE ; /* Interrumpe al capturar */
		TIM2 -> DIER |= TIM_DIER_UIE ; /* Interrumpe al actualizar */
	
		NVIC_SetPriority (TIM2_IRQn , NVIC_EncodePriority (NVIC_GetPriorityGrouping(),3,0));
	}
}

/* Public functions -----------------------------------------------------------*/


void port_ultrasound_init(uint32_t ultrasound_id)
{
    /* Get the ultrasound sensor */
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	
    /* TO-DO alumnos: */
	p_ultrasound -> trigger_ready = true;
	p_ultrasound -> trigger_end = false;
	p_ultrasound -> echo_received = false;
	p_ultrasound -> echo_init_tick = 0;
	p_ultrasound -> echo_end_tick = 0;
    /* Trigger pin configuration */
	stm32f4_system_gpio_config(
		p_ultrasound -> p_trigger_port,
		p_ultrasound -> trigger_pin,
		STM32F4_GPIO_MODE_OUT,
		STM32F4_GPIO_PUPDR_NOPULL
	);
    /* Echo pin configuration */
	stm32f4_system_gpio_config(
		p_ultrasound -> p_echo_port,
		p_ultrasound -> echo_pin,
		STM32F4_GPIO_MODE_AF,
		STM32F4_GPIO_PUPDR_NOPULL
	);
	stm32f4_system_gpio_config_alternate(
		p_ultrasound -> p_echo_port,
		p_ultrasound -> echo_pin,
		p_ultrasound -> echo_alt_fun
	);
    /* Configure timers */
	_timer_trigger_setup();
	_timer_new_measurement_setup();
	_timer_echo_setup(ultrasound_id);
}


// Getters and setters functions


void stm32f4_ultrasound_set_new_trigger_gpio(uint32_t ultrasound_id, GPIO_TypeDef *p_port, uint8_t pin)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->p_trigger_port = p_port;
    p_ultrasound->trigger_pin = pin;
}


void stm32f4_ultrasound_set_new_echo_gpio(uint32_t ultrasound_id, GPIO_TypeDef *p_port, uint8_t pin){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->p_echo_port = p_port;
    p_ultrasound->echo_pin = pin;
}



bool 	port_ultrasound_get_trigger_ready (uint32_t ultrasound_id){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	return(p_ultrasound->trigger_ready);
}//Get the readiness of the trigger signal. 
 
void 	port_ultrasound_set_trigger_ready (uint32_t ultrasound_id, bool trigger_ready){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	p_ultrasound->trigger_ready = trigger_ready;
}//Set the readiness of the trigger signal. 
 
bool 	port_ultrasound_get_trigger_end (uint32_t ultrasound_id){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	return(p_ultrasound->trigger_end);
}//Get the status of the trigger signal. 
 
void 	port_ultrasound_set_trigger_end (uint32_t ultrasound_id, bool trigger_end){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	p_ultrasound->trigger_end = trigger_end;
}//Set the status of the trigger signal. 
 
uint32_t 	port_ultrasound_get_echo_init_tick (uint32_t ultrasound_id){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	return(p_ultrasound->echo_init_tick);
}//Get the time tick when the init of echo signal was received. 
 
void 	port_ultrasound_set_echo_init_tick (uint32_t ultrasound_id, uint32_t echo_init_tick){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	p_ultrasound->echo_init_tick = echo_init_tick;
}//Set the time tick when the init of echo signal was received. 
 
uint32_t 	port_ultrasound_get_echo_end_tick (uint32_t ultrasound_id){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	return(p_ultrasound->echo_end_tick);
}//Get the time tick when the end of echo signal was received. 
 
void 	port_ultrasound_set_echo_end_tick (uint32_t ultrasound_id, uint32_t echo_end_tick){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	p_ultrasound->echo_end_tick = echo_end_tick;
}//Set the time tick when the end of echo signal was received. 
 
bool 	port_ultrasound_get_echo_received (uint32_t ultrasound_id){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	return(p_ultrasound->echo_received);
}//Get the status of the echo signal. 
 
void 	port_ultrasound_set_echo_received (uint32_t ultrasound_id, bool echo_received){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	p_ultrasound->echo_received = echo_received;
}//Set the status of the echo signal. 
 
uint32_t 	port_ultrasound_get_echo_overflows (uint32_t ultrasound_id){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	return(p_ultrasound->echo_overflows);
}//Get the number of overflows of the echo signal timer. 
 
void 	port_ultrasound_set_echo_overflows (uint32_t ultrasound_id, uint32_t echo_overflows){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	p_ultrasound->echo_overflows = echo_overflows;
}

// Util

void 	port_ultrasound_start_measurement (uint32_t ultrasound_id){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	p_ultrasound->trigger_ready = false;
	if (ultrasound_id == PORT_REAR_PARKING_SENSOR_ID){
		TIM2->CNT = 0;
		TIM3->CNT = 0;
	}
	TIM5->CNT = 0;
	stm32f4_system_gpio_write(
		p_ultrasound->p_trigger_port,
		p_ultrasound->trigger_pin,
		true
	);
	
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_EnableIRQ(TIM5_IRQn);

	if (ultrasound_id == PORT_REAR_PARKING_SENSOR_ID){
		TIM2->CR1 |= TIM_CR1_CEN;
		TIM3->CR1 |= TIM_CR1_CEN;
	}
	TIM5->CR1 |= TIM_CR1_CEN;
}

//Stop all the timers of the ultrasound sensor and reset the echo ticks.
void 	port_ultrasound_stop_ultrasound (uint32_t ultrasound_id){
	port_ultrasound_stop_trigger_timer(ultrasound_id);
	port_ultrasound_stop_echo_timer(ultrasound_id);
	port_ultrasound_stop_new_measurement_timer();
	port_ultrasound_reset_echo_ticks(ultrasound_id);
}
 
void 	port_ultrasound_stop_trigger_timer (uint32_t ultrasound_id){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	stm32f4_system_gpio_write(
		p_ultrasound -> p_trigger_port,
		p_ultrasound -> trigger_pin,
		false
	);
	if (ultrasound_id == PORT_REAR_PARKING_SENSOR_ID){
		TIM3->CR1 &= ~TIM_CR1_CEN;
	}
	
}//Stop the timer that controls the trigger signal.
 	 
 
void 	port_ultrasound_stop_echo_timer (uint32_t ultrasound_id){
	if (ultrasound_id == PORT_REAR_PARKING_SENSOR_ID){
		TIM2->CR1 &= ~TIM_CR1_CEN;
	}
}//Stop the timer that controls the echo signal. 

void 	port_ultrasound_reset_echo_ticks (uint32_t ultrasound_id){
	stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
	p_ultrasound -> echo_received = false;
	p_ultrasound -> echo_init_tick = 0;
	p_ultrasound -> echo_end_tick = 0;
	p_ultrasound -> echo_overflows = 0;
}//Reset the time ticks of the echo signal. 

void 	port_ultrasound_start_new_measurement_timer (void){
	NVIC_EnableIRQ(TIM5_IRQn);
	TIM5->CR1 |= TIM_CR1_CEN;
}//Start the timer that controls the new measurement. 

void 	port_ultrasound_stop_new_measurement_timer (void){
	TIM5->CR1 &= ~TIM_CR1_CEN;
}//Stop the timer that controls the new measurement. 

