#include "seven_seg.h"
#include "stm32f303xc.h"
#include "game_logic.h"
#include "timer.h"

extern volatile int time_left;

void enable_7seg_interrupts() {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();
    // Configure Timer 3 for periodic interrupt
    TIM3->PSC = 8000 - 1;              // Timer increments every 1 ms
    TIM3->ARR = 1000    ;               // uncomment this to set the reload time to 1 sec
    TIM3->DIER |= TIM_DIER_UIE;         // Enable update interrupt
    TIM3->CR1 |= TIM_CR1_CEN;           // Start the timer
	NVIC_SetPriority(TIM3_IRQn, 2);     // Set Priority as no 1

    NVIC_EnableIRQ(TIM3_IRQn);          // Enable Timer 3 interrupt

	// Re-enable all interrupts
	__enable_irq();
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;
        if (time_left > 0) {
                time_left--;
                update_display(time_left);
            }
        else if(time_left == 0){
        	end_game(0);
        }
        else if(time_left < 0){
        	update_display(0);
        }
    }
}


void stop_timer(void) {
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

void reduce_time(uint8_t seconds) {

    if (time_left > seconds) {
        time_left -= seconds;
    } else {
        time_left = 0;
    }

}
