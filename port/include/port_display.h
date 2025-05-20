/**
 * @file port_display.h
 * @brief Header for the portable functions to interact with the HW of the display system. The functions must be implemented in the platform-specific code.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */
#ifndef PORT_DISPLAY_SYSTEM_H_
#define PORT_DISPLAY_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Standard C includes */
/* Defines and enums ----------------------------------------------------------*/
/* Enums */

/* Defines ----------------------------------------------------------*/
/** 
 * @brief ID del boton
*/
#define PORT_REAR_PARKING_DISPLAY_ID   0

/** 
 * @brief Valor máximo del valor rgb
*/
#define PORT_DISPLAY_RGB_MAX_VALUE 255

/** 
 * @brief ciclo de trabajo sobre 255 de cada color
*/
#define COLOR_RED (rgb_color_t){255, 0, 0}
#define COLOR_GREEN (rgb_color_t){0, 255, 0}
#define COLOR_BLUE (rgb_color_t){0, 0, 255}
#define COLOR_YELLOW (rgb_color_t){237, 237, 0}
#define COLOR_TURQUOISE (rgb_color_t){26, 89, 82}
#define COLOR_OFF   (rgb_color_t){0, 0, 0}

/* Typedefs --------------------------------------------------------------------*/
/** 
 * @brief Estructura de los colores con un valor de 8 bits para definir el ciclo de trabajo de cada color
*/
typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb_color_t;

/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Configura las especificaciones de hardware de un display determinado. Inicializando tanto el timer como los valores del objeto display.
 * @param display_id ID del objeto display.
 */
void port_display_init (uint32_t display_id);
 
//Configure the HW specifications of a given display. More...
/**
 * @brief Configura las el ciclo de trabajo de los timers de un display determinado. Cambiando el duty ciclo para cada color.
 * @param display_id ID del objeto display.
 * @param color objeto que indica el duty ciclo por color a configurar.
 */
void port_display_set_rgb (uint32_t display_id, rgb_color_t color);

#endif /* PORT_DISPLAY_SYSTEM_H_ */