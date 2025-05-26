/*
  **********************************************************************************
 * 									motors.h										*
 * 																					*
 * 				Contains all functions related to motor function					*
 *																					*
 * 																					*
  **********************************************************************************
 */

#ifndef MOTORS_H
#define MOTORS_H

/*************/
/* INCLUDES */
/*************/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "serial.h"
#include "digitalio.h"
#include "stm32f303xc.h"

/***************/
/* DEFINITIONS */
/***************/
// motor struct
typedef struct {

	bool running;			// flag for when motor is spinning
	uint8_t direction;		// direction of motor rotation
	uint8_t step_index;		// index for step sequence logic
	uint16_t position;		// position of motor

} Motor;


/*************/
/* FUCNTIONS */
/*************/

// motor1_enable: enables the first motor to spin in a specified direction
// inputs:
// - direction: direction of the motor input (FORWARD (1) or BACKWARD (0))
void motor1_enable(uint8_t direction);

// motor2_enable: enables the second motor to spin in a specified direction
// inputs:
// - direction: direction of the motor input (FORWARD (1) or BACKWARD (0))
void motor2_enable(uint8_t direction);

// motor1_disable: disables the first motor
void motor1_disable(void);

// motor2_disable: disables the second motor
void motor2_disable(void);

// set_last_press_time: gets the last time a button was pressed
// inputs:
// - time: time retrieved from millisecond timer
void set_last_press_time(uint32_t time);

// motor_logic: stepper logic for the motors, mainly directional
void motor_logic(void);

// motor_idle: checks whether the last key time was too long ago
// inputs:
// - current_time: the present time given by the millisecond timer
// - current_key: the last key pressed
void motor_idle(uint32_t current_time, uint8_t current_key);

#endif
