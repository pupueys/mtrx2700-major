#include <seven_seg.h>
#include "stm32f303xc.h"

const uint8_t digits[11] = {
		0b00111111, // 0
		0b00000110, // 1
		0b01011011, // 2
		0b11001111, // 3
		0b01100110, // 4
		0b01101101, // 5
		0b01111101, // 6
		0b00000111, // 7
		0b01111111, // 8
		0b11101111, // 9
		0b00000000  // turn off

};

void update_display(int val) {
    static int toggle = 0;
    int tens = val / 10;
    int ones = val % 10;

    if (toggle == 0) {
        // Show tens
        GPIOD->ODR = (GPIOD->ODR & ~0xFF) | digits[tens];
        // Activate digit 1 (PC7 low, PC6 high )
        GPIOC->ODR |= (1 << 6);
        GPIOC->ODR &= ~(1 << 7);
    } else {
        // Show ones
        GPIOD->ODR = (GPIOD->ODR & ~0xFF) | digits[ones];
        // Activate digit 2 (PC7 high, PC6 low )
        GPIOC->ODR |= (1 << 7);
        GPIOC->ODR &= ~(1 << 6);
    }

    toggle = !toggle;
}
