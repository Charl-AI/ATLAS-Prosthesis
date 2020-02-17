/*
This file contains the signal classification functions

muscleStatus is a Schmitt trigger that decides whether the muscle is relaxed
or contracted

classifySignal uses a state transition table to decide which pose to select


09.02.20
 */

#include "signal_classification.h"

// Schmitt trigger to classify muscle as either active or relaxed
int muscleStatus(int processedSignal, int muscleState, int upperThreshold)
{
    int lowerThreshold = 0.5 * upperThreshold;

    // if the muscle was relaxed
    if (muscleState == 0)
    {
        if (processedSignal >= upperThreshold)
        {
            muscleState = 1;
        }
    }
    else
    {
        if (processedSignal <= lowerThreshold)
        {
            muscleState = 0;
        }
    }
    return muscleState;
}

// Takes the status of both muscles and classifies it into a pose
int classifySignal(int currentPose, int bicepState, int tricepState)
{
    // The first index is for the current pose
    // The second index is for the bicepState
    // The third index is for the tricepState
    int stateTable[3][2][2];

    // current pose is open
    stateTable[0][0][0] = 0; // no change, so stay open
    stateTable[0][0][1] = 2; // tricep, so point
    stateTable[0][1][0] = 1; // bicep, so grip
    stateTable[0][1][1] = 0; // both active, so dont change

    // current pose is grip
    stateTable[1][0][0] = 1; // no chage, so stay grip
    stateTable[1][0][1] = 2; // tricep, so point
    stateTable[1][1][0] = 0; // bicep, so return to open
    stateTable[1][1][1] = 1; // both active, so dont change

    // current pose is point
    stateTable[2][0][0] = 2; // no change, so stay grip
    stateTable[2][0][1] = 0; // tricep, so return to open
    stateTable[2][1][0] = 1; // bicep, so grip
    stateTable[2][1][1] = 2; //both active, so dont change

    return stateTable[currentPose][bicepState][tricepState];
}