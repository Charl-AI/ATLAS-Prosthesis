/*
This file allocates meaningful names to the pins on the board.
All functions in this project use these names, so, if we want to wire the 
board differently, we only need to change this file
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef config_H
#define config_H

// Power button pin
#define powerButton 5

// Battery voltage pin
#define VBATPIN A7

// Sensors are connected to analogue inputs 0 and 1
#define bicepSensor A0
#define tricepSensor A1
#define potentiometer A2

// motors for each finger connected to pins 10,11,12
#define thumb 10
#define index_finger 11
#define second_finger 12

// LED lights
#define LED1 1 // red LED for lowest battery level
#define LED2 0
#define LED3 22
#define LED4 23 // green LED for highest battery level

// Relay pins
#define relayhigh 21
#define relaylow 20

#ifdef __cplusplus
}
#endif

#endif
