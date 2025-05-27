#include "motors.h"

// initialising motors
Motor Motor1 = {
		false,		// running
		FORWARD,	// direction
		0,			// step index
		0,			// y pos (motor 1 is y axis motion)
		2600,
		0
};

Motor Motor2 = {
		false,		// running
		FORWARD,	// direction
		0,			// step index
		0,			// x pos (motor 2 is x axis motion)
		1450,
		0
};

static volatile uint32_t last_press_time = 0;

static const uint16_t y_limits[3] = {
		866,
		1732,
		2598
};

// full step sequence for stepper motors
const uint8_t full_step_sequence[4][4] = {
    {1, 0, 1, 0}, // Step 1
    {0, 1, 1, 0}, // Step 2
    {0, 1, 0, 1}, // Step 3
    {1, 0, 0, 1}  // Step 4
};

void motor1_enable(uint8_t direction) {

	Motor1.running = true;
	Motor1.direction = direction;

}

void motor2_enable(uint8_t direction) {

	Motor2.running = true;
	Motor2.direction = direction;

}

void motor1_disable(void) {

	Motor1.running = false;
	*((uint8_t*)&(GPIOD->ODR)) &= ~(0b00011110);  // reset the bits

}

void motor2_disable(void) {

	Motor2.running = false;
	*((uint8_t*)&(GPIOD->ODR)) &= ~(0b11110000);  // reset the bits
}

void set_last_press_time(uint32_t time) {
	last_press_time = time;
}

void motor_logic(void) {
    if (Motor1.running) {
        // update step index
//    	if (Motor1.direction == FORWARD) {	<---------	UNCOMMENT FOR NO LIMITS
        if ((Motor1.direction == FORWARD) && (Motor1.position < Motor1.upper_limit)) {
            Motor1.step_index = (Motor1.step_index + 1) % 4;	// step forward
            Motor1.position++;
        } else if ((Motor1.direction == BACKWARD) && (Motor1.position > Motor1.lower_limit)) {
            Motor1.step_index = (Motor1.step_index + 3) % 4;	// step backward
            Motor1.position--;
        }

        // logic for stepping
        uint8_t *s = full_step_sequence[Motor1.step_index];
        GPIOD->ODR = (GPIOD->ODR & ~(0xF)) |			// write to PD0–PD3 for output to stepper motor
                     (s[0] << 0) | (s[1] << 1) |
                     (s[2] << 2) | (s[3] << 3);

        // debug leds
        GPIOE->ODR = (GPIOE->ODR & ~((1 << 8) | (1 << 9) | (1 << 10) | (1 << 11))) |
                     (s[0] << 8) | (s[1] << 9) |
                     (s[2] << 10) | (s[3] << 11);

        check_room_state(&Motor1);

    }

    if (Motor2.running) {
    	// update step index
//    	if (Motor2.direction == FORWARD) {	<-------- UNCOMMENT FOR NO LIMITS
        if ((Motor2.direction == FORWARD) && (Motor2.position < Motor2.upper_limit)) {
            Motor2.step_index = (Motor2.step_index + 1) % 4;
            Motor2.position++;
        } else if ((Motor2.direction == BACKWARD) && (Motor2.position > Motor2.lower_limit)){
            Motor2.step_index = (Motor2.step_index + 3) % 4;
            Motor2.position--;
        }

        // Write IN1–IN4 to PD4–PD7
        uint8_t *s2 = full_step_sequence[Motor2.step_index];
        GPIOD->ODR = (GPIOD->ODR & ~(0xF0)) |
                     (s2[0] << 4) | (s2[1] << 5) |		// write to PD4-PD7 for output to stepper motor
                     (s2[2] << 6) | (s2[3] << 7);

        // debug leds
        GPIOE->ODR = (GPIOE->ODR & ~(0xF << 12)) |
                     (s2[0] << 12) | (s2[1] << 13) |
                     (s2[2] << 14) | (s2[3] << 15);

        check_room_state(&Motor2);

    }
}

void motor_idle(uint32_t current_time, uint8_t current_key) {

    if (Motor1.running) {

    	// idling the motor if the key press occurred 50ms ago
    	// OR
    	// if the last key press was not valid
    	if ((current_time - last_press_time > 50) ||
    		(current_key != 'w' && current_key != 's'
    		&& current_key != 'e' && current_key != 'q' && current_key != 'z' && current_key != 'c')) {
    		Motor1.step_index = 4;
    		motor1_disable();
    	}
    }

    if (Motor2.running) {

    	// idling the motor if the key press occurred 50ms ago
    	// OR
    	// if the last key press was not valid
    	if ((current_time - last_press_time > 50) ||
    		(current_key != 'a' && current_key != 'd'
			&& current_key != 'e' && current_key != 'q' && current_key != 'z' && current_key != 'c')) {
    		Motor2.step_index = 4;
    		motor2_disable();
    	}
    }

}
