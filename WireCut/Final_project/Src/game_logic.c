#include "stm32f303xc.h"
#include "digital_io.h"
#include "timer.h"
#include "seven_seg.h"
#include "game_logic.h"

#define CORRECT_WIRE_COUNT 2

static volatile uint8_t wire_pulled[4] = {0, 0, 0, 0};         // Keep track of the wires pulled
static const uint8_t correct_wires[4] = {1, 0, 1, 0};         // hard code wires 1 and 3 to be correct
static volatile uint8_t correct_count = 0;                    // Number of correct wires pulled
static volatile uint8_t game_over = 0;                        // Indicate game over


// Enable interrupts on falling edge for PC1–PC4
void enable_wire_interrupts() {
	__disable_irq();  // Disable global interrupts while configuring

	// 1. Enable SYSCFG clock for EXTI configuration
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// 2. Map EXTI lines 4–7 to Port E using SPL-style macros
	SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI4_PE |
	                    SYSCFG_EXTICR2_EXTI5_PE |
	                    SYSCFG_EXTICR2_EXTI6_PE |
	                    SYSCFG_EXTICR2_EXTI7_PE;

	// 3. Select falling edge trigger to detect wire being pulled (goes from 3.3V to 0V)
	EXTI->FTSR |= EXTI_FTSR_TR4 | EXTI_FTSR_TR5 | EXTI_FTSR_TR6 | EXTI_FTSR_TR7;

	// 4. Unmask (enable) EXTI lines 4–7
	EXTI->IMR |= EXTI_IMR_MR4 | EXTI_IMR_MR5 | EXTI_IMR_MR6 | EXTI_IMR_MR7;

	// 5. Enable NVIC IRQs for EXTI lines
	NVIC_SetPriority(EXTI4_IRQn, 1);       // PE4
	NVIC_EnableIRQ(EXTI4_IRQn);

	NVIC_SetPriority(EXTI9_5_IRQn, 1);     // PE5–PE7
	NVIC_EnableIRQ(EXTI9_5_IRQn);

	__enable_irq();  // Re-enable global interrupts
}

void EXTI4_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR4) {
		EXTI->PR |= EXTI_PR_PR4;       // Clear interrupt flag
		handle_wire_pull(0);           // Wire 1 = PE4
	}
}

void EXTI9_5_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR5) {
		EXTI->PR |= EXTI_PR_PR5;
		handle_wire_pull(1);           // Wire 2 = PE5

	}
	if (EXTI->PR & EXTI_PR_PR6) {
		EXTI->PR |= EXTI_PR_PR6;
		handle_wire_pull(2);           // Wire 3 = PE6

	}
	if (EXTI->PR & EXTI_PR_PR7) {
		EXTI->PR |= EXTI_PR_PR7;
		handle_wire_pull(3);           // Wire 4 = PE7
	}
}


void handle_wire_pull(int wire_index) {

    if (game_over || wire_pulled[wire_index])       // Skip if the wire was already pulled
        return;

    wire_pulled[wire_index] = 1;                    // Record the wire pull for future interrupts

    if (correct_wires[wire_index]) {
        correct_count++;
    }
    else {
        reduce_time(15); // Reduce time by 15 seconds
        update_display(time_left);
    }

    if (correct_count == CORRECT_WIRE_COUNT) {
        end_game(1); // Game over player wins
    }
    else if (time_left == 0) {
        end_game(0); // Game over Player lost
    }

}

void end_game(int end_of_game){
	game_over = 1;
	stop_timer();
	if (end_of_game == 1){
		set_led_state(0b11111111);

	}
	else {
		set_led_state(0b10101010);
	}

}
