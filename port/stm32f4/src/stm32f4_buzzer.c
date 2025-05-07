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
} stm32f4_buzzer_hw_t;

/* Global variables */
static stm32f4_buzzer_hw_t buzzers_arr []= {
	[PORT_REAR_PARKING_BUZZER_ID] = {
		.p_port_buzzer = STM32F4_REAR_PARKING_BUZZER_GPIO,
		.pin_buzzer = STM32F4_REAR_PARKING_BUZZER_PIN, 
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

void _timer_pwm_config2 (uint32_t buzzer_id){
	if (buzzer_id == PORT_REAR_PARKING_BUZZER_ID){
		RCC -> APB2ENR |= RCC_APB2ENR_TIM9EN;
		
		TIM9 -> CR1 &= ~TIM_CR1_CEN;
		TIM9 -> CR1 |= TIM_CR1_ARPE;

		TIM9 -> CNT = 0;

		double sys_core_clk = (double)SystemCoreClock;

		double psc = round((((sys_core_clk)/400)/(65535.0+1.0))-1.0);
		double arr = round((((sys_core_clk)/400)/(psc+1.0))-1.0);
		if (arr > 65535.0){
			psc += 1.0;
			arr = round((((sys_core_clk)/400)/(psc+1.0))-1.0);
		}

		TIM9 -> PSC = (uint32_t)psc;
		TIM9 -> ARR = (uint32_t)arr;

		TIM9 -> CCER &= ~TIM_CCER_CC1E;

		TIM9 -> CCER &= ~TIM_CCER_CC1NP;
		TIM9 -> CCER &= ~TIM_CCER_CC1P;
		
		//enable pwm1
		TIM9 -> CCMR1 &= ~TIM_CCMR1_OC1M_0;
		TIM9 -> CCMR1 |= TIM_CCMR1_OC1M_1;
		TIM9 -> CCMR1 |= TIM_CCMR1_OC1M_2;

		
		//preload
		TIM9 -> CCMR1 |= TIM_CCMR1_OC1PE;

		TIM9 -> CCR1 = round(((TIM9->ARR)+1)/2);
		
		TIM9 -> EGR = TIM_EGR_UG;
	}
}

void port_buzzer_set_freq (uint32_t buzzer_id, buzzer_t nota){
	if (buzzer_id == PORT_REAR_PARKING_BUZZER_ID){
		TIM9 -> CR1 &= ~TIM_CR1_CEN;

		TIM9 -> CNT = 0;

		if (nota.freq == 0) return;

		//TIM9 -> CR1 |= TIM_CR1_ARPE;		

		double sys_core_clk = (double)SystemCoreClock;

		double freq = (double)nota.freq;

		double psc = round((((sys_core_clk)/freq)/(65535.0+1.0))-1.0);
		double arr = round((((sys_core_clk)/freq)/(psc+1.0))-1.0);
		if (arr > 65535.0){
			psc += 1.0;
			arr = round((((sys_core_clk)/freq)/(psc+1.0))-1.0);
		}

		TIM9 -> PSC = (uint32_t)psc;
		TIM9 -> ARR = (uint32_t)arr;

		TIM9 -> CCER &= ~TIM_CCER_CC1E;

		TIM9 -> CCER &= ~TIM_CCER_CC1NP;
		TIM9 -> CCER &= ~TIM_CCER_CC1P;
		
		//enable pwm1
		TIM9 -> CCMR1 &= ~TIM_CCMR1_OC1M_0;
		TIM9 -> CCMR1 |= TIM_CCMR1_OC1M_1;
		TIM9 -> CCMR1 |= TIM_CCMR1_OC1M_2;

		
		//preload
		TIM9 -> CCMR1 |= TIM_CCMR1_OC1PE;

		//50%
		TIM9 -> CCR1 = round(((TIM9->ARR)+1)/2);

		//enable
		TIM9 -> CCER |= TIM_CCER_CC1E;

		//actualizar valores
		TIM9 -> EGR = TIM_EGR_UG;

		TIM9 -> CR1 |= TIM_CR1_CEN;
	}
}

/* Public functions -----------------------------------------------------------*/

void port_buzzer_init (uint32_t buzzer_id){
	stm32f4_buzzer_hw_t *p_buzzer = _stm32f4_buzzer_get(buzzer_id);
	
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

	_timer_pwm_config2(buzzer_id);
	port_buzzer_set_freq(buzzer_id,(buzzer_t){600,1});
	port_system_delay_ms(1000);
	port_buzzer_set_freq(buzzer_id,(buzzer_t){0,1});
}