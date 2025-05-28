# The Feline Felon
# Project Overview
---
The Feline Felon is an interactive escape room-style game system built around a STM32F303 microcontroller platform. This project challenges players to complete a series of intricate puzzles and mini-games to successfully "break into" a secured location, combining elements of strategy, skill, and problem-solving.
# Roles and Responsibilities
---
The following were the allocations for the team:
- *Abdullah:*  Lock Picking module
- *Anikan:* Gantry design, LiDAR/TPU development
- *Ethan:* Motor control, piano module
- *Harsh:* LiDAR/TPU development
- *Nadeen:* Bomb defusal module
# Repo Workflow and Convention
---
Each member separately worked on their allocated tasks, meeting up regularly to check on task progress, and once complete, a meeting was held for integration.

The program files can be found in the folder with their respective names. The minutes can be found in the Meeting Minutes folder.

# Modules and Submodules 
---
# Gantry Control System

## Overview

This project implements a two-axis gantry control system using two NEMA-17 stepper motors, controlled by a user via keyboard input. The input is read from a Python script (`gantry.py`) running on a computer, and communicated to an STM32F303 microcontroller over a serial connection using USART1. The microcontroller then processes this input and drives the motors accordingly using the L298N H-bridge motor driver.

Motor stepping logic is implemented in software on the STM32F303, and the system supports movement in both cardinal (W, A, S, D) and diagonal (E, Q, Z, C) directions. The gantry system is room-aware, automatically adjusting motion bounds when the player moves into a new room.

---

## Features

- Dual-axis stepper motor control using software-driven stepping logic
- Full keyboard control with support for both cardinal and diagonal directions
- Real-time serial communication between PC and STM32F303 via USART1
- Integration with L298N H-bridge motor drivers
- Room-based boundaries for contextual control logic
- Idle detection and motor disabling for energy efficiency

---

## Usage Instructions

### STM32 Setup

1. Import the project into STM32CubeIDE.
2. Connect the STM32F303 to your computer using a Mini-USB to USB-A cable. Plug the Mini-USB into the ST-Link port of the STM32 board and the USB-A into your computer.
3. Build and flash the project to the microcontroller using default IDE settings.

### Python Script (`gantry.py`) Setup

1. Open `gantry.py` using a code editor such as Visual Studio Code.
2. Configure the correct COM port and baud rate. For example:

   ```python
   ser = serial.Serial('COM7', 115200)  # Change COM port as needed
   ```

3. Run the script.
4. Use the following keyboard controls:
   - `W`, `A`, `S`, `D`: Move up, left, down, and right respectively.
   - `E`, `Q`, `Z`, `C`: Move diagonally (NE, NW, SW, SE).
5. Room transitions occur automatically. Boundaries are updated based on the player’s position.

### Baud Rate Configuration

The serial baud rate is configured in `main.c` through the `SerialInitialise()` function:

```c
SerialInitialise(BAUD_19200); // Example
```

Supported baud rates:
- `BAUD_9600`
- `BAUD_19200`
- `BAUD_38400`
- `BAUD_57600`
- `BAUD_115200`

Ensure that the baud rate in `gantry.py` matches the one defined on the STM32. Invalid identifiers such as `baud_19200` or `BAUD_420` are not supported and will cause errors.

To verify the correct COM port:
- On Windows, open Device Manager and locate the COM port under “Ports (COM & LPT)”.

While terminal software like PuTTY can be used to verify serial functionality, the preferred method is to run `gantry.py` for full functionality and smoother motor control.

---

## Module Overview

### `digitalio.h` – Digital I/O Configuration

Provides system clock and GPIO initialization:
- `enable_clocks()`: Enables system and peripheral clocks including GPIO and timers.
- `initialise_board()`: Sets up GPIO pins as inputs or outputs as needed.

### `serial.h` – Serial Communication

Handles USART1-based serial communication:
- `SerialInitialise()`: Sets up serial port with specified baud rate and enables USART1.
- `handle_key()`: Processes incoming serial data and updates motor direction flags.
- `get_last_key()`: Returns the last valid key received via serial.

### `timers.h` – Timer Configuration and Management

Manages the timers used for motor control and idle detection:
- `enable_timer2_interrupt()`: Enables Timer 2 interrupt for millisecond tracking.
- `TIM2_IRQHandler()`: Timer 2 ISR for checking idle state based on input timing.
- `enable_timer3_interrupt()`: Enables Timer 3 for stepper motor timing.
- `TIM3_IRQHandler()`: Timer 3 ISR to control step sequence timing and motor speed.
- `get_time()`: Returns the current time in milliseconds.

### `motors.h` – Motor Control

Defines motor behavior and drive logic:
- `motor1_enable(direction)`: Enables motor 1 in the specified direction.
- `motor2_enable(direction)`: Enables motor 2 in the specified direction.
- `motor1_disable()`: Disables motor 1 when idle.
- `motor2_disable()`: Disables motor 2 when idle.
- `set_last_press_time(time)`: Records the last time an input was received.
- `motor_logic()`: Executes step sequence logic and checks movement bounds.
- `motor_idle(current_time, key)`: Handles idle state and disables motors when inactive.

### `gantry_logic.h` – Game State and Room Logic

Controls room-based gameplay logic:
- `check_room_state(motor)`: Evaluates current motor position to determine room state and triggers corresponding game activation logic.

---

## Notes

- The system is designed to be modular and extensible, allowing for additional motor axes, sensor integration, or gameplay functionality.
- Motor performance and responsiveness depend heavily on the chosen baud rate and timer configurations.
- Ensure proper power supply and grounding when using external drivers like the L298N with the STM32 board.


# Capacitive Touch Piano Module

## Overview

This module implements a capacitive touch piano using the STM32F303 microcontroller and the built-in Touch Sensing Controller (TSC). The player must correctly play a predefined song sequence to unlock the door and proceed. While its connection to a robbery may be questionable, the gameplay mechanic adds a unique musical challenge.

The piano consists of five touch-sensitive keys, each corresponding to a musical note (C, D, E, F, G). Upon pressing a key, the corresponding note is sent via serial to a Python script (`piano.py`), which plays a `.wav` file for that note through the computer.

---

## Features

- Capacitive touch input using STM32's TSC hardware abstraction layer (HAL)
- Five playable keys: C, D, E, F, G
- Real-time serial communication with a PC to play audio files
- Validates note sequences to determine whether the correct song is played
- Visual feedback using onboard LEDs
- Game restarts automatically upon success or error
- Waits for external game trigger via GPIO (PC1)

---

## Usage Instructions

### STM32 Setup

1. Import the project into **STM32CubeIDE**.
2. Build and flash the project to the STM32F303 using the default configuration.
3. Connect the STM32F303 to your computer using a Mini-USB to USB-A cable (Mini-USB connects to the ST-Link port).
4. Ensure PC1 is pulled high externally to start the game.

### Python Script (`piano.py`) Setup

1. Open `piano.py` using a text editor or IDE (e.g., Visual Studio Code).
2. Configure the serial port and baud rate by modifying the following line:

   ```python
   ser = serial.Serial('COM7', 115200)  # Update COM port and baud rate as required
   ```

3. Run the Python script.
4. Touch the resistive piano keys to play notes. If you play the correct song sequence, the game will signal completion and reset.

---

## Module Overview

### `digitalio.h` – Digital I/O Management

Handles general-purpose input/output configuration and LED control:

- `enable_clocks()`: Enables GPIO and timer peripheral clocks.
- `initialise_gpio()`: Configures all necessary GPIO pins for input/output.
- `set_leds(pattern)`: Lights up STM32 onboard LEDs based on the given pattern.
- `clear_leds()`: Turns off all STM32 onboard LEDs.
- `game_wait()`: Polls GPIO pin PC1 and blocks execution until it is set high, signaling the start of the game.

---

### `timer.h` – Timer Control

Responsible for handling time-dependent events:

- `enable_timer2_interrupt()`: Enables Timer 2 for debouncing key presses to prevent repeated inputs. Only one input is allowed per second.
- `enable_timer3_interrupt()`: Enables Timer 3, used to clear the key press state after the key is released.

---

### `serial.h` – Serial Communication

Manages serial communication between the STM32 and the host PC:

- `SerialInitialise(baud_rate)`: Sets up USART1 with the specified baud rate.
- `tx_char(character, serial_port, piano, key)`: Sends a character via USART1 to `piano.py`, which plays the corresponding audio note.

---

### `game_logic.h` – Game State and Input Logic

Implements the core gameplay and key-press validation:

- `HAL_TSC_ConvCpltCallback(htsc)`: Callback triggered by touch sensing completion. Checks if any piano key is touched and handles accordingly.
- `key_press_logic(sample_value, piano, key, threshold, serial_port)`: Determines whether a valid key press occurred and triggers audio + LED feedback.
- `gameplay_logic(piano, key)`: Evaluates the current key in the context of the predefined song sequence (e.g., "Ode to Joy"). Resets if the wrong note is played or multiple notes are pressed simultaneously.
- `completion_function(piano)`: Executes when the entire song sequence is played correctly. Sends a signal (pulse on PC9) to the main game controller to unlock the door.
- `reset_piano_state(piano)`: Resets all piano state variables to allow the game to be played again.

---

## Notes

- All touch sensing logic relies on STM32’s HAL for TSC configuration and sampling.
- The game will not begin until PC1 is pulled high externally—ensure this condition is met before testing.
- The `.wav` files must be present on the computer running `piano.py`, and mapped correctly to the serial input characters.
- Make sure your serial COM port matches between `piano.py` and the STM32, and that the baud rate is consistent on both ends.



# Bomb Defusal Game

## Overview

This module implements a timed bomb defusal game using the STM32F3 Discovery board. The player must correctly identify and cut two specific wires within a 30-second countdown. Incorrect wire pulls deduct time, and either two wrong pulls or the timer reaching zero results in failure. A buzzer is triggered on failure, while success opens a door via a signal to the main board.

The game architecture is modular, separating functionality across digital I/O, timers, game logic, and 7-segment display control. The game begins upon receiving a start signal via GPIO from the main control board.

---

## Usage Instructions

### STM32 Setup

1. Import the project into **STM32CubeIDE**.
2. Flash the program to the STM32F303 using the default configuration.
3. Connect wire inputs to pins PE4–PE9 and 5V. Each pin is internally pulled down and triggers an interrupt when pulled to ground (i.e., when the wire is removed).
4. The game begins when PC2 receives a high signal from the main control board.
5. A 7-segment display counts down from 30 seconds.
6. Correct wire pulls increase progress toward defusal.
7. Two incorrect pulls or timeout triggers the buzzer (PA2 via PWM), and the game ends.
8. Upon success, a high signal on PC8 is sent to the main board to open the door.

---

## Valid Inputs

- **Game Start Trigger**: PC2 goes high (received from the main board).
- **Wire Pull Inputs**: PE4–PE9. Connected to wires pulled to 5V. Internally configured as pull-down. A pulled wire registers as logic 0 and triggers an interrupt.
- **Buzzer Output**: PA2, configured as TIM2 Channel 3 with PWM.
- **Game Completion Output**: PC8 goes high on success.

---

## Module Overview

### `main.c` / `main.h` – Entry Point & Buzzer Control

The main function configures all HAL peripherals and loops until a game start signal is received via PC2. Once detected, `start_game()` is called to launch the defusal game.

- `enable_and_initialise()`: Enables GPIOC clocks, sets PC1 as input with pull-down to read the start flag.
- `play_note(uint16_t arr_val)`: Enables the buzzer using PWM output from PA2. Uses `__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, arr_val)` to control buzzer tone.

---

### `digital_io.c` / `digital_io.h` – Digital I/O and Game Signal Control

Manages all GPIO configuration and signals for both gameplay and hardware interaction.

- `enable_clocks()`: Enables GPIO clocks for Ports A, C, D, E and Timer 3.
- `initialise_board()`: Sets up:
  - PD0–PD6 as output pins for 7-segment segments
  - PE4–PE9 as inputs with interrupts for wire pulling (pull-down configuration)
  - PC6–PC8 as output pins for 7-segment digit select and game completion signal
- `set_led_state(state)`: Updates onboard LEDs to indicate game status.
- `buzzer_on()`: Plays a 100ms buzzer tone using PWM on TIM2 Channel 3.
- `open_door()`: Sets PC8 high to signal game success to the main board.
- `start_game()`: Initializes and enables all required components (clocks, timers, interrupts, game state) to begin gameplay.

---

### `timer.c` / `timer.h` – Timer Configuration and Game Logic

Handles countdown logic and multiplexing of the 7-segment display using TIM3.

- `enable_7seg_interrupts()`: Configures Timer 3 to trigger every 5 ms. Uses prescaler = 47000–1 and ARR = 5.
- `TIM3_IRQHandler()`: 
  - Called every 5 ms.
  - Decrements the `time_left` counter every 200 ticks (~1 second).
  - Updates display using `update_display(time_left)`.
  - Calls `end_game()` if `time_left` reaches 0.
- `stop_timer()`: Disables Timer 3 and stops countdown logic.
- `reduce_time(uint8_t seconds)`: Deducts time from the countdown and updates the 7-segment display.

---

### `seven_seg.c` / `seven_seg.h` – 7-Segment Display Management

Controls a dual-digit multiplexed 7-segment display.

- `update_display(int val)`: 
  - Splits input into tens and units digits.
  - Uses a digit mapping array to determine which segments to illuminate.
  - Alternates between the two digits for display multiplexing (driven by Timer 3 interrupts).

---
### `gamelogic.c` / `seven_seg.h` – 7-Segment Display Management
- `void enable_wire_interrupts(void)` - Configures EXTI interrupts on PE4–PE9 (wires). Maps each EXTI line to Port E via SYSCFG and enables falling edge trigger for all 6 pins.
- `void EXTI4_IRQHandler(void)` and `void EXTI9_5_IRQHandler(void)` - this is the interrupt handler which handles each wire pull. Based on the interrupt handler called a index is fixed and `handle_wire_pull(int wire_index)` is called.
- `void handle_wire_pull(int wire_index)` - This is the main logic for handling a wire pull interrupt. It ignores the imterrupt if the game is over or if the wire was already pulled.If the wire is correct (based on correct_wires[]), increments correct_count. If incorrect, reduces the time by 10 seconds, updates the display, and increments wrong_count. Calls end_game(1) if all correct wires are pulled. Calls end_game(0) if time runs out or if 2 wrong wires are pulled. Unmasks interrupt lines and enables NVIC handlers.
- `void end_game(int end_of_game)` - This is the function that runs to end the game. It decides the outcome of the game based on the input of the function. It sets the game_over flag to high and if end_of_game glag is set to 1 it stops the timer, sets LED pattern, and triggers open_door(). If end_of_game == 0 it stops the timer, sets display to 0, and turns on the buzzer.

This controles the game logic and uses interrupts to detect wire pulls.

## Individual Testing

Individual Module testing for each module is given bellow,

### `main.c` / `main.h` 
Ensure PC2 detects a HIGH signal correctly from another board and triggers start_game(). Manually call play_note(arr_val) with known values and confirm buzzer tone plays correctly on PA2 using PWM.

### `digital_io.c `
- Confirm PE4–PE9 read LOW by default and trigger interrupts on falling edge when connected to 5V.
- Toggle PD0–PD6 manually and verify 7-segment segments light up accordingly.
- Call open_door() and check PC8 goes HIGH using multimeter.
- Call buzzer_on() and verify short buzzer beep.

### timer.c 
- Start Timer 3 using enable_7seg_interrupts() and verify periodic interrupts are firing (e.g., via a debug LED).
- Confirm countdown value decrements every second and try using different Prescaller or ARRR values.
- Test reduce_time() and validate that time subtracts correctly and updates display.
- Verify stop_timer() disables countdown properly.

### seven_seg.c
- Call update_display(val) with multiple values (e.g., 30, 15, 01) and confirm the correct digits show.
- Check that digits alternate correctly and segments are lit properly for each digit.

### game_logic.c 
- Simulate correct wire pulls and confirm correct_count increments.
- Simulate wrong wire pulls and verify wrong_count increments and time reduces.
- Confirm game ends correctly after 2 wrong pulls or time reaching 0.
- On win, ensure open_door() is called; on loss, verify buzzer triggers.

## Integration Testing
- Power on the system, confirm idle state until PC2 goes HIGH.
- Trigger a start signal and verify countdown begins.
- Test wire pulls in live conditions (pull correct/incorrect wires).
- Observe correct game state transitions: win or loss and time decreasing.
- Ensure buzzer, LED, and 7-segment all respond according to outcome.
- Verify PC8 HIGH signal is sent only on success.


## Game Flow

1. Wait for game start signal (PC2 high).
2. Countdown begins from 30 seconds (displayed on 7-segment).
3. Player pulls wires from PE4–PE9.
   - Correct wires increase game progress.
   - Wrong wires reduce remaining time via `reduce_time()`.
4. Game ends in one of three ways:
   - **Success**: Two correct wires pulled. `open_door()` is triggered.
   - **Failure**: Two incorrect wires pulled. `buzzer_on()` is triggered.
   - **Timeout**: Countdown reaches 0. `buzzer_on()` is triggered.

---

## Notes

- Buzzer is controlled via PWM on PA2 (TIM2_CH3).
- Wire inputs are configured as pull-down, triggering an interrupt on logic low.
- The game is fully modular, with logic split across digital I/O, timers, and display modules for clarity and maintainability.
- Ensure all peripheral clocks are enabled and timers configured before triggering gameplay.
  
---

# Pan-Tilt Unit + LiDAR

## Summary
This module implements a real-time hide-and-seek style game using a Pan-Tilt Unit (PTU) equipped with a Garmin LiDAR-Lite v3 sensor. The PTU performs a raster scan of its surroundings to detect motion or intrusions based on deviations from a pre-recorded baseline depth map. This module runs continuously on the STM32F3 Discovery board and communicates results via UART.

## Operating Principle
### Raster Scanning
- The PTU is controlled via two PWM channels (pan and tilt), which position the LiDAR sensor in a 3D scan grid.
- The PTU performs a 180 ° × 90 ° raster scan (6 × 5 grid).
- After each PWM adjustment, a delay (~40 ms) allows mechanical settling before capturing a distance measurement.

### Baseline Acquisition
- On startup, the full scan grid is traversed.
- Distance readings are collected at each point and streamed over UART in CSV format.
- This data serves as a reference model for an “empty room.”

### Continuous Monitoring
- The PTU continuously repeats the scan cycle
- At each position, the live distance is compared with the baseline value.
- Any significant deviation (indicating presence of a foreign object or movement) is flagged as an intrusion.
- Intrusion data (coordinates, distance error) is logged and optionally transmitted.

* **MCU:** STM32F3‑Discovery (Cortex‑M4, FPU)
* **PTU:** Mercury Gimbal (dual hobby servos)
  \* Horizontal PWM on `TIM3‑CH1` (PA6)
  \* Vertical   PWM on `TIM3‑CH2` (PA7)
* **Distance sensor:** Garmin LiDAR‑Lite v3 on `PB6/PB7` (I²C‑1)
* **Host PC:** Any OS with Python 3.10+

\## Serial Frame Format

```text
V:<vert_pwm> H:<horiz_pwm> D:<distance_cm>cm\n
Example:  V:1023 H:1500 D:187cm
```


# Find the Correct Voltages 

## Overview
This project modularizes the implementation of finding correct voltages using the STM32 Discovery board. The system uses a modular design to distribute tasks across the project, incorporating analog-to-digital conversion, digital I/O, and game logic. The challenge starts with three potentiometers, where the user must determine the correct voltages. The obstacle is that the user must find two correct voltages to unlock the third potentiometer. Each time the user finds a correct voltage, an LED corresponding to that potentiometer lights up.
## Usage Instrctions 
### STM32 Setup
1. Import the project into STM32Cube IDE
2. Flash the program to the STM32 using the default configuration 
3. Connect the each potentiometer to 5V and GND 
4. Connect each potentiometer output to PC1-PC3 pins in the STM32
5. The game will be start and the user has to change the first and second LED until the LEDs light up 
6. If the user find the two first voltages then the user can find the third voltage
7. If all LEDs are correct then the door will open 
## VALID SUCCESS
- LED1 lights when PC1 voltage matches 1V ± 100mV.
- LED2 lights when PC2 voltage matches 1.5V ± 100mV.
- LED3 lights when PC3 voltage matches 2V ± 100mV, only after LED1 and LED2 are lit.
- PA0 outputs high when all LEDs are lit, indicating game completion.

## Module Overview 
## main.c 
- Call `enable_clocks()` to enable GPIOA, GPIOC, and GPIOE clocks.
- Run `initialise_board()` to set up LED outputs (PE8-PE15) and analog inputs (PC1-PC3).
- Use `ConfigureADC()` to initialize ADC1 for voltage readings.
- Start the game with `SequentialVoltageGameADC()` to enter the main loop.
- Adjust potentiometers to match target voltages and light LEDs.
## peripherals.h / peripherals.c
- **Initialization**:
  - `enable_clocks()`: Enables RCC clocks for GPIOA, GPIOC, and GPIOE.
  - `initialise_board()`: Sets PE8-PE15 as outputs, PC1-PC3 as analog inputs, PA0 as digital output.
  - `ConfigureADC()`: Configures ADC1 with clock sync, calibration, and single-ended mode.
- **ADC Operations**:
  - `ReadADC(channel)`: Reads 12-bit ADC value from a specified channel.
  - `ConvertADCToMillivolts(adcValue)`: Converts ADC value to millivolts using ADC_VREF_MV.
- **Game Logic**:
  - `SequentialVoltageGameADC()`: Loops indefinitely, reads ADC channels, controls LEDs based on voltage thresholds.


## DIGITAL I/O
- **Inputs**: PC1, PC2, PC3 as analog inputs for potentiometers.
- **Outputs**: PE8 (LED1), PE9 (LED2), PE10 (LED3) for LEDs; PA0 for win signal.
- **ADC Channels**: Configured for PC1, PC2, PC3 (channels defined in peripherals.h).
- **Voltage Reference**: Defined by ADC_VREF_MV (typically 3000mV).
- **Resolution**: 12-bit ADC (0-4095 range).


