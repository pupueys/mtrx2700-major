//#include "motors.h"
//
//// setting up motor parameters
//static volatile	bool motor1_running = false;
//static volatile	uint8_t motor1_dir = FORWARD;
//static volatile uint8_t motor1_step_index = 0;
//
//static volatile	bool motor2_running = false;
//static volatile	uint8_t motor2_dir = FORWARD;
//static volatile uint8_t motor2_step_index = 0;
//
//static volatile uint32_t last_press_time = 0;
//
//const uint8_t full_step_sequence[4][4] = {
//    {1, 0, 1, 0}, // Step 1
//    {0, 1, 1, 0}, // Step 2
//    {0, 1, 0, 1}, // Step 3
//    {1, 0, 0, 1}  // Step 4
//};
//
//void motor1_enable(uint8_t direction) {
//
//	motor1_running = true;
//	motor1_dir = direction;
//
//}
//
//void motor2_enable(uint8_t direction) {
//
//	motor2_running = true;
//	motor2_dir = direction;
//
//}
//
//void motor1_disable(void) {
//
//	motor1_running = false;
//	*((uint8_t*)&(GPIOD->ODR)) &= ~(0b00011110);
//
//}
//
//void motor2_disable(void) {
//
//	motor2_running = false;
//	*((uint8_t*)&(GPIOD->ODR)) &= ~(0b11110000);
//}
//
//void set_last_press_time(uint32_t time) {
//	last_press_time = time;
//}
//
//void motor_logic(void) {
//    if (motor1_running) {
//        // Update index
//        if (motor1_dir == FORWARD) {
//            motor1_step_index = (motor1_step_index + 1) % 4;
//        } else {
//            motor1_step_index = (motor1_step_index + 3) % 4;
//
//        }
//
//        // Write IN1–IN4 to PD0–PD3
//        uint8_t *s = full_step_sequence[motor1_step_index];
//        GPIOD->ODR = (GPIOD->ODR & ~(0xF)) |
//                     (s[0] << 0) | (s[1] << 1) |
//                     (s[2] << 2) | (s[3] << 3);
//
//        GPIOE->ODR = (GPIOE->ODR & ~((1 << 8) | (1 << 9) | (1 << 10) | (1 << 11))) |
//                     (s[0] << 8) | (s[1] << 9) |
//                     (s[2] << 10) | (s[3] << 11);
//    }
//
//    if (motor2_running) {
//        if (motor2_dir == FORWARD) {
//            motor2_step_index = (motor2_step_index + 1) % 4;
//        } else {
//            motor2_step_index = (motor2_step_index + 3) % 4;
//        }
//
//        // Write IN1–IN4 to PD4–PD7
//        uint8_t *s2 = full_step_sequence[motor2_step_index];
//        GPIOD->ODR = (GPIOD->ODR & ~(0xF0)) |
//                     (s2[0] << 4) | (s2[1] << 5) |
//                     (s2[2] << 6) | (s2[3] << 7);
//        GPIOE->ODR = (GPIOE->ODR & ~(0xF << 12)) |
//                     (s2[0] << 16) | (s2[1] << 17) |
//                     (s2[2] << 18) | (s2[3] << 19);
//    }
//}
//
//void motor_idle(uint32_t current_time) {
//
//	uint8_t current_key = get_last_key();
//
//    if (motor1_running) {
//
//    	if ((current_time - last_press_time > 50) ||
//    		(current_key != 'w' && current_key != 's'
//    		&& current_key != 'e' && current_key != 'q' && current_key != 'z' && current_key != 'c')) {
//    		motor1_step_index = 4;
//    		motor1_disable();
//    	}
//    }
//
//    if (motor2_running) {
//    	if ((current_time - last_press_time > 50) ||
//    		(current_key != 'a' && current_key != 'd'
//			&& current_key != 'e' && current_key != 'q' && current_key != 'z' && current_key != 'c')) {
//    		motor2_step_index = 4;
//    		motor2_disable();
//    	}
//    }
//
//}
