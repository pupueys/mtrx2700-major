#include "stm32f303xc.h"
#include "digital_io.h"
#include "timer.h"
#include "seven_seg.h"
#include "game_logic.h"
volatile int time_left = 10;

// LED state set to static so that it is only accessible in this file
static volatile uint8_t led_state;

void enable_clocks() {
	// Enable Clocks A, C and D for input and outputs
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIODEN | RCC_AHBENR_GPIOEEN;

    // Enable Clock for timer 3
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
}

void initialise_board(){
	// Initialise 7 seg output pins
	uint16_t *seg_registers = ((uint16_t *)&(GPIOD->MODER));   // PD 0-6 are set to output
	*seg_registers = 0x5555;


	// Set pin PE 4,5,6 and 7 in pull down configuration to read 0 when pulled out
	GPIOE->PUPDR &=
	    ~((0b11 << (4 * 2)) | (0b11 << (5 * 2)) |      // clear old settings for PE4, PE5
	      (0b11 << (6 * 2)) | (0b11 << (7 * 2)));      // and for PE6, PE7

	GPIOE->PUPDR |=
	     (0b10 << (4 * 2)) | (0b10 << (5 * 2)) |       // set 10 ==> pull-down
	     (0b10 << (6 * 2)) | (0b10 << (7 * 2));        // for each pin

    // Set LED pins to input for debugging
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;
}

// LEDs for debugging
void set_led_state(uint8_t state) {                // pass in a uint8_t value which represents the LED state
	led_state = state;                             // set the input of the function to the led_state
	*((uint8_t*)&(GPIOE->ODR) + 1) = led_state;    // update the LED state to the ODR
}


void start_game(){
	enable_clocks();
	initialise_board();
    enable_7seg_interrupts();
    enable_wire_interrupts();
}
