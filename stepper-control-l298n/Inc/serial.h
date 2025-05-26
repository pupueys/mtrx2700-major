#ifndef SERIAL_H
#define SERIAL_H

// INCLUDES
#include "timer.h"
#include "motors.h"
#include "stm32f303xc.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// DEFINITIONS
#define FORWARD 1
#define BACKWARD 0

// Defining the serial port struct
typedef struct {

	// setup parameters
	USART_TypeDef *UART;
	GPIO_TypeDef *GPIO;
	volatile uint32_t MaskAPB2ENR;	// mask to enable RCC APB2 bus registers
	volatile uint32_t MaskAPB1ENR;	// mask to enable RCC APB1 bus registers
	volatile uint32_t MaskAHBENR;	// mask to enable RCC AHB bus registers
	volatile uint32_t SerialPinModeValue;
	volatile uint32_t SerialPinSpeedValue;
	volatile uint32_t SerialPinAlternatePinValueLow;
	volatile uint32_t SerialPinAlternatePinValueHigh;
	volatile uint32_t UART_IRQn;

} SerialPort;

// make any number of instances of the serial port (they are extern because
//   they are fixed, unique values)
extern SerialPort USART1_PORT;


// The user might want to select the baud rate
enum {
  BAUD_9600,
  BAUD_19200,
  BAUD_38400,
  BAUD_57600,
  BAUD_115200
};

/* Initialises the specified serial port and the receiving serial interrupts*/
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port);
/*  Handler for the USART1 interrupts
 *  Activates rx_function if interrupt is triggered
 	Activates tx_function when transmit interrupt is triggered */
void USART1_EXTI25_IRQHandler(void);

/* This function is called when the RXNE interrupt is triggered.
	 * If the buffer is filled, the double buffers will switch, with the second buffer
	 * being able to receive, while the other buffer can be used */
void handle_key(uint8_t pressed_key);

uint8_t get_last_key(void);

#endif
