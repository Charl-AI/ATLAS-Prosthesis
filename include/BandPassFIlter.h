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

// Begin header file, BandPassFilter.h

#ifndef BANDPASSFILTER_H_ // Include guards
#define BANDPASSFILTER_H_

static const int BandPassFilter_numStages = 4;
static const int BandPassFilter_coefficientLength = 20;
extern short BandPassFilter_coefficients[20];

typedef struct
{
	short state[16];
	short output;
} BandPassFilterType;

typedef struct
{
	short *pInput;
	short *pOutput;
	short *pState;
	short *pCoefficients;
	short count;
} BandPassFilter_executionState;


BandPassFilterType *BandPassFilter_create( void );
void BandPassFilter_destroy( BandPassFilterType *pObject );
 void BandPassFilter_init( BandPassFilterType * pThis );
 void BandPassFilter_reset( BandPassFilterType * pThis );
#define BandPassFilter_writeInput( pThis, input )  \
	BandPassFilter_filterBlock( pThis, &(input), &(pThis)->output, 1 );

#define BandPassFilter_readOutput( pThis )  \
	(pThis)->output

 int BandPassFilter_filterBlock( BandPassFilterType * pThis, short * pInput, short * pOutput, unsigned int count );
#define BandPassFilter_outputToFloat( output )  \
	(( (1.0f/4096) * (output) ))

#define BandPassFilter_inputFromFloat( input )  \
	((short)(32768f * (input)))

 void BandPassFilter_filterBiquad_11_14_15( BandPassFilter_executionState * pExecState );
 void BandPassFilter_filterBiquad_12_14_15( BandPassFilter_executionState * pExecState );
 void BandPassFilter_filterBiquad_15_14_15( BandPassFilter_executionState * pExecState );
#endif // BANDPASSFILTER_H_
	
