/*
 * game_logic.h
 *
 *  Created on: May 6, 2025
 *      Author: nadeen
 */

#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_

void enable_wire_interrupts();
void handle_wire_pull(int wire_index);
void end_game(int end_of_game);

extern volatile int time_left;



#endif /* GAME_LOGIC_H_ */
