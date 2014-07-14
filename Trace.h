//
//  Trace.h
//  CSE 6730 Project 1
//
//  Created by Karl Gemayel on 2/8/14.
//  Copyright (c) 2014 Karl Gemayel. All rights reserved.
//

#ifndef __CSE_6730_Project_1__Trace__
#define __CSE_6730_Project_1__Trace__

#include <iostream>

#define DECAY_TIME 10


class Trace {
    
    double trace;           // trace value
    int timeToDecay;        // time remaining before decay occurs
    void decay();           // decay the trace value
    
public:
    
    /**
     * Constructor that initializes the trace.
     */
    Trace();
    
    /**
     * Update the trace by one time-step closer towards decay.
     */
    void update();
    
    /**
     * Increment the trace value by one.
     */
    void increment();
    
    /**
     * Get trace value.
     *
     * @return Returns the current trace value.
     */
    double getValue();
    
};

#endif /* defined(__CSE_6730_Project_1__Trace__) */
