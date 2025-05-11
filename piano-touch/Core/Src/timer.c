#include "game_logic.h"
#include "timer.h"

void enable_timer2_interrupt(void) {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();

	// Configure Timer 3 for periodic interrupt
	TIM2->PSC = 7999;              	// Timer set to 100Hz (8MHz)
	TIM2->ARR = 1999;
	TIM2->CNT = 0;         // Reset counter
	TIM2->SR = 0;          // Clear status register
	TIM2->DIER |= TIM_DIER_UIE;         // Enable update interrupt
	TIM2->CR1 |= TIM_CR1_CEN;           // Start the timer

	NVIC_SetPriority(TIM2_IRQn, 2);     // Set Priority as no 2
    NVIC_EnableIRQ(TIM2_IRQn);          // Enable Timer 3 interrupt

	// Re-enable all interrupts
	__enable_irq();
}

void enable_timer3_interrupt(void) {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();
    // Configure Timer 3 for periodic interrupt
    TIM3->PSC = 7999;              // Timer increments every 1 ms
    TIM3->ARR = 999;                      // The reload by default is set at 1 so that is not noticible
    TIM3->DIER |= TIM_DIER_UIE;         // Enable update interrupt
    TIM3->CR1 |= TIM_CR1_CEN;           // Start the timer
	NVIC_SetPriority(TIM3_IRQn, 2);     // Set Priority as no 2

    NVIC_EnableIRQ(TIM3_IRQn);          // Enable Timer 3 interrupt

	// Re-enable all interrupts
	__enable_irq();
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;

        piano.key_valid = 1;

    }
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;

        piano.key_pressed = 0;

        if (piano.key_clear) {

        	for (int i=0; i < 5; i++) {
        		clear_leds(i);
        	}

			memset(piano.key_states, 0, sizeof(piano.key_states));
        	piano.key_clear = 0;
        }
    }
}
