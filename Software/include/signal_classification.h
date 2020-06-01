/*
This header file defines the functions found in signal_classification.c
These functions are used for classifying the processed signals into 
muscle states and grip poses
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef signal_classification_H
#define signal_classification_H

int muscleStatus(int processedSignal, int muscleState, int upperThreshold);
int classifySignal(int currentPose, int bicepState, int tricepState);

#ifdef __cplusplus
}
#endif

#endif