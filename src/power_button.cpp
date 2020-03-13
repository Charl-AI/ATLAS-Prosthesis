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

// variable to decide whether the device is asleep or awake, 0 is on ,1 is sleep
int device_state = 0;

// function to poll the button to determine if it's been pressed and what type
// of press, then to call the necessary functions
// (horribly long and should be refactored in future)
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
            turnOffLED();
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
                    device_state = 1;
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
                    //  display level on 4 LEDS
                    displayBatteryLevel();
                }
            }
        }
    }
}

// Routine for when turned on from sleep mode
void wakeUpRoutine(void)
{
    if (device_state == 1)
    {
        // Blink LEDs three times
        for (int i = 0; i < 10; i++)
        {
            digitalWrite(LED1, HIGH);
            digitalWrite(LED2, HIGH);
            digitalWrite(LED3, HIGH);
            digitalWrite(LED4, HIGH);
            delay(50);
            digitalWrite(LED1, LOW);
            digitalWrite(LED2, LOW);
            digitalWrite(LED3, LOW);
            digitalWrite(LED4, LOW);
            delay(50);
        }

        //now, use relay
        for (int i = 0; i < 1; i++)
        {
            digitalWrite(relayhigh, HIGH);
            delay(500);
            digitalWrite(relayhigh, LOW);
            delay(500);
        }
        device_state = 0; // reset device state variable
    }
}

// Function to turn off relay and put the device into sleep mode
void putToSleep(void)
{
    openRelay();

    // Blink LEDs three times
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, HIGH);
        delay(50);
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        delay(50);
    }
    // now sleep
    LowPower.sleep();
}

void turnOffLED(void)
{
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
}

void init_UI(void)
{
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(powerButton, INPUT);

    pinMode(relayhigh, OUTPUT);
    pinMode(relaylow, OUTPUT);

    closeRelay();
}

void displayBatteryLevel(void)
{
    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2;
    measuredvbat *= 3.3;  // Multiply by 3.3V, reference voltage
    measuredvbat /= 1024; // convert to voltage
    // Battery has max voltage 4.2, nominal 3.7 and cut off 2.75
    //  display level on 4 LEDS
    if (measuredvbat > 2.75)
    {
        digitalWrite(LED1, HIGH);
    }
    if (measuredvbat > 3.1)
    {
        digitalWrite(LED2, HIGH);
    }
    if (measuredvbat > 3.5)
    {
        digitalWrite(LED3, HIGH);
    }
    if (measuredvbat > 3.8)
    {
        digitalWrite(LED4, HIGH);
    }
}

void openRelay(void)
{
    // put relay code here
    for (int i = 0; i < 1; i++)
    {
        digitalWrite(relaylow, HIGH);
        delay(500);
        digitalWrite(relaylow, LOW);
        delay(500);
    }
}

void closeRelay(void)
{
}