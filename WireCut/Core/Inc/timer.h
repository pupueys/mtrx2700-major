/*
 * timer.h
 *
 *  Created on: May 6, 2025
 *      Author: nadeen
 */

#ifndef TIMER_H_
#define TIMER_H_

// FUNCTIONS

void enable_7seg_interrupts();
// Enables the 7 seg timer interrupts which calls the handler every 5 ms
// Input - NA
// Output - calls the handler every 5 ms

void TIM3_IRQHandler(void);
// Calls the update display function to update the time left to the 7 seg and decreases the time left by 1 every 1 second
// Input - NA
// Output - calls the update display function and reduces time left

void stop_timer(void) ;
// Stops the timer once the game is over

void reduce_time(uint8_t seconds);
// Reduces the time left by the amount of input as a penalty for wrong wire pull
// Input - the amount of time that needs to be decreased
// Output - updates time left and calls the update_time function


// VARIABLES
static int tick;


#endif /* TIMER_H_ */
