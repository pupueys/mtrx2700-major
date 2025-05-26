#include "game_logic.h"

// instantiate the serial port parameters
//   note: the complexity is hidden in the c file
SerialPort USART1_PORT = {USART1,
		GPIOC,
		RCC_APB2ENR_USART1EN, 	// bit to enable for APB2 bus
		0x00,					// bit to enable for APB1 bus
		RCC_AHBENR_GPIOCEN, 	// bit to enable for AHB bus
		0xA00,					// bits to set alternate function
		0xF00,					// bits to set high speed mode
		0x770000,  				// for USART1 PC10 and 11, this is in the AFR low register
		0x00, 					// no change to the high alternate function register
		0x00,					// tx char NULL
		USART1_IRQn,			// IRQn for USART1
		0x00,
		0x00,
		0x00
};

// Initialising the piano struct
Steinway piano = {
    .keys = {
        { TSC_GROUP1_IDX,  1700, CKEY, 0 },  // C
        { TSC_GROUP3_IDX, 1700, DKEY, 0 },  // D
        { TSC_GROUP5_IDX,  750, EKEY, 0 },  // E
        { TSC_GROUP6_IDX, 1700, FKEY, 0 },  // F
        { TSC_GROUP8_IDX, 1700, GKEY, 0 },  // G
    },

	// initial state
    .key_states = { 0 },
    .sequence = { 0 },
    .touch_count = 0,
	.key_pressed = 0,
	.key_clear = 0,
	.key_valid = 1		// initially, a key is ready to be read
};


void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef* htsc){

	// discharge the TSC
	HAL_TSC_IODischarge(htsc, ENABLE);

	// check the touch sensing channels
	for (int i = 0; i < 5; i++) {
		if (HAL_TSC_GroupGetStatus(htsc, piano.keys[i].group) == TSC_GROUP_COMPLETED) {
			uint16_t val = (uint16_t)HAL_TSC_GroupGetValue(htsc, piano.keys[i].group);
			key_press_logic(val, &piano, i, piano.keys[i].threshold, &USART1_PORT); // C
		}
	}

	// error if interrupts not initialised
	if (HAL_TSC_Start_IT(htsc) != HAL_OK) {
		Error_Handler();
	}

}


void key_press_logic(uint16_t sample_value, Steinway *piano, uint8_t key, uint16_t threshold, SerialPort *serial_port) {

	if (sample_value < threshold) {				// checks if the value is under the threshold for "pressed"
												// if pressed, sample_value is below the threshold

		if (piano->key_valid) {
			if (!piano->key_pressed) {				// case when key pressed

			  piano->key_states[key] = 1;
			  piano->key_pressed = 1;
			  piano->key_clear = 1;

			  set_leds(key);
			  enable_timer2_interrupt();
			  enable_timer3_interrupt();

			  switch(key) {

			  case CKEY:
				  tx_char(0x43, serial_port, piano, key);
				  break;

			  case DKEY:
				  tx_char(0x44, serial_port, piano, key);
				  break;

			  case EKEY:
				  tx_char(0x45, serial_port, piano, key);
				  break;

			  case FKEY:
				  tx_char(0x46, serial_port, piano, key);
				  break;

			  case GKEY:
				  tx_char(0x47, serial_port, piano, key);
				  break;
			  }
			}

		 else {
			 piano->key_states[key] = 0;		// key not pressed
		}
	}

  }
}

void gameplay_logic(Steinway* piano, uint8_t key) {

	uint8_t concurrent_keys = 0;

	// the correct sequence of keys (ode to joy)
	uint8_t correct_sequence[MAX_SEQ_COUNT] = {EKEY, EKEY, FKEY, GKEY, GKEY, FKEY, EKEY, DKEY, CKEY, CKEY,
								DKEY, EKEY, EKEY, DKEY, DKEY, EKEY, EKEY, FKEY, GKEY, GKEY, FKEY, EKEY, DKEY,
								CKEY, CKEY, DKEY, EKEY, DKEY, CKEY, CKEY};

	// for convenience
	uint8_t j = piano->touch_count;

	if (piano->key_valid && piano->key_pressed) {

		// flag the key as pressed
		piano->key_states[key] = 1;

		// check how many keys have been pressed
		for (int i = 0; i < KEY_COUNT; i++) {
			if (piano->key_states[i] == 1) {
				concurrent_keys++;
			} else {
				continue;
			}
		}

		// if there is more than one key pressed at a time, reset the game
		if (concurrent_keys != 1) {
			memset(piano->key_states, 0, sizeof(piano->key_states));
			piano->touch_count = 0;
			piano->key_valid = 0;
			return;
		}
		// if there is exactly one key pressed
		else if (concurrent_keys == 1) {

			piano->sequence[j] = key;

			// increase count if the pressed key is correct
			if (piano->sequence[j] == correct_sequence[j]){
				piano->touch_count++;

			}
			// if wrong key, reset
			else {
				piano->touch_count = 0;
				piano->key_valid = 0;
				return;
			}
		}

		// if the song is played correctly, game is complete!
		if (piano->touch_count == MAX_SEQ_COUNT) {
			piano->touch_count = 0;
			completion_function();
		}

		piano->key_valid = 0;
	}

	return;
}

// InitialiseSerial - Initialise the serial port
// Input: baudRate is from an enumerated set
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port) {

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
		*baud_rate_config = 0x1388;  // 9600 at 8MHz
		break;
	case BAUD_19200:
		*baud_rate_config = 0x9C4;  // 19200 at 8MHz
		break;
	case BAUD_38400:
		*baud_rate_config = 0x4E2;  // 38400 at 8MHz
		break;
	case BAUD_57600:
		*baud_rate_config = 0x342;  // 57600 at 8MHz
		break;
	case BAUD_115200:
		*baud_rate_config = 0x1A1;  // 115200 at 8MHz
		break;
	}


	// enable serial port for tx and rx
	serial_port->UART->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

	__disable_irq(); // disable all interrupts while changing settings

	// enabling the interrupts
	serial_port->UART->CR3 |= USART_CR3_EIE;		// enabling the error interrupts

	// activating interrupts and setting priority
	NVIC_SetPriority(serial_port->UART_IRQn, 2);
	NVIC_EnableIRQ(serial_port->UART_IRQn);			//  enable UART interrupts

	__enable_irq(); // re-enable all interrupts

}

// USART1 interrupt handler
void USART1_IRQHandler(void) {
    // TXE: Ready to transmit a byte
    if ((USART1->ISR & USART_ISR_TXE) && (USART1->CR1 & USART_CR1_TXEIE)) {
        USART1->TDR = USART1_PORT.TxChar;

    	// Disable TXE interrupt, enable TC to detect completion
        USART1->CR1 &= ~USART_CR1_TXEIE;
        USART1->CR1 |= USART_CR1_TCIE;
    }

    // TC: Transmission complete
    if ((USART1->ISR & USART_ISR_TC) && (USART1->CR1 & USART_CR1_TCIE)) {
        USART1->ICR = USART_ICR_TCCF;  // Clear TC flag
        USART1->CR1 &= ~USART_CR1_TCIE;		// Disable TC interrupt

        if (USART1_PORT.TransmissionState) {
            USART1_PORT.TransmissionState = 0;
            if (USART1_PORT.Piano && USART1_PORT.Piano->key_valid) {
                gameplay_logic(USART1_PORT.Piano, USART1_PORT.Key);
            }
        }
    }
}

// Transmitting a single character
void tx_char(uint8_t character, SerialPort *serial_port, Steinway* piano, uint8_t key) {
    serial_port->TxChar = character;
    serial_port->Piano = piano;
    serial_port->Key = key;
    serial_port->TransmissionState = 1;

    __disable_irq();

    // Enable TXE interrupt (start writing when TDR is empty)
    serial_port->UART->CR1 |= USART_CR1_TXEIE;

    __enable_irq();
}


// when the game is complete, send completed game state to main board
void completion_function(void) {

	GPIOC->ODR |= (1 << 9);

	return;
}
