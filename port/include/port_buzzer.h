/**
 * @file port_buzzer.h
 * @brief Header for the portable functions to interact with the HW of the BUZZER system. The functions must be implemented in the platform-specific code.
 * @author Eneko Emilio Sendín Gallastegi
 * @author Rodrigo Gutierrez Fontán
 * @date fecha
 */
#ifndef PORT_BUZZER_SYSTEM_H_
#define PORT_BUZZER_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Standard C includes */
/* Defines and enums ----------------------------------------------------------*/
/* Enums */

/* Defines ----------------------------------------------------------*/
#define PORT_PARKING_BUZZER_ID   0

#define BUZZER_OFF (buzzer_t){0}
//#define COLOR_BASIC (buzzer_t){400}

// NOTAS en Hz
#define DO 261
#define RE 293
#define MI 329
#define FA 349
#define SOL 392
#define LA 440
#define SI 494
#define DO_ALTO 523

/* Typedefs --------------------------------------------------------------------*/
typedef struct
{
	uint32_t freq; //frecuencia de la nota en Hz
} buzzer_t;

/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Configura las especificaciones de hardware de un pulsador determinado. Inicializando tanto los timers como los valores del objeto buzzer.
 * @param buzzer_id ID del objeto buzzer.
 */
void port_buzzer_init (uint32_t buzzer_id);
 
//Configure the HW specifications of a given buzzer.

/**
 * @brief Función que modifica frecuencia de pulsador
 * @param buzzer_id ID del objeto buzzer.
 * @param buzzer objeto buzzer con una frecuencia y un tiempo de pulso.
 */
void port_buzzer_set_freq (uint32_t buzzer_id, buzzer_t buzzer);

/**
 * @brief Función que incrementa el contador de la estructura buzzer.
 * @param buzzer_id ID del objeto buzzer.
 */
void port_buzzer_counter_add(uint32_t buzzer_id);

/**
 * @brief Función que reinicia el contador de la estructura buzzer.
 * @param buzzer_id ID del objeto buzzer.
 */
void port_buzzer_counter_reset(uint32_t buzzer_id);

/**
 * @brief Función que devuelve el valor del contador de la estructura buzzer.
 * @param buzzer_id ID del objeto buzzer.
 * @return entero con el valor actual del contador del pulsador.
 */
uint32_t get_port_buzzer_counter(uint32_t buzzer_id);
#endif /* PORT_BUZZER_SYSTEM_H_ */