/*
This header file defines the functions found in signal_acquisition.c
These functions are for using the ADC to sample the data on a timer interrupt
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef signal_acquisition_H
#define signal_acquisition_H

// define function prototypes
void init_ADC(void);
void acquire_signal(int sensor, short rawData[], int length, int position);

#ifdef __cplusplus
}
#endif

#endif