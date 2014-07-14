//
//  Trace.cpp
//  CSE 6730 Project 1
//
//  Created by Karl Gemayel on 2/8/14.
//  Copyright (c) 2014 Karl Gemayel. All rights reserved.
//

#include "Trace.h"

// constructor that initializes the trace
Trace::Trace() {
    trace = 1;                      // trace value starts at one
    timeToDecay = DECAY_TIME;       // number of timesteps before decay
}

// updates trace by one time-step
void Trace::update() {
    timeToDecay -= 1;           // decrement "timer" by one
    
    if (timeToDecay == 0) {       // if time is up, decay trace and reset timer
        decay();
        timeToDecay = DECAY_TIME;           // reset decay timer
    }
}

// increments trace value by one
void Trace::increment() {
    trace++;
}

// decays trace value
void Trace::decay() {
    trace = std::max(trace/2.0, 1.0);  // reduce trace value by half (to a minimum of one)
}


// returns the current trace value
double Trace::getValue() {
    return trace;
}