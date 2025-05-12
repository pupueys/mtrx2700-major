/*
  **********************************************************************************
 * 								game_logic.h										*
 * Contains all functions related to the function of the piano game submodule		*
 * including the callback function that handles the TSC interrupts.					*
 * 																					*
  **********************************************************************************
 */

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

/*************/
/* INCLUDES */
/*************/
#include <digitalio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "digitalio.h"
#include "stm32f303xc.h"

/***************/
/* DEFINITIONS */
/***************/
#define CKEY 0
#define DKEY 1
#define EKEY 2
#define FKEY 3
#define GKEY 4

#define KEY_COUNT 5
#define MAX_SEQ_COUNT 30

#define TSC_GROUP1_IDX 0
#define TSC_GROUP3_IDX 2
#define TSC_GROUP5_IDX 4
#define TSC_GROUP6_IDX 5
#define TSC_GROUP8_IDX 7


/************/
/* TYPEDEFS */
/************/
// Key struct
typedef struct {
	uint32_t group;								// value to distinguish the touch sensing group
	uint16_t threshold;							// threshold value for when key is pressed
	uint8_t value;								// the set value of the key (C = 0, D = 1, E = 2, F = 3, G = 4)
	uint8_t touched_flag;						// signals if the key was pressed
} PianoKey;

// Piano struct
typedef struct {
	PianoKey keys[KEY_COUNT];					// array of the piano keys
	uint8_t key_states[KEY_COUNT];				/* array of the "pressed" state of the piano keys
												 * of the form [C, D, E, F, G]. If the entry is 1, the key is
												 * pressed. Otherwise, key is not pressed.
												 */

	uint8_t sequence[MAX_SEQ_COUNT];			// sequence of pressed keys
	uint16_t touch_count;						// count of how many successful keys have been pressed in succession

	uint8_t key_pressed;						// flag for when a key is pressed
	uint8_t key_clear;							// flag to clear the key statuses
	uint8_t key_valid;							// flag to say when a key press is ready to be read
} Steinway;

/*************/
/* EXTERNS */
/*************/
extern volatile uint8_t game_complete;
extern Steinway piano;

/*************/
/* FUNCTIONS */
/*************/

// HAL_TSC_ConvCpltCallback:
// Callback function for when touch sensor triggered; calls key_press_logic for each channel when triggered
void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef* htsc);

// key_press_logic: Logic for when a key is pressed; checks if value is below the threshold (if key is pressed)
// inputs:
/* - sample_value = raw digital data from touch sensor
 * - piano = address to piano struct
 * - key = key that was "pressed"/key to check
 * - threshold = known threshold for when the key is pressed
 */
void key_press_logic(uint16_t sample_value, Steinway *piano, uint8_t key, uint16_t threshold);

// gameplay_logic: Logic for the piano gameplay; checks if sequence of keys is correct.
// inputs:
/* - piano: address to piano struct
 * - key: key that was pressed
 */
void gameplay_logic(Steinway* piano, uint8_t key);

// completion_function: Function for when the game is finished
void completion_function(void);


#endif
