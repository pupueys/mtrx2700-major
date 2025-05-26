/*
  **********************************************************************************
 * 									digitalio.h										*
 * 																					*
 * 				Contains all functions related to digital function					*
 *																					*
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
#include "stm32f303xc.h"

/*************/
/* FUNCTIONS */
/*************/

// enable_clocks: enables all necessary clocks for GPIO and timers;
void enable_clocks(void);

// initialise_board: initialises GPIO for output
void initialise_board(void);

#endif
