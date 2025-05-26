#include "timer.h"

// setting up motor parameters
static volatile	bool motor1_running = false;
static volatile	uint8_t motor1_dir = FORWARD;
static volatile uint8_t motor1_step_index = 0;

static volatile	bool motor2_running = false;
static volatile	uint8_t motor2_dir = FORWARD;
static volatile uint8_t motor2_step_index = 0;

static volatile uint32_t last_press_time = 0;
static volatile uint32_t ms_timer = 0;

const uint8_t full_step_sequence[4][4] = {
    {1, 0, 1, 0}, // Step 1
    {0, 1, 1, 0}, // Step 2
    {0, 1, 0, 1}, // Step 3
    {1, 0, 0, 1}  // Step 4
};

void motor1_enable(uint8_t direction) {

	motor1_running = true;
	motor1_dir = direction;

}

void motor2_enable(uint8_t direction) {

	motor2_running = true;
	motor2_dir = direction;

}

void motor1_disable(void) {

	motor1_running = false;
	*((uint8_t*)&(GPIOD->ODR)) &= ~(0b00011110);

}

void motor2_disable(void) {

	motor2_running = false;
	*((uint8_t*)&(GPIOD->ODR)) &= ~(0b11110000);
}

void set_last_press_time(uint32_t time) {
	last_press_time = time;
}

uint32_t get_time(void){
	return ms_timer;
}


void enable_timer3_interrupt(void) {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();
     //Configure Timer 3 for periodic interrupt
    TIM3->PSC = 333;              		// Timer set to 100Hz
    TIM3->ARR = 20;                     // 1s timer

//	TIM3->PSC = 499;
//	TIM3->ARR = 99;

    TIM3->DIER |= TIM_DIER_UIE;         // Enable update interrupt
    TIM3->CR1 |= TIM_CR1_CEN;           // Start the timer
	NVIC_SetPriority(TIM3_IRQn, 2);     // Set Priority as no 2

    NVIC_EnableIRQ(TIM3_IRQn);          // Enable Timer 3 interrupt

	// Re-enable all interrupts
	__enable_irq();
}

void enable_timer2_interrupt(void) {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();
    // Configure Timer 2 for periodic interrupt
    TIM2->PSC = 7;              		// Timer set to 1MHz
    TIM2->ARR = 999;                     // 1ms timer
    TIM2->DIER |= TIM_DIER_UIE;         // Enable update interrupt
    TIM2->CR1 |= TIM_CR1_CEN;           // Start the timer
	NVIC_SetPriority(TIM2_IRQn, 3);     // Set Priority as no 3

    NVIC_EnableIRQ(TIM2_IRQn);          // Enable Timer 2 interrupt

	// Re-enable all interrupts
	__enable_irq();
}

void TIM3_IRQHandler(void) {

    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;

	if (motor1_running) {
		// Update index
		if (motor1_dir == FORWARD) {
			motor1_step_index = (motor1_step_index + 1) % 4;
		} else {
			motor1_step_index = (motor1_step_index + 3) % 4;

		}

		// Write IN1–IN4 to PD0–PD3
		uint8_t *s = full_step_sequence[motor1_step_index];
		GPIOD->ODR = (GPIOD->ODR & ~(0xF)) |
					 (s[0] << 0) | (s[1] << 1) |
					 (s[2] << 2) | (s[3] << 3);

		GPIOE->ODR = (GPIOE->ODR & ~((1 << 8) | (1 << 9) | (1 << 10) | (1 << 11))) |
					 (s[0] << 8) | (s[1] << 9) |
					 (s[2] << 10) | (s[3] << 11);
	}

	if (motor2_running) {
		if (motor2_dir == FORWARD) {
			motor2_step_index = (motor2_step_index + 1) % 4;
		} else {
			motor2_step_index = (motor2_step_index + 3) % 4;
		}

		// Write IN1–IN4 to PD4–PD7
		uint8_t *s = full_step_sequence[motor2_step_index];
		GPIOD->ODR = (GPIOD->ODR & ~(0xF0)) |
					 (s[0] << 4) | (s[1] << 5) |
					 (s[2] << 6) | (s[3] << 7);
		GPIOE->ODR = (GPIOE->ODR & ~(0xF << 12)) |
					 (s[0] << 12) | (s[1] << 13) |
					 (s[2] << 14) | (s[3] << 15);
	}

//        motor_logic();

    }
}

void TIM2_IRQHandler(void) {

    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        ms_timer++;

    	uint8_t current_key = get_last_key();

        if (motor1_running) {

        	if ((ms_timer - last_press_time > 50) ||
        		(current_key != 'w' && current_key != 's'
        		&& current_key != 'e' && current_key != 'q' && current_key != 'z' && current_key != 'c')) {
        		motor1_step_index = 4;
        		motor1_disable();
        	}
        }

        if (motor2_running) {
        	if ((ms_timer - last_press_time > 50) ||
        		(current_key != 'a' && current_key != 'd'
    			&& current_key != 'e' && current_key != 'q' && current_key != 'z' && current_key != 'c')) {
        		motor2_step_index = 4;
        		motor2_disable();
        	}
        }

//        motor_idle(ms_timer);

    }
}
