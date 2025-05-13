#include "seven_seg.h"
#include "stm32f303xc.h"
#include "game_logic.h"
#include "timer.h"
#include "digital_io.h"


static int tick = 0;

void enable_7seg_interrupts() {
    // Disable the interrupts 
    __disable_irq();
    // Configure Timer 3 for periodic interrupt
    TIM3->PSC = 8000 - 1;              // Timer increments every 1 ms
    TIM3->ARR = 5    ;                 // 5 ms timer
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

        update_display(time_left); // Multiplexing

        tick++;
        if (tick >= 200) {  // 200 * 5ms = 1000ms for a 1 second decrement
            tick = 0;
            if (time_left > 0) {
                time_left--;
            } else {
                end_game(0);
            }
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
