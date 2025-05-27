/*
  **********************************************************************************
 * 									serial.h										*
 * 			Contains all functions related to serial functionality, including 		*
 *									transmission.									*									*
 * 																					*
  **********************************************************************************
 */

#ifndef SERIAL_H
#define SERIAL_H

/*************/
/* INCLUDES */
/*************/
#include <digitalio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "stm32f303xc.h"

/***************/
/* DEFINITIONS */
/***************/
#define CKEY 0
#define DKEY 1
#define EKEY 2
#define FKEY 3
#define GKEY 4

#define KEY_COUNT 5
#define MAX_SEQ_COUNT 30

#define TSC_GROUP1_IDX 0
#define TSC_GROUP3_IDX 2
#define TSC_GROUP5_IDX 4
#define TSC_GROUP6_IDX 5
#define TSC_GROUP8_IDX 7

// Forward declaration of Steinway struct
typedef struct Steinway Steinway;
typedef struct PianoKey PianoKey;

typedef struct SerialPort {
    USART_TypeDef *UART;
    GPIO_TypeDef *GPIO;
    volatile uint32_t MaskAPB2ENR;
    volatile uint32_t MaskAPB1ENR;
    volatile uint32_t MaskAHBENR;
    volatile uint32_t SerialPinModeValue;
    volatile uint32_t SerialPinSpeedValue;
    volatile uint32_t SerialPinAlternatePinValueLow;
    volatile uint32_t SerialPinAlternatePinValueHigh;
    volatile uint8_t TxChar;
    volatile uint32_t UART_IRQn;
    volatile uint8_t  TransmissionState;

    struct Steinway* Piano;
    uint8_t Key;
} SerialPort;

extern SerialPort USART1_PORT;

enum {
    BAUD_9600,
    BAUD_19200,
    BAUD_38400,
    BAUD_57600,
    BAUD_115200
};

// SerialInitialise: Initialises serial ports
// inputs:
/* - baudRate: Baud rate for serial communication
 * - serial_port: Port for serial communication (USART1)
 */
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port);

// USART1_EXTI25_IRQHandler: Serial interrupt handler
void USART1_EXTI25_IRQHandler(void);

// tx_char: Outputs one ASCII character over serial
// inputs:
/* - character: ASCII character to be transmitted
 * - serial_port: Serial port to transmit over; USART1 for this program
 * - piano: Piano struct which stores the piano data
 * - key: Corresponding key to be transmitted
 */
void tx_char(uint8_t character, SerialPort *serial_port, struct Steinway* piano, uint8_t key);

#endif
