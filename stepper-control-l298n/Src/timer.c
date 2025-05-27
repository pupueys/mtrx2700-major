#include "timer.h"

static volatile uint32_t ms_timer = 0;

uint32_t get_time(void){
	return ms_timer;
}

void enable_timer3_interrupt(void) {

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;           // enable clock for timer 3

	__disable_irq();
    TIM3->PSC = 333;              		// Timer set to 100Hz
    TIM3->ARR = 20;                     // 1s timer

    TIM3->DIER |= TIM_DIER_UIE;         // Enable update interrupt
    TIM3->CR1 |= TIM_CR1_CEN;           // Start the timer
	NVIC_SetPriority(TIM3_IRQn, 2);     // Set Priority as no 2

    NVIC_EnableIRQ(TIM3_IRQn);          // Enable Timer 3 interrupt

	// Re-enable all interrupts
	__enable_irq();
}

void enable_timer2_interrupt(void) {

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;           // enable clock for timer 2

	__disable_irq();
    TIM2->PSC = 7;              		// Timer set to 1MHz
    TIM2->ARR = 999;                    // 1ms timer
    TIM2->DIER |= TIM_DIER_UIE;         // Enable update interrupt
    TIM2->CR1 |= TIM_CR1_CEN;           // Start the timer
	NVIC_SetPriority(TIM2_IRQn, 3);     // Set Priority as no 3

    NVIC_EnableIRQ(TIM2_IRQn);          // Enable Timer 2 interrupt

	// Re-enable all interrupts
	__enable_irq();
}

void TIM3_IRQHandler(void) {

    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;	// clear flags

        motor_logic();				// run stepper motor logic

    }
}

void TIM2_IRQHandler(void) {

    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;	// clear flags
        ms_timer++;

    	uint8_t current_key = get_last_key();	// retrieve last key pressed
        motor_idle(ms_timer, current_key);		// check if motors should be idle

    }
}
