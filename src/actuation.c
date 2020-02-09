/*
This file contains the functions involved in moving the fingers. 
Remember to include the config file which defines which pins the fingers
are connected to 

02.02.20
 */
#include <Arduino.h>
#include "config.h"
#include "actuation.h"

// Here we initialise the output pins for the motors
void init_motor(void)
{
    pinMode(thumb, OUTPUT);
    pinMode(index_finger, OUTPUT);
    pinMode(second_finger, OUTPUT);
}

// This function takes the pin number of the finger as an argument and
// extends the motor connected to that pin
void extend_finger(int finger)
{
    for (int i = 0; i < 10; i++)
    {
        digitalWrite(finger, HIGH);
        delay(1);
        digitalWrite(finger, LOW);
        delay(1);
    }
}

// This function takes the pin number of the finger as an argument and
// retracts the motor connected to that pin
void close_finger(int finger)
{
    for (int i = 0; i < 10; i++)
    {
        digitalWrite(finger, HIGH);
        delay(2);
        digitalWrite(finger, LOW);
        delay(2);
    }
}

// this function takes the pose input and makes the hand make that pose
// 0 is the open pose, 1 is the closed pose and 2 is the point pose
void select_pose(int pose)
{
    // pose 0 represents the open pose
    if (pose == 0)
    {
        extend_finger(thumb);
        extend_finger(index_finger);
        extend_finger(second_finger);
    }

    // pose 1 represents the closed pose
    if (pose == 1)
    {
        close_finger(thumb);
        close_finger(index_finger);
        close_finger(second_finger);
    }

    // pose 2 represents the point pose
    if (pose == 2)
    {
        close_finger(thumb);
        extend_finger(index_finger);
        close_finger(second_finger);
    }
}