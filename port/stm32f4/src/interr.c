/**
 * @file interr.c
 * @brief Interrupt service routines for the STM32F4 platform.
 * @author Eneko Emilio Sendín Gallastegi
 * @author Rodrigo Gutierrez Fontán
 * @date 2025-03-18
 */

// Include HW dependencies:
#include "port_system.h"
#include "stm32f4_system.h"
#include <stdio.h>

// Include headers of different port elements:
#include "port_button.h"
#include "port_ultrasound.h"
#include "stm32f4_button.h"
#include "stm32f4_ultrasound.h"

//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
//------------------------------------------------------
/**
 * @brief Rutina de atencion al timer del sistema.
 *
 * @note Incrementa el contador del timer en 1.
 */
void SysTick_Handler(void)
{
	uint32_t ms_local = port_system_get_millis();
	port_system_set_millis(ms_local+1);

}

/**
 * @brief Se encarga de las interrupciones globales Px10-Px15.
 *
 * @note Primero, esta funcion identifica la linea/pin que genero la interrupcion. A continuacion, realiza la accion deseada. Antes de finalizar, limpia el registro de interrupciones pendientes.
 */

void EXTI15_10_IRQHandler ( void )
{
	port_system_systick_resume();
	/* ISR parking button */
	if ( port_button_get_pending_interrupt ( PORT_PARKING_BUTTON_ID ) ){
		if(port_button_get_value(PORT_PARKING_BUTTON_ID)){
			port_button_set_pressed(PORT_PARKING_BUTTON_ID, false);
		}else{
			port_button_set_pressed(PORT_PARKING_BUTTON_ID, true);
		}
		port_button_clear_pending_interrupt(PORT_PARKING_BUTTON_ID);
	}
}

/**
 * @brief Rutina de atencion a la interrupcion del timer 3.
 *
 * @note Controla la duracion de la señal de trigger.
 */

void TIM3_IRQHandler(){
	TIM3->SR &= ~TIM_SR_UIF;
	port_ultrasound_set_trigger_end(PORT_REAR_PARKING_SENSOR_ID,true);
}

/**
 * @brief Rutina de atencion a la interrupcion del timer 5.
 *
 * @note Controla la duracion de las mediciones del ultrasonidos.
 */

void TIM5_IRQHandler(){
	TIM5->SR &= ~TIM_SR_UIF;
	port_ultrasound_set_trigger_ready(PORT_REAR_PARKING_SENSOR_ID,true);
}

/**
 * @brief Rutina de atencion a la interrupcion del timer 2.
 *
 * @note Controla la duracion de la señal eco.
 */

void TIM2_IRQHandler(){
	port_system_systick_resume();
	if((TIM2->SR) & TIM_SR_UIF){
		port_ultrasound_set_echo_overflows(
			PORT_REAR_PARKING_SENSOR_ID,
			port_ultrasound_get_echo_overflows(PORT_REAR_PARKING_SENSOR_ID)+1
		);
		TIM2->SR &= ~TIM_SR_UIF;
	}
	
	if((TIM2->SR & TIM_SR_CC2IF) != 0){
		if((port_ultrasound_get_echo_init_tick(PORT_REAR_PARKING_SENSOR_ID) == 0) & (port_ultrasound_get_echo_end_tick(PORT_REAR_PARKING_SENSOR_ID) == 0)){
			port_ultrasound_set_echo_init_tick(PORT_REAR_PARKING_SENSOR_ID,TIM2->CCR2);
		}else{
			port_ultrasound_set_echo_end_tick(PORT_REAR_PARKING_SENSOR_ID,TIM2->CCR2);
			port_ultrasound_set_echo_received(PORT_REAR_PARKING_SENSOR_ID,true);
		}
		TIM2->SR &= ~TIM_SR_CC2IF;
	}
	
}