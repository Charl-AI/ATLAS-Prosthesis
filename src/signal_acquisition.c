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

void acquire_signal(int sensor, short rawData[], int length, int position)
{
}