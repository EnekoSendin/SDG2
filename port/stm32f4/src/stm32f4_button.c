/**
 * @file stm32f4_button.c
 * @brief Portable functions to interact with the button FSM library. All portable functions must be implemented in this file.
 * @author Eneko Emilio Sendín Gallastegi
 * @author Rodrigo Gutierrez Fontán
 * @date 2025-03-18
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */

/* HW dependent includes */
#include "port_button.h" // Used to get general information about the buttons (ID, etc.)
#include "port_system.h" // Used to get the system tick
#include <stdio.h>
#include "stm32f4_system.h"
#include "stm32f4_button.h"

/* Microcontroller dependent includes */
// TO-DO alumnos: include the necessary files to interact with the GPIOs


/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief La estructura del boton tienen los siguientes elementos: el puerto y pin del boton, el modo de pull-up/pull-down y un booleano para saber si el boton está presionado
 *
 */
typedef struct
{
    GPIO_TypeDef *p_port;
    uint8_t pin;
    uint8_t pupd_mode;
    bool flag_pressed;
} stm32f4_button_hw_t;

/* Global variables ------------------------------------------------------------*/
static stm32f4_button_hw_t 	buttons_arr [] = {
	[PORT_PARKING_BUTTON_ID] = {.p_port = STM32F4_PARKING_BUTTON_GPIO,.pin = STM32F4_PARKING_BUTTON_PIN, .pupd_mode = STM32F4_GPIO_PUPDR_NOPULL},
};

/* Private functions ----------------------------------------------------------*/
/**
 * @brief Get the button status struct with the given ID.
 *
 * @param button_id Button ID.
 *
 * @return Pointer to the button state struct.
 * @return NULL If the button ID is not valid.
 */
stm32f4_button_hw_t *_stm32f4_button_get(uint32_t button_id)
{
    // Return the pointer to the button with the given ID. If the ID is not valid, return NULL.
    if (button_id < sizeof(buttons_arr) / sizeof(buttons_arr[0]))
    {
        return &buttons_arr[button_id];
    }
    else
    {
        return NULL;
    }
}

/* Public functions -----------------------------------------------------------*/
void port_button_init(uint32_t button_id)
{
    // Retrieve the button struct using the private function and the button ID
    stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
	
    /* TO-DO alumnos */
	stm32f4_system_gpio_config(
		p_button->p_port,
		p_button->pin,
		STM32F4_GPIO_MODE_IN,
		p_button->pupd_mode
	);
	stm32f4_system_gpio_config_exti(
		p_button->p_port,
		p_button->pin,
		(STM32F4_TRIGGER_BOTH_EDGE | STM32F4_TRIGGER_ENABLE_INTERR_REQ)
	);
	stm32f4_system_gpio_exti_enable(
		p_button->pin,
		1,
		0 
	);
}

void stm32f4_button_set_new_gpio(uint32_t button_id, GPIO_TypeDef *p_port, uint8_t pin)
{
    stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
    p_button->p_port = p_port;
    p_button->pin = pin;
}
 
bool 	port_button_get_pressed (uint32_t button_id){
	stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
	return p_button->flag_pressed;
}
 
bool 	port_button_get_value (uint32_t button_id){
	stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
	return stm32f4_system_gpio_read(p_button->p_port,p_button->pin);
}
 
void 	port_button_set_pressed (uint32_t button_id, bool pressed){
	stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
	p_button->flag_pressed = pressed;
}
 
bool 	port_button_get_pending_interrupt (uint32_t button_id){
	stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
	return (EXTI->PR && BIT_POS_TO_MASK(p_button->pin) != 0);
}
 
void 	port_button_clear_pending_interrupt (uint32_t button_id){
	stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
	EXTI->PR |= BIT_POS_TO_MASK(p_button->pin);
}
 
void 	port_button_disable_interrupts (uint32_t button_id){
	stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
	stm32f4_system_gpio_exti_disable(p_button->pin);
}
