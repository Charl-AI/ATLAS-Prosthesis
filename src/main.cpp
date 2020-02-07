/*
Software for group 19 ME3 DMT project
This project is to develop a low-cost, 3D printable MMG prosthetic hand,
ATLAS

02.02.20
*/

/******************************************************************************/
//include statements and boilerplate code

#include <Arduino.h>
#include "config.h"
#include "BandPassFilter.h"
#include "signal_acquisition.h"
#include "signal_classification.h"
#include "signal_processing.h"
#include "actuation.h"

/******************************************************************************/
// Define Global variables here
const int SAMPLES = 50;
short bicepRaw[SAMPLES];
short tricepRaw[SAMPLES];

/******************************************************************************/
// interrupt service routines

/******************************************************************************/
// setup function, runs once when device is booted

void setup()
{

  init_motor();
}

/******************************************************************************/
// main loop of the program

void loop()
{
  processSignal(bicepRaw, SAMPLES);
  processSignal(tricepRaw, SAMPLES);
}

/******************************************************************************/