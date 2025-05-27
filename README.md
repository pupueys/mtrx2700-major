# mtrx2700-major project 
# Project Overview
---
# Roles and Responsibilities
---
# Repo Workflow and Convention
---
# Modules and Submodules 
---
## Wire Cut Game
### Summary
---
This project modularises the implementation of a wire-cutting game using the STM32F3 Discovery board. The system uses multiple modules to separate responsibilities across digital I/O, game logic, and 7 Seg display control. The game begins on a signal from the main board and requires the player to pull the correct wires within a countdown period of 30 seconds. Pulling the wrong wire reduces the time, and 2 wrong wire pulls or timer going to 0 results in the failure of the game. A buzzer goes off if the player fails. If the player cuts the correct two wires the player wins and a signal goes to the main board via GPIO signaling to start the next game and open the door.

### Usage
---
- Import the project into STM32CubeIDE.
- Run the main file. And the game starts when a signal from the main board is received.
- The 7-segment display counts down from 30 seconds.
- The player pulls wires connected to PE4 to PE9 and 5V. Correct pulls increase progress; wrong pulls reduce remaining time.
- Success opens a door via PC8. Failure plays a buzzer sound using PWM.

### Valid Input
---
- Game starts when PC2 reads a high value coming from main board.
- Pull wires connected to PE4–PE9 which are then connected to 5v. Its is pull down configuration so when wire is pulled the pin reads 0v and interrupt is triggered.
- PA2 is set to output a PWM signal using HAL
- Game logic decides the output of the game.

###  Functions and Modularity 
---
#### main.c / main.h
HAL was used to set pin PA2 to tim2/ch3 which outputs a PWM signal. A prescaller of 47 and ARR of 500 was set to create appropriate PWM signals. This PA2 is used to turn the buzzer on or off.
The main function is the entry point of the code where it first configure all HAl releated peripherals and waits for the game start flag in a while loop which is sent by the main board and then runs the `start_game()` function which starts the wire cut game.
- `void enable_and_initialise()` - This function is used to enable the GPIOC clocks and set PC1 as an input. It also sets PC1 in a pull down configuration to read the flag coming from main board.
- `void play_note(uint16_t arr_val)` - This function is called in the digital_io.c function. It uses the PWM signal from PA2 to turn the buzzer on using the input value passed onto the function. `__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, arr_val)` is used to trun the buzzer on.
---
#### digital_io.c / digital_io.h_
This module handles all digital IO parts of the code like initialising the pins, enabling clocks and sending signals indicating end of game.
- `enable_clocks()` - Enables clocks for GPIO ports (A, C, D, E) and Timer 3 for 7 seg display.
- `initialise_board` - Initialises GPIO pins PD0-6 for 7 segment, PE4-9 for wire interrupts and also sets it as pull down. It also sets PC6-8 as outputs for digit control and game_over signal.
- `set_led_state(state)` - Turns the on board LEDs based on the pattern sent as a input to the function. It is used to test the end results of the game.
- `buzzer_on()` - Triggers the buzzer using PWM on TIM2 Channel 3 to generate a tone for 100 ms.
- `open_door()` - It sets PC8 as high which is connected to the main board. This signals that the game is over and the doors should be opened.
- `start_game()` -  High-level setup function that calls all other initialisation functions, resets time_left, and enables interrupts for 7-segment and wires.This basically starts the entire game when called.
  
---
#### timer.c / timer.h
This module handles all timer related functions in 1 file. Everything related to timers are programmed in this so that it is modular and easily accessible.
- `enable_7seg_interrupts()` - Configures Timer 3 to trigger every 5 ms and the interrput handler is called. A presclaler of 47000 - 1 and ARR of 5 was choosen to create a 5 ms timer. This interrupt is used for multiplexing and countdown logic.
- `TIM3_IRQHandler()` - This handles the tim 3 interrupt and it decreases the `time_left` by 1 every 200 ticks which will amount to 1 second since the handler is called every 5 ms. The updated time_left is then get updated to the display by using `update_display(time_left)` function. This function also calls the `end_game()` when the time_left hits 0.
- `stop_timer()` - This is the function that is called when the time_left is 0 or if the game is over. It stops the timer 3 interrupt so that the time_left is no longer updated.
- `reduce_time(uint8_t seconds)` - This function is called when the user pulls the two wrong wires. The input of the function is the amount of time that needs to be detucted. Then this time if subracted from the total time left. Later `update_display(time_left)` function is called to update the display.
  
---
#### seven_seg.c/seven_seg.h
Manages the 2-digit 7-segment display. It has a array of values representing each number that should be displayed. 
- `void update_display(int val)` - This function first splits the input into tens and ones and then it uses the digit array to map the respective 
  


