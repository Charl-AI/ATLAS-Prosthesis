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

void init_ADC(void)
{
    // Input pin for ADC Arduino A0/PA02
    REG_PORT_DIRCLR1 = PORT_PA00;

    // Enable multiplexing on PA02_AIN0 PA03/ADC_VREFA
    PORT->Group[0].PINCFG[2].bit.PMUXEN = 1;
    PORT->Group[0].PINCFG[3].bit.PMUXEN = 1;
    PORT->Group[0].PMUX[1].reg = PORT_PMUX_PMUXE_B | PORT_PMUX_PMUXO_B;

    // Enable the APBC clock for the ADC
    REG_PM_APBCMASK |= PM_APBCMASK_ADC;

    //This allows you to setup a div factor for the selected clock certain clocks allow certain division factors: Generic clock generators 3 - 8 8 division factor bits - DIV[7:0]
    GCLK->GENDIV.reg |= GCLK_GENDIV_ID(3) | GCLK_GENDIV_DIV(1);
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
        ;

    //configure the generator of the generic clock with 48MHz clock
    GCLK->GENCTRL.reg |= GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_ID(3); // GCLK_GENCTRL_DIVSEL don't need this, it makes divide based on power of two
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
        ;

    //enable clock, set gen clock number, and ID to where the clock goes (30 is ADC)
    GCLK->CLKCTRL.reg |= GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(3) | GCLK_CLKCTRL_ID(30);
    while (GCLK->STATUS.bit.SYNCBUSY)
        ;

    // Select reference
    REG_ADC_REFCTRL = ADC_REFCTRL_REFSEL_INTVCC1; //set vref for ADC to VCC

    // Average control 1 sample, no right-shift
    REG_ADC_AVGCTRL |= ADC_AVGCTRL_SAMPLENUM_1;

    // Sampling time, no extra sampling half clock-cycles
    REG_ADC_SAMPCTRL = ADC_SAMPCTRL_SAMPLEN(0);

    // Input control and input scan
    REG_ADC_INPUTCTRL |= ADC_INPUTCTRL_GAIN_1X | ADC_INPUTCTRL_MUXNEG_GND | ADC_INPUTCTRL_MUXPOS_PIN0;
    // Wait for synchronization
    while (REG_ADC_STATUS & ADC_STATUS_SYNCBUSY)
        ;

    ADC->CTRLB.reg |= ADC_CTRLB_RESSEL_8BIT | ADC_CTRLB_PRESCALER_DIV32 | ADC_CTRLB_FREERUN; //This is where you set the divide factor, note that the divide call has no effect until you change Arduino wire.c
    //Wait for synchronization
    while (REG_ADC_STATUS & ADC_STATUS_SYNCBUSY)
        ;

    ADC->WINCTRL.reg = ADC_WINCTRL_WINMODE_DISABLE; // Disable window monitor mode
    while (ADC->STATUS.bit.SYNCBUSY)
        ;

    ADC->EVCTRL.reg |= ADC_EVCTRL_STARTEI; //start ADC when event occurs
    while (ADC->STATUS.bit.SYNCBUSY)
        ;

    ADC->CTRLA.reg |= ADC_CTRLA_ENABLE; //set ADC to run in standby
    while (ADC->STATUS.bit.SYNCBUSY)
        ;

    ADC->INTENSET.reg |= ADC_INTENSET_RESRDY; // enable ADC ready interrupt
    while (ADC->STATUS.bit.SYNCBUSY)
        ;

    NVIC_EnableIRQ(ADC_IRQn);      // enable ADC interrupts
    NVIC_SetPriority(ADC_IRQn, 0); //set priority of the interrupt
}

void acquire_signal(int sensor, short rawData[], int length, int position)
{
}