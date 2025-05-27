/*
 * 7seg.h
 *
 *  Created on: May 6, 2025
 *      Author: nadeen
 */

#ifndef SEVEN_SEG_H_
#define SEVEN_SEG_H_


// FUNCTIONS

void update_display(int val);
// Update the display based on the time left. Do multiplexing to display both ones and tens at the same time.
// Input - the time left in the game
// Output - ODR is modified to update the time left

#endif /* 7SEG_H_ */
