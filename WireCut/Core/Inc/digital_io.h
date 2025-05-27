#ifndef DIGITAL_IO_H
#define DIGITAL_IO_H

// FUNCTIONS

void enable_clocks();
// Enable all the clocks for GPIO and timers
// Input - NA
// Output - All clocks enabled
void initialise_board();
// Initialise all required GPIO pins to its respective configuration
// Input - NA
// Output - All pins configured accordingly
void start_game();
// Call enable_clocks and initialise_boards functions enable 7 seg interrupts and wire interrupts
// Input - NA
// Output - All functions required are called and runs the game
void buzzer_on();
// Turn buzzer on by calling play_note function
// Input - NA
// Output - runs the play_note function
void set_led_state(uint8_t state);
// Turn on board LEDs according to the input pattern
// Input - Bit pattern required to turn LEDs on
// Output - turns the LEDs on but modifying ODR
void open_door();
// opens the door by setting PA8 as high which send the signal to the main board

// VARIABLES

extern volatile int time_left;           // Represents the time left which will be displayed in 7 seg
extern volatile uint8_t led_state;       // contains the led pattern

#endif
