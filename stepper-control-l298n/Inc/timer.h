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
#include "digitalio.h"
#include "stm32f303xc.h"

/*************/
/* FUCNTIONS */
/*************/

uint32_t get_time(void);

// enable_timer3_interrupt: enables the interrupt for timer 3 as a one second timer
void enable_timer3_interrupt(void);

void enable_timer2_interrupt(void);

// TIM3_IRQHandler: interrupt handler for TIM3
void TIM3_IRQHandler(void);

void TIM2_IRQHandler(void);


#endif
