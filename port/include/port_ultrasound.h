/**
 * @file port_ultrasound.h
 * @brief Header for the portable functions to interact with the HW of the ultrasound sensors. The functions must be implemented in the platform-specific code.
 * @author Eneko Emilio Sendín Gallastegi
 * @author Rodrigo Gutierrez Fontán
 * @date 2025-03-18
 */
#ifndef PORT_ULTRASOUND_H_
#define PORT_ULTRASOUND_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
/* Standard C includes */

/* Defines and enums ----------------------------------------------------------*/
#define 	PORT_REAR_PARKING_SENSOR_ID   0 /*!< ID del primer objeto ultrasound*/
 
#define 	PORT_PARKING_SENSOR_TIMEOUT_MS 100 /*!< Tiempo del timeout hasta recibir señal echo */
 
#define 	SPEED_OF_SOUND_MS   343 /*!< Velocidad de sonido*/

#define		PORT_PARKING_SENSOR_TRIGGER_UP_US 10 /*!< Valor cada cuanto conmuta el trigger*/
/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Configura las especificaciones de hardware de un sensor de ultrasonido determinado. Inicializando tanto los timers como los valores del objeto ultrasound.
 * @param ultrasound_id ID del objeto ultrasound.
 */
void 	port_ultrasound_init (uint32_t ultrasound_id);

/**
 * @brief Iniciar una nueva medición con el sensor de ultrasonidos.
 * @param ultrasound_id ID del objeto ultrasound.
 */
void 	port_ultrasound_start_measurement (uint32_t ultrasound_id); 

/**
 * @brief Detiene el temporizador que controla la señal de disparo.
 * @param ultrasound_id ID del objeto ultrasound.
 */
void 	port_ultrasound_stop_trigger_timer (uint32_t ultrasound_id);
 
/**
 * @brief Detiene el temporizador que controla la señal de eco.
 * @param ultrasound_id ID del objeto ultrasound.
 */
void 	port_ultrasound_stop_echo_timer (uint32_t ultrasound_id);

/**
 * @brief Inicia el temporizador que controla la nueva medición.
 * @param ultrasound_id ID del objeto ultrasound.
 */
void 	port_ultrasound_start_new_measurement_timer (void);

/**
 * @brief Detiene el temporizador que controla la nueva medición.
 * @param ultrasound_id ID del objeto ultrasound.
 */
void 	port_ultrasound_stop_new_measurement_timer (void);

/**
 * @brief Restablecer los echo_ticks de la señal de eco.
 * @param ultrasound_id ID del objeto ultrasound.
 */
void 	port_ultrasound_reset_echo_ticks (uint32_t ultrasound_id);

/**
 * @brief Detenga todos los temporizadores del sensor de ultrasonido y restablezca los ticks del eco.
 * @param ultrasound_id ID del objeto ultrasound.
 */
void 	port_ultrasound_stop_ultrasound (uint32_t ultrasound_id);

/**
 * @brief Obtener la preparación de la señal de disparo.
 * @param ultrasound_id ID del objeto ultrasound.
 * @returns booleano con el estado de trigger ready
 */
bool 	port_ultrasound_get_trigger_ready (uint32_t ultrasound_id);

/**
 * @brief Establezca la  señal de trigger ready.
 * @param ultrasound_id ID del objeto ultrasound.
 * @param trigger_ready nuevo boolean a configurar.
 */
void 	port_ultrasound_set_trigger_ready (uint32_t ultrasound_id, bool trigger_ready);

/**
 * @brief Obtener la señal de final de disparo.
 * @param ultrasound_id ID del objeto ultrasound.
 * @returns booleano con el estado de trigger end
 */
bool 	port_ultrasound_get_trigger_end (uint32_t ultrasound_id);

/**
 * @brief Modificar la señal de final de disparo.
 * @param ultrasound_id ID del objeto ultrasound.
 * @param trigger_end nuevo boolean a configurar.
 */
void 	port_ultrasound_set_trigger_end (uint32_t ultrasound_id, bool trigger_end);

/**
 * @brief Obtener la variable init tick del objeto ultrasound.
 * @param ultrasound_id ID del objeto ultrasound.
 * @returns entero de 32 bits con el valor de init tick.
 */
uint32_t 	port_ultrasound_get_echo_init_tick (uint32_t ultrasound_id);

/**
 * @brief Modificar la variable init tick del objeto ultrasound.
 * @param ultrasound_id ID del objeto ultrasound.
 * @param echo_init_tick entero de 32 bits con el nuevo valor de init tick.
 */
void 	port_ultrasound_set_echo_init_tick (uint32_t ultrasound_id, uint32_t echo_init_tick);

/**
 * @brief Obtener la variable end tick del objeto ultrasound.
 * @param ultrasound_id ID del objeto ultrasound.
 * @returns entero de 32 bits con el valor de end tick.
 */
uint32_t 	port_ultrasound_get_echo_end_tick (uint32_t ultrasound_id);

/**
 * @brief Modificar la variable end tick del objeto ultrasound.
 * @param ultrasound_id ID del objeto ultrasound.
 * @param echo_end_tick entero de 32 bits con el nuevo valor de end tick.
 */
void 	port_ultrasound_set_echo_end_tick (uint32_t ultrasound_id, uint32_t echo_end_tick);
 	//Set the time tick when the end of echo signal was received. 

/**
 * @brief Obtener la variable echo received del objeto ultrasound.
 * @param ultrasound_id ID del objeto ultrasound.
 * @returns boolean con el valor de echo received.
 */
bool 	port_ultrasound_get_echo_received (uint32_t ultrasound_id);

/**
 * @brief Modificar la variable echo received del objeto ultrasound.
 * @param ultrasound_id ID del objeto ultrasound.
 * @param echo_received boolean con el nuevo valor de echo received.
 */
void 	port_ultrasound_set_echo_received (uint32_t ultrasound_id, bool echo_received);

/**
 * @brief Obtener la variable overflows tick del objeto ultrasound.
 * @param ultrasound_id ID del objeto ultrasound.
 * @returns entero de 32 bits con el valor de overflows.
 */
uint32_t 	port_ultrasound_get_echo_overflows (uint32_t ultrasound_id);

/**
 * @brief Modificar la variable overflows tick del objeto ultrasound.
 * @param ultrasound_id ID del objeto ultrasound.
 * @param entero de 32 bits con el nuevo valor de overflows.
 */
void 	port_ultrasound_set_echo_overflows (uint32_t ultrasound_id, uint32_t echo_overflows);

#endif /* PORT_ULTRASOUND_H_ */
