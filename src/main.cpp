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

// These are the parameters to be tuned for performance
const int SAMPLES = 50;
const int THRESHOLD = 100;

// Here we declare the global arrays to store the raw signals
short bicepRaw[SAMPLES];
short tricepRaw[SAMPLES];

// This is the current position we are at in the raw array
int position = 0;
// This is the sensor that will be read from next
int sensor = bicepSensor;

// Here we store the current pose and state of each muscle
int bicepState = 0;
int tricepState = 0;
int pose = 0;
/******************************************************************************/
// Interrupt Service Routine (ISR) for timer TC4
// This triggers every 0.5 ms and will cause the ADC to read from a sensor,
// the sensor being read will alternate each time
void TC4_Handler()
{
  // Check for overflow (OVF) interrupt
  if (TC4->COUNT16.INTFLAG.bit.OVF && TC4->COUNT16.INTENSET.bit.OVF)
  {
    if (sensor == bicepSensor)
    {
      acquire_signal(bicepSensor, bicepRaw, SAMPLES, position);
      sensor = tricepSensor;
    }
    if (sensor == tricepSensor)
    {
      acquire_signal(tricepSensor, tricepRaw, SAMPLES, position);
      sensor = bicepSensor;
      position += 1;
    }

    REG_TC4_INTFLAG = TC_INTFLAG_OVF; // Clear the OVF interrupt flag
  }
}

/******************************************************************************/
// setup function, runs once when device is booted

void setup()
{

  init_timer();
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
  pose = classifySignal(pose, bicepState, tricepState);

  select_pose(pose);
}

/******************************************************************************/