#include <digitalio.h>


void enable_clocks(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;  // Enable GPIOA and GPIOE clocks
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;                      // Enable clock for Timer 2
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;                      // Enable clock for Timer 3
}

void initialise_gpio(void) {
	// get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;

	// configuring MODE8 to be an input signal
	GPIOC->MODER &= ~(0b11 << (8 * 2));  // Clear MODER8[1:0]

	// configuring MODE9 to be an output signal
	GPIOC->MODER &= ~(0b11 << (9 * 2));
	GPIOC->MODER |= (0b01 << (9 * 2));
}

void set_leds(uint8_t key) {

	GPIOE->ODR |= (1 << (8 + key));   // Turn ON LED corresponding to key

}

void clear_leds(uint8_t key) {

	GPIOE->ODR &= !(1 << (8 + key));  // Turn OFF LED corresponding to key
}

void game_wait(void) {

	while((GPIOC->IDR & (1 << 8)) == 0) {

	}

}

