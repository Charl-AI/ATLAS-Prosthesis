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

#include "signal_classification.h"
#include "actuation.h"
#include "filters.h"
#include "debugging.h"
/******************************************************************************/
// Define Global variables here

// This is the number of samples averaged over, tune this for performance
const int SAMPLES = 500;

// This is the current position we are at in the raw array
int position = 0;

// Here we store the current pose and state of each muscle
int bicepState = 0;
int tricepState = 0;
int pose = 0;

// here we store the sum of the squares for processing
long bicep_sum_squares = 0;
long tricep_sum_squares = 0;

// Here we declare the global arrays to store the raw signals
short bicepFiltered[SAMPLES];
short tricepFiltered[SAMPLES];

// Parameters for the filters
const float low_cutoff_freq = 100.0; //high cutoff frequency in Hz
const float high_cutoff_freq = 5;    // low cutoff frequency in Hz
const float sampling_time = 0.001;   //Sampling time in seconds.
IIR::ORDER order = IIR::ORDER::OD4;  // 4th order filter

// Filters for Bicep measurements
Filter bicepLowPass(low_cutoff_freq, sampling_time, order);
Filter bicepHighPass(high_cutoff_freq, sampling_time, order, IIR::TYPE::HIGHPASS);

// Filters for Tricep measurements
Filter tricepLowPass(low_cutoff_freq, sampling_time, order);
Filter tricepHighPass(high_cutoff_freq, sampling_time, order, IIR::TYPE::HIGHPASS);

/******************************************************************************/

/******************************************************************************/
// setup function, runs once when device is booted
void setup()
{
  init_motor();
  init_debug();
}

/******************************************************************************/
// main loop of the program
void loop()
{

  int THRESHOLD = analogRead(A2) / 10;

  //unsigned long starttime = micros();

  // collect and filter the raw bicep data
  float bicepLP = bicepLowPass.filterIn(analogRead(bicepSensor));
  int newBicepFiltered = bicepHighPass.filterIn(bicepLP);

  // store old data temporarily and replace it with new data
  short oldBicepFiltered = bicepFiltered[position % SAMPLES];
  bicepFiltered[position % SAMPLES] = newBicepFiltered;

  // find the RMS of the filtered data
  bicep_sum_squares -= oldBicepFiltered * oldBicepFiltered;
  bicep_sum_squares += newBicepFiltered * newBicepFiltered;
  int bicepRMS = sqrt(bicep_sum_squares / SAMPLES);

  // determine the state of the muscle and store the old one
  int oldBicepState = bicepState;
  bicepState = muscleStatus(bicepRMS, oldBicepState, THRESHOLD);

  // collect and filter the raw tricep data
  float tricepLP = tricepLowPass.filterIn(analogRead(tricepSensor));
  int newTricepFiltered = tricepHighPass.filterIn(tricepLP);

  // store old data temporarily and replace it with new data
  short oldTricepFiltered = tricepFiltered[position % SAMPLES];
  tricepFiltered[position % SAMPLES] = newTricepFiltered;

  // find the RMS of the filtered data
  tricep_sum_squares -= oldTricepFiltered * oldTricepFiltered;
  tricep_sum_squares += newTricepFiltered * newTricepFiltered;
  int tricepRMS = sqrt(tricep_sum_squares / SAMPLES);

  // determine the state of the muscle and store the old one
  int oldTricepState = tricepState;
  tricepState = muscleStatus(tricepRMS, oldTricepState, THRESHOLD);

  // increment our position in the raw array
  position++;

  // check if there has been any change in muscle state
  if ((bicepState != oldBicepState) | (tricepState != oldTricepState))
  {
    // calculate new pose and actuate
    pose = classifySignal(pose, bicepState, tricepState);
    select_pose(pose);
  }

  print_to_plotter(bicepRMS, tricepRMS, THRESHOLD);
  //print_to_plotter(bicepRaw[position % SAMPLES]);
  //unsigned long finishtime = micros();
  /*if ((finishtime - starttime) < 1000)
    {
      digitalWrite(19, HIGH);
    }
    */
}

/******************************************************************************/
