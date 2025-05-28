#include "game_logic.h"
#include "serial.h"

// Initialising the piano struct
Steinway piano = {
    .keys = {
        { TSC_GROUP1_IDX,  1700, CKEY, 0 },  // C
        { TSC_GROUP3_IDX, 1700, DKEY, 0 },  // D
        { TSC_GROUP5_IDX,  750, EKEY, 0 },  // E
        { TSC_GROUP6_IDX, 1700, FKEY, 0 },  // F
        { TSC_GROUP8_IDX, 1700, GKEY, 0 },  // G
    },

	// initial state
    .key_states = { 0 },
    .sequence = { 0 },
    .touch_count = 0,
	.key_pressed = 0,
	.key_clear = 0,
	.key_valid = 1		// initially, a key is ready to be read
};


void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef* htsc){

	// discharge the TSC
	HAL_TSC_IODischarge(htsc, ENABLE);

	// check the touch sensing channels
	for (int i = 0; i < 5; i++) {
		if (HAL_TSC_GroupGetStatus(htsc, piano.keys[i].group) == TSC_GROUP_COMPLETED) {
			uint16_t val = (uint16_t)HAL_TSC_GroupGetValue(htsc, piano.keys[i].group);
			key_press_logic(val, &piano, i, piano.keys[i].threshold, &USART1_PORT); // C
		}
	}

	// error if interrupts not initialised
	if (HAL_TSC_Start_IT(htsc) != HAL_OK) {
		Error_Handler();
	}

}


void key_press_logic(uint16_t sample_value, Steinway *piano, uint8_t key, uint16_t threshold, SerialPort *serial_port) {

	if (sample_value < threshold) {				// checks if the value is under the threshold for "pressed"
												// if pressed, sample_value is below the threshold

		if (piano->key_valid) {
			if (!piano->key_pressed) {				// case when key pressed

			  piano->key_states[key] = 1;
			  piano->key_pressed = 1;
			  piano->key_clear = 1;

			  set_leds(key);
			  enable_timer2_interrupt();
			  enable_timer3_interrupt();

			  // check which key was pressed
			  switch(key) {

			  case CKEY:
				  tx_char(0x43, serial_port, piano, key);	// output 'C'
				  break;

			  case DKEY:
				  tx_char(0x44, serial_port, piano, key);	// output 'D'
				  break;

			  case EKEY:
				  tx_char(0x45, serial_port, piano, key);	// output 'E'
				  break;

			  case FKEY:
				  tx_char(0x46, serial_port, piano, key);	// output 'F'
				  break;

			  case GKEY:
				  tx_char(0x47, serial_port, piano, key);	// output 'G'
				  break;
			  }
			}

		 else {
			 piano->key_states[key] = 0;		// key not pressed
		}
	}

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

		// flag the key as pressed
		piano->key_states[key] = 1;

		// check how many keys have been pressed
		for (int i = 0; i < KEY_COUNT; i++) {
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
//				GPIOC->ODR |= (1 << 9);
//				GPIOC->ODR &= ~(1 << 9);
				return;
			}
		}

		// if the song is played correctly, game is complete!
		if (piano->touch_count == MAX_SEQ_COUNT) {
			piano->touch_count = 0;
			completion_function(&piano);
		}

		piano->key_valid = 0;
	}

	return;
}

// when the game is complete, send completed game state to main board
void completion_function(Steinway *piano) {

	GPIOC->ODR |= (1 << 9);
	GPIOC->ODR &= ~(1 << 9);
	reset_piano_state(piano);

}

void reset_piano_state(Steinway *piano) {
    memset(piano->key_states, 0, sizeof(piano->key_states));
    piano->touch_count = 0;
    piano->key_valid = 1;
    piano->key_pressed = 0;
    piano->key_clear = 0;
}
