#include "peripherals.h"
// Enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;
}

// Initialize the discovery board I/O (outputs and analog inputs)
void initialise_board(void) {
	// Configure LEDs (PE8-PE15) as outputs
	    uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	    *led_output_registers = 0x5555;

    // Configure PC1, PC2, and PC3 as analog inputs for voltage readings
    // Mode 11 = Analog mode for each pin
    GPIOC->MODER |= GPIO_MODER_MODER1_0 | GPIO_MODER_MODER1_1;  // PC1 (Channel 7)
    GPIOC->MODER |= GPIO_MODER_MODER2_0 | GPIO_MODER_MODER2_1;  // PC2 (Channel 8)
    GPIOC->MODER |= GPIO_MODER_MODER3_0 | GPIO_MODER_MODER3_1;  // PC3 (Channel 9)

    // Configure PA0 to be digital output
    GPIOA->MODER &= ~(0b11 << (1 * 0));
    GPIOA->MODER |= (0b01 << 1 * 2);
    // Configure PC6 to be digital output
    GPIOC->MODER &= ~(0b11 << (6 * 2));
    GPIOC->MODER |= (0b01 << 6 * 2);


}

/******************************************************************************
 * ADC Functions
 ******************************************************************************/

// Configure the ADC for operation
void ConfigureADC(void) {
    // Enable the clock for ADC1
    RCC->AHBENR |= RCC_AHBENR_ADC12EN;

    // Set to synchronize the ADC with the clock
    ADC12_COMMON->CCR |= ADC12_CCR_CKMODE_0;

    // ADEN must be = 0 for configuration (is the default)
    ADC1->CR &= ~ADC_CR_ADVREGEN;    // Clear voltage regulator enable
    ADC1->CR |= ADC_CR_ADVREGEN_0;   // Set ADVREGEN TO 01
    ADC1->CR &= ~ADC_CR_ADCALDIF;    // Clear bit to enable Single-ended-input

    // Calibrate the ADC (self calibration routine)
    ADC1->CR |= ADC_CR_ADCAL;
    while((ADC1->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL); // Wait for calibration to finish

    // Enable ADC
    ADC1->CR |= ADC_CR_ADEN;

    // Wait for ADC to be ready
    while (ADC1->ISR == 0);
}

// Read a single value from the specified ADC channel
uint16_t ReadADC(uint32_t channel) {
    // Configure the sequence register for the specified channel
    ADC1->SQR1 = 0;
    ADC1->SQR1 |= channel << ADC_SQR1_SQ1_Pos; // Set the request for specified channel

    // Request the process to start
    ADC1->CR |= ADC_CR_ADSTART;

    // Wait for the end of conversion
    while(!(ADC1->ISR & ADC_ISR_EOC));

    // Return the conversion result
    return ADC1->DR;
}

// Convert ADC value to millivolts
uint16_t ConvertADCToMillivolts(uint16_t adcValue) {
    return (adcValue * ADC_VREF_MV) / ADC_MAX_VALUE;
}

// Function to implement the sequential voltage game
void SequentialVoltageGameADC(void) {
    // Configure the ADC
    ConfigureADC();

    uint16_t adcValue1 = 0;
    uint16_t adcValue2 = 0;
    uint16_t adcValue3 = 0;
    uint8_t correct_count = 0;
    uint8_t led1_state = 0;
    uint8_t led2_state = 0;

    /* Loop forever */
    for(;;) {
        // Read values from all three potentiometers
        adcValue1 = ReadADC(VOLTAGE1_CHANNEL);
        adcValue2 = ReadADC(VOLTAGE2_CHANNEL);
        adcValue3 = ReadADC(VOLTAGE3_CHANNEL);

        // Check first potentiometer and control LED1
        // Check if within threshold range (with tolerance)
        if (adcValue1 >= (VOLTAGE1_THRESHOLD - TOLERANCE_ADC) &&
            adcValue1 <= (VOLTAGE1_THRESHOLD + TOLERANCE_ADC)) {
            // Turn on LED1
            LED_PORT->BSRR = (1 << LED1_PIN);
            led1_state = 1;
        } else {
            // Turn off LED1
            LED_PORT->BSRR = (1 << (LED1_PIN + 16));
            led1_state = 0;
        }

        // Check second potentiometer and control LED2
        if (adcValue2 >= (VOLTAGE2_THRESHOLD - TOLERANCE_ADC) &&
            adcValue2 <= (VOLTAGE2_THRESHOLD + TOLERANCE_ADC)) {
            // Turn on LED2
            LED_PORT->BSRR = (1 << LED2_PIN);
            led2_state = 1;
        } else {
            // Turn off LED2
            LED_PORT->BSRR = (1 << (LED2_PIN + 16));
            led2_state = 0;
        }

        // Count how many of the first two LEDs are correct
        correct_count = led1_state + led2_state;

        // Only check the third potentiometer if the first two are correct
        if (correct_count >= 2) {
            // Check third potentiometer and control LED3
            if (adcValue3 >= (VOLTAGE3_THRESHOLD - TOLERANCE_ADC) &&
                adcValue3 <= (VOLTAGE3_THRESHOLD + TOLERANCE_ADC)) {
                // Turn on LED3
                LED_PORT->BSRR = (1 << LED3_PIN);

                GPIOC->ODR |= (1 << 6);

            } else {
                // Turn off LED3
                LED_PORT->BSRR = (1 << (LED3_PIN + 16));
            }
        } else {
            // If fewer than 2 LEDs are correct, ensure LED3 is off
            LED_PORT->BSRR = (1 << (LED3_PIN + 16));
        }

        // Small delay to avoid too frequent readings
        for(volatile uint32_t i = 0; i < 10000; i++);
    }
}
