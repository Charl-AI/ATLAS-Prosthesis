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
void ADC_Handler()
{
  bicepRaw[position % SAMPLES] = REG_ADC_RESULT;
  position++;
  ADC->INTFLAG.reg = ADC_INTENSET_RESRDY; //Need to reset interrupt
}
/******************************************************************************/
// setup function, runs once when device is booted

void setup()
{
  init_ADC();
  init_motor();
  Serial.begin(115200);
}

/******************************************************************************/
// main loop of the program
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
void loop()
{

  int bicepProcessed = processSignal(bicepRaw, SAMPLES, position);
  //int tricepProcessed = processSignal(tricepRaw, SAMPLES);

  /*
  bicepState = muscleStatus(bicepProcessed, bicepState, THRESHOLD);
  tricepState = muscleStatus(tricepProcessed, tricepState, THRESHOLD);
  pose = classifySignal(pose, bicepState, tricepState);
*/
  //select_pose(pose);

  print_to_plotter(bicepProcessed);
  //print_to_plotter(bicepRaw[position % SAMPLES]);
}

/******************************************************************************/
