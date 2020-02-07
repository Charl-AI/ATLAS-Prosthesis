/*
This file contains the functions necessary to process the raw signal
from the MMG sensors

07/02/20
 */

#include <math.h>
#include "signal_processing.h"
#include "BandPassFilter.h"

int processSignal(short rawData[], int length)
{
    // create filter and variable to store RMS
    BandPassFilterType *BandPassFilter = BandPassFilter_create();
    long RMS = 0;

    // iterate through raw data, filtering each value and adding the
    // filtered value's square to the running RMS
    for (int i = 0; i < length; ++i)
    {
        BandPassFilter_writeInput(BandPassFilter, rawData[i]);
        int filtered = BandPassFilter_readOutput(BandPassFilter);
        RMS += filtered * filtered;
    }

    BandPassFilter_destroy(BandPassFilter); // destroy the filter

    RMS /= length;
    return (sqrt(RMS));
}