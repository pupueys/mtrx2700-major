
#ifndef TIMER_H
#define TIMER_h

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <digitalio.h>
#include "stm32f303xc.h"

void enable_timer_interrupt(void);
void TIM3_IRQHandler(void);

#endif
