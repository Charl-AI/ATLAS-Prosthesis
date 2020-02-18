/*
This file contains all the functions necessary for the device's button

Upon a single click, the button will show the battery level and upon a 
long click it will put the device into sleep mode

18.02.20
 */
#include <Arduino.h>
#include <ArduinoLowPower.h>
#include "power_button.h"
#include "config.h"

// function to poll the button to determine if it's been pressed and what type
// of press
void checkButtonState(void)
{
}

// Routine for when turned on from sleep mode
void wakeUpRoutine(void)
{
}

// function to display the battery level on the LEDs
void displayBatteryLevel(void)
{
}

// Function to turn off relay and put the device into sleep mode
void putToSleep(void)
{
}

// call this in the main function to link all the above functions together
void buttonHandler(void)
{
}