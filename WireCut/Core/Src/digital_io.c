#include "stm32f303xc.h"
#include "digital_io.h"
#include "timer.h"
#include "seven_seg.h"
#include "game_logic.h"
#include "main.h"

volatile int time_left = 30;
volatile uint8_t led_state;

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


	// Set pin PE 4,5,6,7,8, and 9 in pull down configuration to read 0 when pulled out
	GPIOE->PUPDR &=
	    ~((0b11 << (4 * 2)) | (0b11 << (5 * 2)) |      // clear PE4,5,6,7,8 and 9
	      (0b11 << (6 * 2)) | (0b11 << (7 * 2)) |
		  (0b11 << (8 * 2)) | (0b11 << (9 * 2)) );

	GPIOE->PUPDR |=
	    (0b10 << (4 * 2)) | (0b10 << (5 * 2)) |       // 10 for pull down configuration
	    (0b10 << (6 * 2)) | (0b10 << (7 * 2)) |
	    (0b10 << (8 * 2)) | (0b10 << (9 * 2));

	// PC6 and PC7 as output for digit control and PC 8 as output for door
	GPIOC->MODER &= ~(0b11 << (6*2) | 0b11 << (7*2) | 0b11 << (8*2) );
	GPIOC->MODER |=  (0b01 << (6*2) | 0b01 << (7*2) | 0b01 << (8*2) );




    // Set LED pins to input for debugging
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;
}

// LEDs for debugging
void set_led_state(uint8_t state) {                // pass in a uint8_t value which represents the LED state
	led_state = state;                             // set the input of the function to the led_state
	*((uint8_t*)&(GPIOE->ODR) + 1) = led_state;    // update the LED state to the ODR
}

void buzzer_on(){
	play_note(150);                                // play buzzer using 150 signal

}

void open_door(){
    GPIOC->ODR |= (1 << 8);                        // set PC8 as high to open the doors
}


void start_game(){
	enable_clocks();                               // enable all clocks
	initialise_board();                            // initialise all inputs, outputs and pull down config
    enable_7seg_interrupts();                      // enable timer 3 interrupt for 7 seg
    enable_wire_interrupts();                      // enable pull down interrupt for 6 pins
    time_left = 30;
}
