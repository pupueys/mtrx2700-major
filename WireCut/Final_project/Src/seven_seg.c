#include <seven_seg.h>
#include "stm32f303xc.h"

const uint8_t digits[10] = {
		0b00111111, // 0
		0b00000110, // 1
		0b01011011, // 2
		0b11001111, // 3
		0b01100110, // 4
		0b01101101, // 5
		0b01111101, // 6
		0b00000111, // 7
		0b01111111, // 8
		0b11101111   // 9
};

void update_display(int val) {
    GPIOD->ODR = (GPIOD->ODR & ~0xFF) | digits[val];
}
