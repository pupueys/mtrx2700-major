/*
 * timer.h
 *
 *  Created on: May 6, 2025
 *      Author: nadeen
 */

#ifndef TIMER_H_
#define TIMER_H_

extern volatile int time_left;
void enable_7seg_interrupts();
void TIM3_IRQHandler(void);
void stop_timer(void) ;
void reduce_time(uint8_t seconds);



#endif /* TIMER_H_ */
