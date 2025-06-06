/**
 * @file stm32f4_display.c
 * @brief Portable functions to interact with the display system FSM library. All portable functions must be implemented in this file.
 * @author Eneko Emilio Sendín Gallastegi
 * @author Rodrigo Gutiérrez Fontán
 * @date 2025-05-20
 */

/* Standard C includes */
#include <stdio.h>
#include <math.h>
#include "port_display.h"
#include "port_system.h"
#include "stm32f4_system.h"
#include "stm32f4_display.h"
/* HW dependent includes */

/* Microcontroller dependent includes */

/* Defines --------------------------------------------------------------------*/

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Esta estructura tiene los siguientes elementos: el puerto y pin del display de cada uno de los colores rojo, verde y azul
 */
typedef struct
{
    GPIO_TypeDef * p_port_red;
	uint8_t pin_red;
	GPIO_TypeDef * p_port_green;
	uint8_t pin_green;
	GPIO_TypeDef *p_port_blue;
	uint8_t pin_blue;
} stm32f4_display_hw_t;

/* Global variables */
static stm32f4_display_hw_t displays_arr []= {
	[PORT_REAR_PARKING_DISPLAY_ID] = {
		.p_port_red = STM32F4_REAR_PARKING_DISPLAY_RGB_R_GPIO,
		.pin_red = STM32F4_REAR_PARKING_DISPLAY_RGB_R_PIN, 
		.p_port_green = STM32F4_REAR_PARKING_DISPLAY_RGB_G_GPIO,
		.pin_green = STM32F4_REAR_PARKING_DISPLAY_RGB_G_PIN,
		.p_port_blue = STM32F4_REAR_PARKING_DISPLAY_RGB_B_GPIO,
		.pin_blue = STM32F4_REAR_PARKING_DISPLAY_RGB_B_PIN, 
	},
};
/* Private functions -----------------------------------------------------------*/
stm32f4_display_hw_t *_stm32f4_display_get(uint32_t display_id){
	if (display_id < sizeof(displays_arr) / sizeof(displays_arr[0])){
        return &displays_arr[display_id];
    }
    else{
        return NULL;
    }
}

/**
 * @brief Configuración del timer que se encarga del PWM de los colores
 * @param display_id el ID del display
 */
void _timer_pwm_config (uint32_t display_id){
	if (display_id == PORT_REAR_PARKING_DISPLAY_ID){
		RCC -> APB1ENR |= RCC_APB1ENR_TIM4EN;
		TIM4 -> CR1 &= ~TIM_CR1_CEN;
		TIM4 -> CR1 |= TIM_CR1_ARPE;

		TIM4 -> CNT = 0;

		double sys_core_clk = (double)SystemCoreClock;

		double psc = round((((sys_core_clk)/50)/(65535.0+1.0))-1.0);
		double arr = round((((sys_core_clk)/50)/(psc+1.0))-1.0);
		if (arr > 65535.0){
			psc += 1.0;
			arr = round((((sys_core_clk)/50)/(psc+1.0))-1.0);
		}

		TIM4 -> PSC = (uint32_t)psc;
		TIM4 -> ARR = (uint32_t)arr;

		TIM4 -> CCER &= ~TIM_CCER_CC1E;
		TIM4 -> CCER &= ~TIM_CCER_CC3E;
		TIM4 -> CCER &= ~TIM_CCER_CC4E;

		TIM4 -> CCER &= ~TIM_CCER_CC1NP;
		TIM4 -> CCER &= ~TIM_CCER_CC1P;
		TIM4 -> CCER &= ~TIM_CCER_CC3NP;
		TIM4 -> CCER &= ~TIM_CCER_CC3P;
		TIM4 -> CCER &= ~TIM_CCER_CC4NP;
		TIM4 -> CCER &= ~TIM_CCER_CC4P;

		TIM4 -> CCMR1 &= ~TIM_CCMR1_OC1M_0;
		TIM4 -> CCMR1 |= TIM_CCMR1_OC1M_1;
		TIM4 -> CCMR1 |= TIM_CCMR1_OC1M_2;

		TIM4 -> CCMR2 &= ~TIM_CCMR2_OC3M_0;
		TIM4 -> CCMR2 |= TIM_CCMR2_OC3M_1;
		TIM4 -> CCMR2 |= TIM_CCMR2_OC3M_2;

		TIM4 -> CCMR2 &= ~TIM_CCMR2_OC4M_0;
		TIM4 -> CCMR2 |= TIM_CCMR2_OC4M_1;
		TIM4 -> CCMR2 |= TIM_CCMR2_OC4M_2;

		TIM4 -> CCMR1 |= TIM_CCMR1_OC1PE;
		TIM4 -> CCMR2 |= TIM_CCMR2_OC3PE;
		TIM4 -> CCMR2 |= TIM_CCMR2_OC4PE;
	}
}
/* Public functions -----------------------------------------------------------*/
void port_display_set_rgb (uint32_t display_id, rgb_color_t color){
	if (display_id == PORT_REAR_PARKING_DISPLAY_ID){
		uint8_t r = color.r;
		uint8_t g = color.g;
		uint8_t b = color.b;

		TIM4 -> CR1 &= ~TIM_CR1_CEN;
		TIM4 ->CNT = 0;
		if (r == 0 && g == 0 && b == 0){
			TIM4 -> CCER &= ~TIM_CCER_CC1E;
			TIM4 -> CCER &= ~TIM_CCER_CC3E;
			TIM4 -> CCER &= ~TIM_CCER_CC4E;
			return;
		}

		if (r == 0){
			TIM4 -> CCER &= ~TIM_CCER_CC1E;
		}else{
			TIM4 -> CCR1 = round(r*((TIM4->ARR)+1)/PORT_DISPLAY_RGB_MAX_VALUE);
			TIM4 -> CCER |= TIM_CCER_CC1E;
		}
		if (g == 0){
			TIM4 -> CCER &= ~TIM_CCER_CC3E;
		}else{
			TIM4 -> CCR3 = round(g*((TIM4->ARR)+1)/PORT_DISPLAY_RGB_MAX_VALUE);
			TIM4 -> CCER |= TIM_CCER_CC3E;
		}
		if (b == 0){
			TIM4 -> CCER &= ~TIM_CCER_CC4E;
		}else{
			TIM4 -> CCR4 = round(b*((TIM4->ARR)+1)/PORT_DISPLAY_RGB_MAX_VALUE);
			TIM4 -> CCER |= TIM_CCER_CC4E;
		}

		TIM4 -> EGR |= TIM_EGR_UG;
		TIM4 -> CR1 |= TIM_CR1_CEN;
	}
}

void port_display_init (uint32_t display_id){
	stm32f4_display_hw_t *p_display = _stm32f4_display_get(display_id);
	
	stm32f4_system_gpio_config(
		p_display -> p_port_blue,
		p_display -> pin_blue,
		STM32F4_GPIO_MODE_AF,
		STM32F4_GPIO_PUPDR_NOPULL
	);
	stm32f4_system_gpio_config(
		p_display -> p_port_green,
		p_display -> pin_green,
		STM32F4_GPIO_MODE_AF,
		STM32F4_GPIO_PUPDR_NOPULL
	);
	stm32f4_system_gpio_config(
		p_display -> p_port_red,
		p_display -> pin_red,
		STM32F4_GPIO_MODE_AF,
		STM32F4_GPIO_PUPDR_NOPULL
	);

	stm32f4_system_gpio_config_alternate(
		p_display -> p_port_blue,
		p_display -> pin_blue,
		2
	);
	stm32f4_system_gpio_config_alternate(
		p_display -> p_port_green,
		p_display -> pin_green,
		2
	);
	stm32f4_system_gpio_config_alternate(
		p_display -> p_port_red,
		p_display -> pin_red,
		2
	);

	_timer_pwm_config(display_id);
	port_display_set_rgb(display_id,COLOR_OFF);
}
