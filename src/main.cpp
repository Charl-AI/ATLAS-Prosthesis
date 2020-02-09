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
const int THRESHOLD = 100;
short bicepRaw[SAMPLES];
short tricepRaw[SAMPLES];

int bicepState = 0;
int tricepState = 0;
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
  int bicepProcessed = processSignal(bicepRaw, SAMPLES);
  int tricepProcessed = processSignal(tricepRaw, SAMPLES);

  bicepState = muscleStatus(bicepProcessed, bicepState, THRESHOLD);
  tricepState = muscleStatus(tricepProcessed, tricepState, THRESHOLD);
  int pose = classifySignal(pose, bicepProcessed, tricepProcessed);

  select_pose(pose);
}

/******************************************************************************/