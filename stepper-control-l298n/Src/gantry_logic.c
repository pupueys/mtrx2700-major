#include "gantry_logic.h"
#include "motors.h"

void check_room_state(Motor* motor) {

	// checks if the motor is within room 1
	if (motor->position < ROOM1) {

		motor->lower_limit = 0;
		motor->upper_limit = ROOM1;

		// signals to enable game 1
		GPIOC->ODR |= (1 << 1);
		GPIOC->ODR &= ~(1 << 2);
		GPIOC->ODR &= ~(1 << 3);

	}

	// checks if the motor is within room 2
	else if (motor->position < ROOM2 && motor->position >= ROOM1) {

		motor->lower_limit = ROOM1;
		motor->upper_limit = ROOM2;

		// signals to enable game 2
		GPIOA->ODR |= (1 << 2);
		GPIOA->ODR &= ~(1 << 1);
		GPIOA->ODR &= ~(1 << 3);

	}

	else if (motor->position < ROOM3 && motor->position >= ROOM2) {

		motor->lower_limit = ROOM2;
		motor->upper_limit = ROOM3;

		// signals to enable game 3
		GPIOA->ODR |= (1 << 3);
		GPIOA->ODR &= ~(1 << 1);
		GPIOA->ODR &= ~(1 << 2);

	}


}
