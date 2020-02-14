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
/******************************************************************************/
// Define Global variables here

// These are the parameters to be tuned for performance
const int SAMPLES = 100;
const int THRESHOLD = 100;

// Here we declare the global arrays to store the raw signals
short bicepFiltered[SAMPLES];
short tricepFiltered[SAMPLES];

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

void mySetup()
{
  //init_ADC();
  init_motor();

  // initialise serial port for debugging and development
  Serial.begin(115200);
}

/******************************************************************************/
// print to plotter for debugging and development
void print_to_plotter(int data)
{
  Serial.print(0); // To freeze the lower limit
  Serial.print(" ");
  Serial.print(70); // To freeze the upper limit
  Serial.print(" ");
  //Serial.print(threshold); // To mark the threshold voltage
  Serial.print(" ");
  Serial.println(data);
}
// main loop of the program
int main()
{
  init();
  mySetup();

  // This is the current position we are at in the raw array
  int position = 0;
  // Here we store the current pose and state of each muscle
  int bicepState = 0;
  int tricepState = 0;
  int pose = 0;

  float bicep_sum_squares = 0;
  float tricep_sum_squares = 0;

  while (1)
  {
    // collect and filter the raw bicep data
    float bicepLP = bicepLowPass.filterIn(analogRead(bicepSensor));
    float newBicepFiltered = bicepHighPass.filterIn(bicepLP);

    // store old data temporarily and replace it with new data
    short oldBicepFiltered = bicepFiltered[position % SAMPLES];
    bicepFiltered[position % SAMPLES] = newBicepFiltered;

    // find the RMS of the filtered data
    bicep_sum_squares -= oldBicepFiltered * oldBicepFiltered;
    bicep_sum_squares += newBicepFiltered * newBicepFiltered;
    int bicepRMS = sqrt(bicep_sum_squares / SAMPLES);
    // determine the state of the muscle
    bicepState = muscleStatus(bicepRMS, bicepState, THRESHOLD);

    /*
    // collect and filter the raw tricep data
    float tricepLP = tricepLowPass.filterIn(analogRead(tricepSensor));
    float newTricepFiltered = tricepHighPass.filterIn(tricepLP);

    // store old data temporarily and replace it with new data
    short oldTricepFiltered = tricepFiltered[position % SAMPLES];
    tricepFiltered[position % SAMPLES] = newTricepFiltered;

    // find the RMS of the filtered data
    tricep_sum_squares -= oldTricepFiltered * oldTricepFiltered;
    tricep_sum_squares += newTricepFiltered * newTricepFiltered;
    int tricepRMS = sqrt(tricep_sum_squares / SAMPLES);
    // determine the state of the muscle
    tricepState = muscleStatus(tricepRMS, tricepState, THRESHOLD);
*/
    // increment our position in the raw array
    position++;

    // store the previous pose
    int oldPose = pose;
    // determine the new pose
    pose = classifySignal(oldPose, bicepState, tricepState);

    /*
    // actuate if necessary
    if (pose != oldPose)
    {
      select_pose(pose);
    }
*/
    print_to_plotter(bicepRMS);
    //print_to_plotter(bicepRaw[position % SAMPLES]);
  }
}

/******************************************************************************/
