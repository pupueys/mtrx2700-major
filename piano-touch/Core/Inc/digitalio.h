/*
  **********************************************************************************
 * 									digitalio.h										*
 * Contains all functions related to digital IO functionality, including LEDs and	*
 *									GPIO clocks.																		*
 * 																					*
  **********************************************************************************
 */

#ifndef DIGITALIO_H
#define DIGITALIO_H

/*************/
/* INCLUDES */
/*************/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "timer.h"
#include "stm32f303xc.h"

/*************/
/* FUNCTIONS */
/*************/

// enable_clock: Enables GPIO and timer clocks
void enable_clocks(void);

// initialise_leds: Initialises onboard LEDs
void initialise_gpio(void);

// set_leds: Sets the LEDs when given a certain key
// Input: key that has been pressed
void set_leds(uint8_t key);

// clear_leds:Clears the LED for a certain key
// Input: key that has been "let go" (no longer pressed(
void clear_leds(uint8_t key);

// game_wait: waits for input signal to start the game
void game_wait();

void EXTI0_IRQHandler(void);

#endif
