//
//  TrafficSignal.h
//  CSE 6730 Project 1
//
//  Created by Chu Han on 2/8/14.
//  Copyright (c) 2014 Chu Han. All rights reserved.
//

#ifndef __CSE_6730_Project_1__TrafficSignal__
#define __CSE_6730_Project_1__TrafficSignal__

#include <assert.h>
#include "Direction.h"

class TrafficSignal {
public:
    
    /**
     * Constructor that sets up the traffic signal and randomly initializes it
     * to red or green.
     *
     * @param redSec length of red signal in seconds.
     * @param greenSec length of green signal in seconds.
     * @param secPerStep number of seconds per time-step.
     */
    TrafficSignal(int redSec, int greenSec, double secPerStep, int direction);
    
    /**
     * Update the traffic signal by one time-step.
     */
    void update();
    
    /**
     * Check whether traffic signal is red.
     *
     * @return true if signal is red; false otherwise.
     */
    bool isRed() const;
    
    
    /**
     * Get the remaining number of time-steps that the signal will remain red.
     *
     * @return the remaining time-steps before light turns green. If light is green, returns 0.
     */
    int remainingRed() const;
    
    
    /**
     * Set the traffic signal to the complement of itself (i.e. swaps the red and green signals),
     * and sets a new direction corresponding to the complement.
     *
     * @param newDirection the new direction
     */
    void complement(int newDirection);
    
    /**
     * Get the signal's direction.
     *
     * @return the signal's direction.
     */
    int getDirection() const;
    
    
    int getID() const {
        return myID;
    }
    
    
private:
    
    static int _id;     // a static class id
    
    int myID;           // unique id for traffic signal object
    int direction;      // direction of light
    int red;            // number of time-steps red lasts
    int green;          // number of time-steps green lasts
    int period;         // length of a single cycle/period (i.e. red+green)
    int currentT;       // current time-step within the period
};

#endif /* defined(__CSE_6730_Project_1__TrafficSignal__) */
