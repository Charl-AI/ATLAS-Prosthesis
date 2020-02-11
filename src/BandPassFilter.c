/******************************* SOURCE LICENSE *********************************
Copyright (c) 2019 MicroModeler.

A non-exclusive, nontransferable, perpetual, royalty-free license is granted to the Licensee to 
use the following Information for academic, non-profit, or government-sponsored research purposes.
Use of the following Information under this License is restricted to NON-COMMERCIAL PURPOSES ONLY.
Commercial use of the following Information requires a separately executed written license agreement.

This Information is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

******************************* END OF LICENSE *********************************/

// A commercial license for MicroModeler DSP can be obtained at http://www.micromodeler.com/launch.jsp

#include "BandPassFilter.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

short BandPassFilter_coefficients[20] =
	{
		// Scaled for a 16x16:32 Direct form 2 IIR filter with:
		// Output shift = 15
		// Input shift = 11
		// Central shift = 14
		// Input  has a maximum value of 1, scaled by 2^15
		// Output has a maximum value of 1.1931770116522946, scaled by 2^14

		8492, 16984, 8492, 18330, -5516,   // b0 Q17(0.0648), b1 Q17(0.130), b2 Q17(0.0648), a1 Q14(1.12), a2 Q14(-0.337)
		8192, 16384, 8192, 22123, -10822,  // b0 Q17(0.0625), b1 Q17(0.125), b2 Q17(0.0625), a1 Q14(1.35), a2 Q14(-0.661)
		8192, -16384, 8192, 31762, -15397, // b0 Q13(1.00), b1 Q13(-2.00), b2 Q13(1.00), a1 Q14(1.94), a2 Q14(-0.940)
		8192, -16384, 8192, 32390, -16022  // b0 Q13(1.00), b1 Q13(-2.00), b2 Q13(1.00), a1 Q14(1.98), a2 Q14(-0.978)

};

BandPassFilterType *BandPassFilter_create(void)
{
	BandPassFilterType *result = (BandPassFilterType *)malloc(sizeof(BandPassFilterType)); // Allocate memory for the object
	BandPassFilter_init(result);														   // Initialize it
	return result;																		   // Return the result
}

void BandPassFilter_destroy(BandPassFilterType *pObject)
{
	free(pObject);
}

void BandPassFilter_init(BandPassFilterType *pThis)
{
	BandPassFilter_reset(pThis);
}

void BandPassFilter_reset(BandPassFilterType *pThis)
{
	memset(&pThis->state, 0, sizeof(pThis->state)); // Reset state to 0
	pThis->output = 0;								// Reset output
}

int BandPassFilter_filterBlock(BandPassFilterType *pThis, short *pInput, short *pOutput, unsigned int count)
{
	BandPassFilter_executionState executionState; // The executionState structure holds call data, minimizing stack reads and writes
	if (!count)
		return 0;												// If there are no input samples, return immediately
	executionState.pInput = pInput;								// Pointers to the input and output buffers that each call to filterBiquad() will use
	executionState.pOutput = pOutput;							// - pInput and pOutput can be equal, allowing reuse of the same memory.
	executionState.count = count;								// The number of samples to be processed
	executionState.pState = pThis->state;						// Pointer to the biquad's internal state and coefficients.
	executionState.pCoefficients = BandPassFilter_coefficients; // Each call to filterBiquad() will advance pState and pCoefficients to the next biquad

	// The 1st call to filter1_filterBiquad() reads from the caller supplied input buffer and writes to the output buffer.
	// The remaining calls to filterBiquad() recycle the same output buffer, so that multiple intermediate buffers are not required.

	BandPassFilter_filterBiquad_11_14_15(&executionState); // Run biquad #0
	executionState.pInput = executionState.pOutput;		   // The remaining biquads will now re-use the same output buffer.

	BandPassFilter_filterBiquad_12_14_15(&executionState); // Run biquad #1

	BandPassFilter_filterBiquad_15_14_15(&executionState); // Run biquad #2

	BandPassFilter_filterBiquad_15_14_15(&executionState); // Run biquad #3

	// At this point, the caller-supplied output buffer will contain the filtered samples and the input buffer will contain the unmodified input samples.
	return count; // Return the number of samples processed, the same as the number of input samples
}

void BandPassFilter_filterBiquad_11_14_15(BandPassFilter_executionState *pExecState)
{
	// Read state variables
	short w0, x0;
	short w1 = pExecState->pState[0];
	short w2 = pExecState->pState[1];

	// Read coefficients into work registers
	short b0 = *(pExecState->pCoefficients++);
	short b1 = *(pExecState->pCoefficients++);
	short b2 = *(pExecState->pCoefficients++);
	short a1 = *(pExecState->pCoefficients++);
	short a2 = *(pExecState->pCoefficients++);

	// Read source and target pointers
	short *pInput = pExecState->pInput;
	short *pOutput = pExecState->pOutput;
	short count = pExecState->count;
	long accumulator;

	// Loop for all samples in the input buffer
	while (count--)
	{
		// Read input sample
		x0 = *(pInput++);

		// Run feedback part of filter
		accumulator = (long)w2 * a2;
		accumulator += (long)w1 * a1;
		accumulator += (long)x0 << 11;

		w0 = accumulator >> 14;

		// Run feedforward part of filter
		accumulator = (long)w0 * b0;
		accumulator += (long)w1 * b1;
		accumulator += (long)w2 * b2;

		w2 = w1; // Shuffle history buffer
		w1 = w0;

		// Write output
		*(pOutput++) = accumulator >> 15;
	}

	// Write state variables
	*(pExecState->pState++) = w1;
	*(pExecState->pState++) = w2;
}

void BandPassFilter_filterBiquad_12_14_15(BandPassFilter_executionState *pExecState)
{
	// Read state variables
	short w0, x0;
	short w1 = pExecState->pState[0];
	short w2 = pExecState->pState[1];

	// Read coefficients into work registers
	short b0 = *(pExecState->pCoefficients++);
	short b1 = *(pExecState->pCoefficients++);
	short b2 = *(pExecState->pCoefficients++);
	short a1 = *(pExecState->pCoefficients++);
	short a2 = *(pExecState->pCoefficients++);

	// Read source and target pointers
	short *pInput = pExecState->pInput;
	short *pOutput = pExecState->pOutput;
	short count = pExecState->count;
	long accumulator;

	// Loop for all samples in the input buffer
	while (count--)
	{
		// Read input sample
		x0 = *(pInput++);

		// Run feedback part of filter
		accumulator = (long)w2 * a2;
		accumulator += (long)w1 * a1;
		accumulator += (long)x0 << 12;

		w0 = accumulator >> 14;

		// Run feedforward part of filter
		accumulator = (long)w0 * b0;
		accumulator += (long)w1 * b1;
		accumulator += (long)w2 * b2;

		w2 = w1; // Shuffle history buffer
		w1 = w0;

		// Write output
		*(pOutput++) = accumulator >> 15;
	}

	// Write state variables
	*(pExecState->pState++) = w1;
	*(pExecState->pState++) = w2;
}

void BandPassFilter_filterBiquad_15_14_15(BandPassFilter_executionState *pExecState)
{
	// Read state variables
	short w0, x0;
	short w1 = pExecState->pState[0];
	short w2 = pExecState->pState[1];

	// Read coefficients into work registers
	short b0 = *(pExecState->pCoefficients++);
	short b1 = *(pExecState->pCoefficients++);
	short b2 = *(pExecState->pCoefficients++);
	short a1 = *(pExecState->pCoefficients++);
	short a2 = *(pExecState->pCoefficients++);

	// Read source and target pointers
	short *pInput = pExecState->pInput;
	short *pOutput = pExecState->pOutput;
	short count = pExecState->count;
	long accumulator;

	// Loop for all samples in the input buffer
	while (count--)
	{
		// Read input sample
		x0 = *(pInput++);

		// Run feedback part of filter
		accumulator = (long)w2 * a2;
		accumulator += (long)w1 * a1;
		accumulator += (long)x0 << 15;

		w0 = accumulator >> 14;

		// Run feedforward part of filter
		accumulator = (long)w0 * b0;
		accumulator += (long)w1 * b1;
		accumulator += (long)w2 * b2;

		w2 = w1; // Shuffle history buffer
		w1 = w0;

		// Write output
		*(pOutput++) = accumulator >> 15;
	}

	// Write state variables
	*(pExecState->pState++) = w1;
	*(pExecState->pState++) = w2;
}
