/*
  **********************************************************************************
 * 									digitalio.h										*
 * Contains all functions related to digital IO functionality, including LEDs and	*
 *									GPIO clocks.																		*
 * 																					*
  **********************************************************************************
 */

#ifndef TIMER_H
#define TIMER_h

/*************/
/* INCLUDES */
/*************/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <digitalio.h>
#include "stm32f303xc.h"

/*************/
/* FUCNTIONS */
/*************/

// enable_timer2_interrupt: enables the interrupt for timer 2 as a two second timer
void enable_timer2_interrupt(void);

// enable_timer3_interrupt: enables the interrupt for timer 3 as a one second timer
void enable_timer3_interrupt(void);

// TIM2_IRQHandler: interrupt handler for TIM2; flags that a key press is ready to be read after 2s
void TIM2_IRQHandler(void);

// TIM3_IRQHandler: interrupt handler for TIM3; clears the key pressed state after 1s
void TIM3_IRQHandler(void);

#endif
