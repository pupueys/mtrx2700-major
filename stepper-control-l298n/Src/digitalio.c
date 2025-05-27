#include "digitalio.h"

void enable_clocks(void) {

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN
				| RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIODEN | RCC_AHBENR_GPIOEEN;		//enabling GPIOA and GPIOE clocks

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 		  // Enable SYSCFG clock

}


// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_board(void) {

	// setting up GPIOD for output
	uint16_t *output_registers = ((uint16_t *)&(GPIOD->MODER));
	*output_registers = 0x5555;

	// get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;

	// 2. Configure PA1, PA2, PA3 as general-purpose output mode (MODER = 0b01)
	GPIOA->MODER &= ~((0b11 << (1 * 2)) |  // Clear MODER1
	                  (0b11 << (2 * 2)) |  // Clear MODER2
	                  (0b11 << (3 * 2)));  // Clear MODER3

	GPIOA->MODER |=  ((0b01 << (1 * 2)) |  // Set MODER1 to 0b01 (output)
	                  (0b01 << (2 * 2)) |  // Set MODER2 to 0b01
	                  (0b01 << (3 * 2)));  // Set MODER3 to 0b01
}

