# MTRX Group 3

##  Date - 14/05/2025
### Present:
- Nadeen  
- Anikan  
- Ethan  
- Abdullah  
- Harsh  

### Meeting Purpose:
Progress review and inter-module compatibility discussion

#### Mid-Development Updates
- Wire cut game improved with incorrect-wire detection logic and countdown timer.
- Piano game added loop and added a logic for the right pattern.
- Voltage game incorporated ADC and added LEDs to show correct voltage.
- Gantry could now traverse full path and return to start.
- Servo doors responded correctly to button press. Need to change it such that i can react to GPIO logic.
- PTU began environmental scanning and rotated accordingly.
- Started testing communication between modules (e.g., triggering next game).
- Discussed implementing a shared flag variable or GPIO signaling for game sequencing.
- Noted issues with noise in analog input during voltage game testing.
- Reviewed game fail-states and how players would retry.

#### Action Items (To be completed before 21/05/25)
- Nadeen to modularise wire cut game code.
- Ethan to handle piano game fail-states and GPIO communication.
- Abdullah to resolve analog noise and update logic.
- Anikan to print remaining parts and verify alignment.
- Harsh to implement PTU tracking reaction when player detected.

### Previous Actions:
- Individual games upgraded and extended

### Next Meeting Date:
21/05/25
