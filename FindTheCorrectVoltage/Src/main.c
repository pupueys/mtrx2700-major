#include <stdint.h>
#include "stm32f303xc.h"
#include "peripherals.h"

int main(void)
{
    // Initialize board and peripherals
    enable_clocks();
    initialise_board();

    // Run the sequential voltage game
    SequentialVoltageGameADC();

    // This point should never be reached
    while(1);
}
