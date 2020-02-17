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

void init_debug(void);
void print_to_plotter(int bicep, int tricep, int threshold);

#ifdef __cplusplus
}
#endif

#endif
