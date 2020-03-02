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
// of press, then to call the necessary functions
void buttonHandler(void)
{
    // stores button state: 0 is up, 1 is down
    static int button_state;
    // stores whether battery level is being displayed (0 or 1)
    static int display_state;
    // stores time when button was first pressed down
    static unsigned long pressed_time;
    // stores time when battery level was first displayed
    static unsigned long display_time;

    // first check if we are currently showing the battery level
    if (display_state == 1)
    {
        // if battery level has been shown for more than 1 second
        if (millis() - display_time >= 1000)
        {
            // Turn off lights
            // Set display state back to zero
            display_state = 0;
        }
    }
    else
    {
        // if button not previously pressed down
        if (button_state == 0)
        {
            // check if button has been pressed
            if (digitalRead(powerButton) == HIGH)
            {
                // set time for when button was pressed and set state to 1
                pressed_time = millis();
                button_state = 1;
            }
        }
        // if button has been previously pressed
        if (button_state == 1)
        {
            // check if button has been released yet
            if (digitalRead(powerButton) == LOW)
            {
                // if long press of button (over 1s)
                if (millis() - pressed_time >= 1000)
                {
                    // set state back to zero and put to sleep
                    button_state = 0;
                    putToSleep();
                }
                // if short press of button (< 1s)
                else
                {
                    // set state back to zero and display battery level
                    button_state = 0;
                    display_state = 1;
                    display_time = millis();

                    float measuredvbat = analogRead(VBATPIN);
                    measuredvbat *= 2;
                    measuredvbat *= 3.3;  // Multiply by 3.3V, reference voltage
                    measuredvbat /= 1024; // convert to voltage
                    // Battery has max voltage 4.2, nominal 3.7 and cut off 2.75
                    //
                }
            }
        }
    }
}

// Routine for when turned on from sleep mode
void wakeUpRoutine(void)
{
    // put relay code here

    // put LED code here
}

// Function to turn off relay and put the device into sleep mode
void putToSleep(void)
{
    // put relay code here

    // put LED code here

    LowPower.sleep();
}
