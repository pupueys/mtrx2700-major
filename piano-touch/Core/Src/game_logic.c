#include "game_logic.h"

// Initialising the piano struct
Steinway piano = {
    .keys = {
        { TSC_GROUP1_IDX,  450, CKEY, 0 },  // C
        { TSC_GROUP3_IDX, 1700, DKEY, 0 },  // D
        { TSC_GROUP5_IDX,  750, EKEY, 0 },  // E
        { TSC_GROUP6_IDX, 1700, FKEY, 0 },  // F
        { TSC_GROUP8_IDX, 1700, GKEY, 0 },  // G
    },
    .key_states = { 0 },
    .sequence = { 0 },
    .touch_count = 0,
	.key_pressed = 0,
	.key_clear = 0,
	.key_valid = 0
};


void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef* htsc){

	// discharge the TSC
	HAL_TSC_IODischarge(htsc, ENABLE);

	// check the touch sensing channels
	for (int i = 0; i < 5; i++) {
		if (HAL_TSC_GroupGetStatus(htsc, piano.keys[i].group) == TSC_GROUP_COMPLETED) {
			uint16_t val = (uint16_t)HAL_TSC_GroupGetValue(htsc, piano.keys[i].group);
			key_press_logic(val, &piano, i, piano.keys[i].threshold); // C
		}
	}

	// error if interrupts not initialised
	if (HAL_TSC_Start_IT(htsc) != HAL_OK) {
		Error_Handler();
	}

}


void key_press_logic(uint16_t sample_value, Steinway *piano, uint8_t key, uint16_t threshold) {
	if (sample_value < threshold) {

		if (!piano->key_pressed) {

			piano->key_pressed = 1;
			piano->key_clear = 1;
			set_leds(key);
		  enable_timer2_interrupt();
		  enable_timer3_interrupt();
			gameplay_logic(piano, key);			// when key pressed, progress to game logic

		}
	}
	 else {
		 piano->key_states[key] = 0;		// key not pressed
	}

}

void gameplay_logic(Steinway* piano, uint8_t key) {

	uint8_t concurrent_keys = 0;

	// the correct sequence of keys (ode to joy)
	uint8_t correct_sequence[MAX_SEQ_COUNT] = {EKEY, EKEY, FKEY, GKEY, GKEY, FKEY, EKEY, DKEY, CKEY, CKEY,
								DKEY, EKEY, EKEY, DKEY, DKEY, EKEY, EKEY, FKEY, GKEY, GKEY, FKEY, EKEY, DKEY,
								CKEY, CKEY, DKEY, EKEY, DKEY, CKEY, CKEY};

	// for convenience
	uint8_t j = piano->touch_count;

	if (piano->key_valid && piano->key_pressed) {

		piano->key_states[key] = 1;

		// check how many keys have been pressed
		for (int i = 0; i < 5; i++) {
			if (piano->key_states[i] == 1) {
				concurrent_keys++;
			} else {
				continue;
			}
		}

		// if there is more than one key pressed at a time, reset the game
		if (concurrent_keys != 1) {
			memset(piano->key_states, 0, sizeof(piano->key_states));
			piano->touch_count = 0;
			piano->key_valid = 0;
			return;
		}
		// if there is exactly one key pressed
		else if (concurrent_keys == 1) {

			piano->sequence[j] = key;

			// increase count if the pressed key is correct
			if (piano->sequence[j] == correct_sequence[j]){
				piano->touch_count++;

			}
			// if wrong key, reset
			else {
				piano->touch_count = 0;
				piano->key_valid = 0;
				return;
			}
		}

		// if the song is played correctly, game is complete!
		if (piano->touch_count == MAX_SEQ_COUNT) {
			piano->touch_count = 0;
			completion_function();
		}

		piano->key_valid = 0;
	}

	return;
}

void completion_function(void) {
	game_complete = 1;
	return;
}
