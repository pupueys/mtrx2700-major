#ifndef DIGITALIO_H
#define DIGITALIO_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "timer.h"
#include "stm32f303xc.h"


// define the type of callback which is a function pointer that takes no input and gives no output
typedef void (*callback)(void);


// Function declarations
void enable_clocks(void);
void initialise_leds(void);
void set_leds(uint8_t key);
void clear_leds(uint8_t key);

#endif
