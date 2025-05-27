#ifndef GANTRY_LOGIC_H
#define GANTRY_LOGIC_H

/*************/
/* INCLUDES */
/*************/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//
typedef struct Motor Motor;

#include "serial.h"
#include "digitalio.h"
#include "motors.h"
#include "stm32f303xc.h"

/***************/
/* DEFINITIONS */
/***************/
#define ROOM1 866
#define ROOM2 1732
#define ROOM3 2598

// check_room_state: checks for the y position of the player
// inputs:
// - motor: The motor that facilitates y motion
void check_room_state(Motor* motor);


#endif
