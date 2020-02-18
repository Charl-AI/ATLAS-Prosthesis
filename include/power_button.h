/*
Header file for power button source file
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef power_button_H
#define power_button_H

void wakeUpRoutine(void);
void checkButtonState(void);
void displayBatteryLevel(void);
void putToSleep(void);
void buttonHandler(void);

#ifdef __cplusplus
}
#endif

#endif
