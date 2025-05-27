/*
 * game_logic.h
 *
 *  Created on: May 6, 2025
 *      Author: nadeen
 */

#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_

// Functions

void enable_wire_interrupts();
// Enable interrupts on falling edge for PE4–PE9
// Input - NA
// Output - Call the respective interrupt handler when wire is pulled

void handle_wire_pull(int wire_index);
// Called when interrupt is happened, updates the game variables and checks if game is over
// Input - The index of the wire that is pulled based on the handler called
// Output - Updates the state of the game variables and end game is end condition is true

void end_game(int end_of_game);
// End the game and based on the input see if the player won or not
// Input - Flag indicating win or lose
// Output - open the doors or turn the buzzer on based on the input flag


// Variables

static volatile uint8_t wire_pulled[6];                         // Keep track of the wires pulled
static const uint8_t correct_wires[6];                          // hard code wires 1 and 3 to be correct
static volatile uint8_t correct_count;                          // Number of correct wires pulled
static volatile uint8_t wrong_count;                            // Number of wrong wires pulled
static volatile uint8_t game_over;                              // Indicate game over



#endif /* GAME_LOGIC_H_ */
