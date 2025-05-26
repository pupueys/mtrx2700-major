#include "serial.h"

volatile uint8_t last_key = 0;

// USART1 struct
SerialPort USART1_PORT = {USART1,
		GPIOC,
		RCC_APB2ENR_USART1EN, 	// bit to enable for APB2 bus
		0x00,					// bit to enable for APB1 bus
		RCC_AHBENR_GPIOCEN, 	// bit to enable for AHB bus
		0xA00,					// bits to set alternate function
		0xF00,					// bits to set high speed mode
		0x770000,  				// for USART1 PC10 and 11, this is in the AFR low register
		0x00, 					// no change to the high alternate function register
		USART1_IRQn				// IRQn for USART1
};

void SerialInitialise(uint32_t baudRate,SerialPort *serial_port) {

	// enable clock power, system configuration clock and GPIOC
	// common to all UARTs
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// enable the GPIO which is on the AHB bus
	RCC->AHBENR |= serial_port->MaskAHBENR;

	// set pin mode to alternate function for the specific GPIO pins
	serial_port->GPIO->MODER = serial_port->SerialPinModeValue;

	// enable high speed clock for specific GPIO pins
	serial_port->GPIO->OSPEEDR = serial_port->SerialPinSpeedValue;

	// set alternate function to enable USART to external pins
	serial_port->GPIO->AFR[0] |= serial_port->SerialPinAlternatePinValueLow;
	serial_port->GPIO->AFR[1] |= serial_port->SerialPinAlternatePinValueHigh;

	// enable the device based on the bits defined in the serial port definition
	RCC->APB1ENR |= serial_port->MaskAPB1ENR;
	RCC->APB2ENR |= serial_port->MaskAPB2ENR;

	// Get a pointer to the 16 bits of the BRR register that we want to change
	uint16_t *baud_rate_config = (uint16_t*)&serial_port->UART->BRR; // only 16 bits used!

	// Baud rate calculation from datasheet
	switch(baudRate){
	case BAUD_9600:
		*baud_rate_config = 0x342;  // 9600 at 8MHz
		break;
	case BAUD_19200:
		*baud_rate_config = 0x19C;  // 19200 at 8MHz
		break;
	case BAUD_38400:
		*baud_rate_config = 0xD0;  // 38400 at 8MHz
		break;
	case BAUD_57600:
		*baud_rate_config = 0x8B;  // 57600 at 8MHz
		break;
	case BAUD_115200:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	}


	// enable serial port for tx and rx
	serial_port->UART->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

	__disable_irq(); // disable all interrupts while changing settings

	// enabling the interrupts
	serial_port->UART->CR1 |= USART_CR1_RXNEIE;		// enabling RXNE interrupts
	serial_port->UART->CR3 |= USART_CR3_EIE;		// enabling the error interrupts

	// activating interrupts and setting priority
	NVIC_SetPriority(serial_port->UART_IRQn, 1);
	NVIC_EnableIRQ(serial_port->UART_IRQn);			//  enable UART interrupts

	__enable_irq(); // re-enable all interrupts

}

void USART1_EXTI25_IRQHandler(void) {

    USART1->ICR |= USART_ICR_ORECF | USART_ICR_FECF | USART_ICR_NCF | USART_ICR_PECF;

    if (USART1->ISR & USART_ISR_RXNE) {

    	// read the key press
    	uint8_t pressed_key = USART1->RDR;
    	last_key = pressed_key;
        handle_key(pressed_key);

    }
}

void handle_key(uint8_t pressed_key) {

	uint32_t current_time = get_time();

	// logic for the key presses
	switch(pressed_key) {

		case 'w':

			motor1_enable(FORWARD);
			break;

		case 's':

			motor1_enable(BACKWARD);
			break;

		case 'd':

			motor2_enable(FORWARD);
			break;

		case 'a':

			motor2_enable(BACKWARD);
			break;

		case 'e':
			motor1_enable(FORWARD);
			motor2_enable(FORWARD);
			break;

		case 'q':
			motor1_enable(FORWARD);
			motor2_enable(BACKWARD);
			break;

		case 'z':
			motor1_enable(BACKWARD);
			motor2_enable(BACKWARD);

		case 'c':
			motor1_enable(BACKWARD);
			motor2_enable(FORWARD);

        default:
            // Ignore unrecognized keys
            break;

	}

	// record when a key was last pressed
	set_last_press_time(current_time);

}

uint8_t get_last_key(void){
	return last_key;
}
