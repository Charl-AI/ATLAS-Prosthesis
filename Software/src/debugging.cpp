/*
These functions are used for development and debugging, dont include
these in the final product
 */
#include <Arduino.h>
#include "debugging.h"

// use this function in setup to enable debugging LEDs
void init_debug(void)
{
    Serial.begin(115200);

    pinMode(19, OUTPUT);
    pinMode(17, OUTPUT);
    pinMode(18, OUTPUT);
}

// print to plotter for debugging and development
void print_to_plotter(int bicep, int tricep, int threshold)
{
    Serial.print(0); // To freeze the lower limit
    Serial.print(",");
    Serial.print(150); // To freeze the upper limit
    Serial.print(",");
    Serial.print(threshold); // To mark the threshold voltage
    Serial.print(",");
    Serial.print(bicep);
    Serial.print(",");
    Serial.println(tricep);
}