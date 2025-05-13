/**
 * @file stm32f4_buzzer.c
 * @brief Portable functions to interact with the buzzer system FSM library. All portable functions must be implemented in this file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Standard C includes */
#include <stdio.h>
#include <math.h>
#include "port_buzzer.h"
#include "port_system.h"
#include "stm32f4_system.h"
#include "stm32f4_buzzer.h"
/* HW dependent includes */

/* Microcontroller dependent includes */

/* Defines --------------------------------------------------------------------*/

/* Typedefs --------------------------------------------------------------------*/
typedef struct
{
    GPIO_TypeDef * p_port_buzzer;
	uint8_t pin_buzzer;
	uint32_t pipi_counter;
} stm32f4_buzzer_hw_t;

/* Global variables */
static stm32f4_buzzer_hw_t buzzers_arr []= {
	[PORT_PARKING_BUZZER_ID] = {
		.p_port_buzzer = STM32F4_PARKING_BUZZER_GPIO,
		.pin_buzzer = STM32F4_PARKING_BUZZER_PIN, 
	},
};
/* Private functions -----------------------------------------------------------*/
stm32f4_buzzer_hw_t *_stm32f4_buzzer_get(uint32_t buzzer_id){
	if (buzzer_id < sizeof(buzzers_arr) / sizeof(buzzers_arr[0])){
        return &buzzers_arr[buzzer_id];
    }
    else{
        return NULL;
    }
}

void _buzzer_timer_pwm_config (uint32_t buzzer_id){
	if (buzzer_id == PORT_PARKING_BUZZER_ID){
		//Habilitar contador TIM8
		RCC -> APB2ENR |= RCC_APB2ENR_TIM8EN;
		
		//Enable contador
		TIM8 -> CR1 &= ~TIM_CR1_CEN;

		//Habilitar preload
		TIM8 -> CR1 |= TIM_CR1_ARPE;

		//Contador a cero
		TIM8 -> CNT = 0;

		//Configurar frecuencia en registros ARR y PSC
		double sys_core_clk = (double)SystemCoreClock;

		double psc = round((((sys_core_clk)/400)/(65535.0+1.0))-1.0);
		double arr = round((((sys_core_clk)/400)/(psc+1.0))-1.0);
		if (arr > 65535.0){
			psc += 1.0;
			arr = round((((sys_core_clk)/400)/(psc+1.0))-1.0);
		}

		TIM8 -> PSC = (uint32_t)psc;
		TIM8 -> ARR = (uint32_t)arr;

		//Disable output capture en canal2
		TIM8 -> CCER &= ~TIM_CCER_CC2E;

		//Limpiar bits de polaridad de canal2
		TIM8 -> CCER &= ~TIM_CCER_CC2NP;
		TIM8 -> CCER &= ~TIM_CCER_CC2P;

		//en channel2 esta en el registro CCMR1 en CCMR2 estan ch3 y ch4
		//enable pwm1
		TIM8 -> CCMR1 &= ~TIM_CCMR1_OC2M_0;
		TIM8 -> CCMR1 |= TIM_CCMR1_OC2M_1;
		TIM8 -> CCMR1 |= TIM_CCMR1_OC2M_2;

		//preload en canal2
		TIM8 -> CCMR1 |= TIM_CCMR1_OC2PE; 

		//50% en canal 2
		TIM8 -> CCR2 = round(((TIM8->ARR)+1)/2);

		//Habilitar contador	
		TIM8 -> EGR = TIM_EGR_UG;
	}
}
static void _timer_9_setup(uint32_t buzzer_id){
	if(buzzer_id == PORT_PARKING_BUZZER_ID){
		RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
		
		TIM9 -> CR1 &= ~TIM_CR1_CEN;

		//TIM9->CR1 |= TIM_CR1_ARPE;

		double sys_core_clk = (double)SystemCoreClock;

		double psc = round((((sys_core_clk)/2.0)/(65535.0+1.0))-1.0);
		double arr = round((((sys_core_clk)/2.0)/(psc+1.0))-1.0);
		if (arr > 65535.0){
			psc += 1.0;
			arr = round((((sys_core_clk)/2.0)/(psc+1.0))-1.0);
		}

		TIM9 -> PSC = (uint32_t)psc;
		TIM9 -> ARR = (uint32_t)arr;

		
		//TIM9->EGR |= TIM_EGR_UG;
	
		TIM9 -> DIER |= TIM_DIER_UIE ; /* Interrumpe al actualizar */

		TIM9 -> CR1 |= TIM_CR1_CEN;
		TIM9 -> SR &= ~TIM_SR_UIF;
	
		NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
		NVIC_SetPriority(TIM1_BRK_TIM9_IRQn , NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,0));
	}
}
void port_buzzer_set_freq (uint32_t buzzer_id, buzzer_t nota){
	if (buzzer_id == PORT_PARKING_BUZZER_ID){
		
		//Disable timer
		TIM8 -> CR1 &= ~TIM_CR1_CEN;

		//Disable output capture en canal2
		TIM8 -> CCER &= ~TIM_CCER_CC2E;

		//Disable output mode
		TIM8->BDTR &= ~TIM_BDTR_MOE;

		//Contador a cero
		TIM8 -> CNT = 0;

		if (nota.freq == 0) return;

		//Configurar frecuencia en registros ARR y PSC
		double sys_core_clk = (double)SystemCoreClock;

		double freq = (double)nota.freq;

		double psc = round((((sys_core_clk)/freq)/(65535.0+1.0))-1.0);
		double arr = round((((sys_core_clk)/freq)/(psc+1.0))-1.0);
		if (arr > 65535.0){
			psc += 1.0;
			arr = round((((sys_core_clk)/freq)/(psc+1.0))-1.0);
		}

		TIM8 -> PSC = (uint32_t)psc;
		TIM8 -> ARR = (uint32_t)arr;

		//Limpiar bits de polaridad de canal2
		TIM8 -> CCER &= ~TIM_CCER_CC2NP;
		TIM8 -> CCER &= ~TIM_CCER_CC2P;
		
		//enable pwm mode1 de canal2
		TIM8 -> CCMR1 &= ~TIM_CCMR1_OC2M_0;
		TIM8 -> CCMR1 |= TIM_CCMR1_OC2M_1;
		TIM8 -> CCMR1 |= TIM_CCMR1_OC2M_2;

		//preload en canal2
		TIM8 -> CCMR1 |= TIM_CCMR1_OC2PE;

		//output mode activate
		TIM8->BDTR |= TIM_BDTR_MOE;

		//50% en canal 2
		TIM8 -> CCR2 = round(((TIM8->ARR)+1)/2);

		//Enable output capture en canal2
		TIM8 -> CCER |= TIM_CCER_CC2E;

		//Actualizar valores de reloj
		TIM8 -> EGR = TIM_EGR_UG;

		//Habilitar contador
		TIM8 -> CR1 |= TIM_CR1_CEN;
	}
}

/* Public functions -----------------------------------------------------------*/
void port_buzzer_counter_add(uint32_t buzzer_id){
	stm32f4_buzzer_hw_t *p_buzzer = _stm32f4_buzzer_get(buzzer_id);
	p_buzzer->pipi_counter+=1;

}
void port_buzzer_counter_reset(uint32_t buzzer_id){
	stm32f4_buzzer_hw_t *p_buzzer = _stm32f4_buzzer_get(buzzer_id);
	p_buzzer->pipi_counter=0;

}
uint32_t get_port_buzzer_counter(uint32_t buzzer_id){
	stm32f4_buzzer_hw_t *p_buzzer = _stm32f4_buzzer_get(buzzer_id);
	return p_buzzer->pipi_counter;

}
void port_buzzer_init (uint32_t buzzer_id){
	stm32f4_buzzer_hw_t *p_buzzer = _stm32f4_buzzer_get(buzzer_id);
	p_buzzer->pipi_counter=0;
	stm32f4_system_gpio_config(
		p_buzzer -> p_port_buzzer,
		p_buzzer -> pin_buzzer,
		STM32F4_GPIO_MODE_AF,
		STM32F4_GPIO_PUPDR_NOPULL
	);
	
	stm32f4_system_gpio_config_alternate(
		p_buzzer -> p_port_buzzer,
		p_buzzer -> pin_buzzer,
		3
	);

	_buzzer_timer_pwm_config(buzzer_id);
	_timer_9_setup(buzzer_id);
	
	//Check if buzzer works
	//port_buzzer_set_freq(buzzer_id,(buzzer_t){600,1});
	//port_system_delay_ms(1000);
	//port_buzzer_set_freq(buzzer_id,(buzzer_t){0,1});
}