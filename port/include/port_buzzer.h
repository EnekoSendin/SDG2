/**
 * @file port_display.h
 * @brief Header for the portable functions to interact with the HW of the BUZZER system. The functions must be implemented in the platform-specific code.
 * @author alumno1
 * @author alumno2
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
#define PORT_REAR_PARKING_BUZZER_ID   0
#define PORT_BUZZER_MAX_VALUE 255
#define BUZZER_OFF (buzzer_t){0, 0}
#define COLOR_BASIC (buzzer_t){400,5000}

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
	uint32_t freq;//hz
	uint32_t time;//ms
} buzzer_t;

/* Function prototypes and explanation -------------------------------------------------*/
void port_buzzer_init (uint32_t buzzer_id);
 
//Configure the HW specifications of a given buzzer. More...
 
void port_buzzer_set_freq (uint32_t buzzer_id, buzzer_t buzzer);

#endif /* PORT_BUZZER_SYSTEM_H_ */