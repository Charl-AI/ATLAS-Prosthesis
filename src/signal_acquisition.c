/*
This file contains all the functions necessary to acquire signal from the
two sensors

we can use the following formula to calculate the settings for the clock:
timer_freq = clock_freq /(prescaler*(CC0+1))
since we want a timer freq of 2 kHz and we have a clocl freq of 48 MHz,
we need a prescaler of 1024 and CC0 of 22

09.02.20
 */
#include <Arduino.h>
#include "signal_acquisition.h"
#include "config.h"

// Here we set timer TC4 to overflow every 0.5 ms and generate an interrupt
void init_timer(void)
{
    // Set up the generic clock (GCLK4) used to clock timers
    REG_GCLK_GENDIV = GCLK_GENDIV_DIV(1) | GCLK_GENDIV_ID(4);

    while (GCLK->STATUS.bit.SYNCBUSY)
        ; // Wait for synchronization

    REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |         // Set the duty cycle to 50/50
                       GCLK_GENCTRL_GENEN |       // Enable GCLK4
                       GCLK_GENCTRL_SRC_DFLL48M | // Set the 48MHz clock source
                       GCLK_GENCTRL_ID(4);        // Select GCLK4
    while (GCLK->STATUS.bit.SYNCBUSY)
        ; // Wait for synchronization

    // Feed GCLK4 to TC4 and TC5
    REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |     // Enable GCLK4 to TC4 and TC5
                       GCLK_CLKCTRL_GEN_GCLK4 | // Select GCLK4
                       GCLK_CLKCTRL_ID_TC4_TC5; // Feed the GCLK4 to TC4 and TC5
    while (GCLK->STATUS.bit.SYNCBUSY)
        ; // Wait for synchronization

    // Set the TC4 CC0 register as the TOP value in match frequency mode
    // This is calculated to be 22 to give a 2 kHz overflow rate
    REG_TC4_COUNT16_CC0 = 22;
    while (TC4->COUNT16.STATUS.bit.SYNCBUSY)
        ; // Wait for synchronization

    // Set Nested Vector Interrupt Controller priority for TC4 to 0 (highest)
    NVIC_SetPriority(TC4_IRQn, 0);
    // Connect TC4 to Nested Vector Interrupt Controller (NVIC)
    NVIC_EnableIRQ(TC4_IRQn);

    REG_TC4_INTFLAG |= TC_INTFLAG_OVF;  // Clear the interrupt flags
    REG_TC4_INTENSET = TC_INTENSET_OVF; // Enable TC4 interrupts

    // Set prescaler to 1024, 48MHz/1024 = 46.875kHz
    // and put the timer TC4 into match frequency (MFRQ) mode
    REG_TC4_CTRLA |= TC_CTRLA_PRESCALER_DIV1024 |
                     TC_CTRLA_WAVEGEN_MFRQ |
                     TC_CTRLA_ENABLE; // Enable TC4
    while (TC4->COUNT16.STATUS.bit.SYNCBUSY)
        ; // Wait for synchronization
}

void acquire_signal(int sensor, short rawData[], int length, int position)
{
}