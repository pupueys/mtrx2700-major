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

// INCLUDES //

#include <digitalio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "digitalio.h"
#include "stm32f303xc.h"

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

// TYPEDEFS //

// Key struct
typedef struct {
	uint32_t group;
	uint16_t threshold;
	uint8_t value;
	uint8_t touched_flag;
} PianoKey;

// Piano struct
typedef struct {
	PianoKey keys[5];
	uint8_t key_states[5];
	uint8_t sequence[MAX_SEQ_COUNT];
	uint16_t touch_count;

	uint8_t key_pressed;
	uint8_t key_clear;
	uint8_t key_valid;
} Steinway;

// EXTERNS //
extern volatile uint8_t game_complete;
extern Steinway piano;

// FUNCTIONS //

void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef* htsc);
void key_press_logic(uint16_t sample_value, Steinway *piano, uint8_t key, uint16_t threshold);
void gameplay_logic(Steinway* piano, uint8_t key);
void completion_function(void);


#endif
