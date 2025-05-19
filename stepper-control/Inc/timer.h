/*
  **********************************************************************************
 * 									timer.h											*
 * 																					*
 * 				Contains all functions related to timer functionality				*
 *																					*
 * 																					*
  **********************************************************************************
 */

#ifndef TIMER_H
#define TIMER_H

/*************/
/* INCLUDES */
/*************/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "serial.h"
#include "digital_io.h"
#include "stm32f303xc.h"

/*************/
/* FUCNTIONS */
/*************/

void motor1_enable(uint8_t direction);

void motor2_enable(uint8_t direction);

void motors_disable(void);

// enable_timer3_interrupt: enables the interrupt for timer 3 as a one second timer
void enable_timer3_interrupt(void);

// TIM3_IRQHandler: interrupt handler for TIM3
void TIM3_IRQHandler(void);

#endif
