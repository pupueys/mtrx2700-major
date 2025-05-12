#include <digitalio.h>


void enable_clocks(void) {

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOEEN;		//enabling GPIOA and GPIOE clocks

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;           // enable clock for timer 2
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;           // enable clock for timer 3

}

void initialise_leds(void) {
	// get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;
}

void set_leds(uint8_t key) {

	GPIOE->ODR |= (1 << (8 + key));   // Turn ON LED corresponding to key

}

void clear_leds(uint8_t key) {

	GPIOE->ODR &= !(1 << (8 + key));  // Turn OFF LED corresponding to key
}

