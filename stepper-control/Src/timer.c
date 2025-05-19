#include "timer.h"

// setting up motor parameters
static volatile	bool motor1_running = false;
static volatile	bool motor2_running = false;
static volatile	uint8_t motor1_dir = FORWARD;
static volatile	uint8_t motor2_dir = FORWARD;
static volatile bool motor1_step_state = false;
static volatile bool motor2_step_state = false;


void motor1_enable(uint8_t direction) {

	motor1_running = true;
	motor1_dir = direction;

}

void motor2_enable(uint8_t direction) {

	motor2_running = true;
	motor2_dir = direction;

}

void motors_disable(void) {

	motor1_running = false;
	motor2_running = false;
	*((uint8_t*)&(GPIOD->ODR)) &= ~(0b00011110);

}

void enable_timer3_interrupt(void) {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();
    // Configure Timer 3 for periodic interrupt
    TIM3->PSC = 7199;              		// Timer set to 100Hz
    TIM3->ARR = 49;                     // 1s timer
    TIM3->DIER |= TIM_DIER_UIE;         // Enable update interrupt
    TIM3->CR1 |= TIM_CR1_CEN;           // Start the timer
	NVIC_SetPriority(TIM3_IRQn, 2);     // Set Priority as no 2

    NVIC_EnableIRQ(TIM3_IRQn);          // Enable Timer 3 interrupt

	// Re-enable all interrupts
	__enable_irq();
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;

        // Motor1: PD1 = STEP, PD2 = DIR
        if (motor1_running == true) {
            // STEP (PD1) pulse
            if (step_state_motor1) {
                *((uint8_t*)&(GPIOD->ODR)) |= (1 << 1);    // Set PD1
            }
            else {
                *((uint8_t*)&(GPIOD->ODR)) &= ~(1 << 1);   // Clear PD1
            }
            // DIR (PD2)
            if (motor1_dir == FORWARD) {
                *((uint8_t*)&(GPIOD->ODR)) |= (1 << 2);
            }
            else {
                *((uint8_t*)&(GPIOD->ODR)) &= ~(1 << 2);
            }

            step_state_motor1 = !step_state_motor1;  // Toggle for next interrupt
        }

        // Motor2: PD3 = STEP, PD4 = DIR
        if (motor2_running) {
            if (step_state_motor2) {
                *((uint8_t*)&(GPIOD->ODR)) |= (1 << 3);    // Set PD3
            }
            else {
                *((uint8_t*)&(GPIOD->ODR)) &= ~(1 << 3);   // Clear PD3
            }

            if (motor2_dir == FORWARD) {
                *((uint8_t*)&(GPIOD->ODR)) |= (1 << 4);
            }
            else {
                *((uint8_t*)&(GPIOD->ODR)) &= ~(1 << 4);
            }

            step_state_motor2 = !step_state_motor2;
        }

        if ((USART1->ISR & USART_ISR_RXNE) == 0) {
        	motors_disable();
        }
    }
}
