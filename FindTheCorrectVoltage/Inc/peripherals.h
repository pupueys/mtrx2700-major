#ifndef PERIPHERALS_H
#define PERIPHERALS_H
#include <stdint.h>
#include "stm32f303xc.h"

// ADC Reference Voltage in millivolts (typically 3000mV for 3V systems)
#define ADC_VREF_MV         3000
#define ADC_MAX_VALUE       4095    // 12-bit ADC (2^12 - 1)

// Predefined voltage thresholds in millivolts
#define VOLTAGE1_MV         1000    // 1.0V threshold for first input
#define VOLTAGE2_MV         1500    // 1.5V threshold for second input
#define VOLTAGE3_MV         2000    // 2.0V threshold for third input

// Tolerance range in millivolts (how close the guess needs to be)
#define VOLTAGE_TOLERANCE_MV 100    // +/- 100mV tolerance

// Convert millivolts to ADC values
#define MV_TO_ADC(mv)       ((mv * ADC_MAX_VALUE) / ADC_VREF_MV)

// ADC threshold values calculated from millivolts
#define VOLTAGE1_THRESHOLD  MV_TO_ADC(VOLTAGE1_MV)
#define VOLTAGE2_THRESHOLD  MV_TO_ADC(VOLTAGE2_MV)
#define VOLTAGE3_THRESHOLD  MV_TO_ADC(VOLTAGE3_MV)
#define TOLERANCE_ADC       MV_TO_ADC(VOLTAGE_TOLERANCE_MV)

// ADC channel definitions
#define VOLTAGE1_CHANNEL    7     // First ADC channel (PC1)
#define VOLTAGE2_CHANNEL    8     // Second ADC channel (PC2)
#define VOLTAGE3_CHANNEL    9     // Third ADC channel (PC3)

// LED definitions (PE8, PE9, PE10 on STM32F303 Discovery board)
#define LED1_PIN            8     // PE8
#define LED2_PIN            9     // PE9
#define LED3_PIN            10    // PE10
#define LED_PORT            GPIOE

// Board initialization functions
void enable_clocks(void);
void initialise_board(void);

// ADC functions
void ConfigureADC(void);
uint16_t ReadADC(uint32_t channel);
uint16_t ConvertADCToMillivolts(uint16_t adcValue);
void SequentialVoltageGameADC(void);

#endif
