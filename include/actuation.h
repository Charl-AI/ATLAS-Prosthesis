/*
This header file defines the functions found in actuation.c
These functions are used for moving the hand between grip poses
*/

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef actuation_H
#define	actuation_H

// define function prototypes
void init_motor(void);
void extend_finger(int finger);
void close_finger(int finger);
void select_pose(int pose);

#ifdef	__cplusplus
}
#endif

#endif	