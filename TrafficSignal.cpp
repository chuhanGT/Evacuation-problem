//
//  TrafficSignal.cpp
//  CSE 6730 Project 1
//
//  Created by Chu Han on 2/5/14.
//  Copyright (c) 2014 Chu Han. All rights reserved.
//

#include "TrafficSignal.h"
#include "RNG.h"


int TrafficSignal::_id = 0;


// Constructor
TrafficSignal::TrafficSignal(int redSec, int greenSec, double secPerStep, int dir){
    red = int(redSec/secPerStep);           // convert red-seconds to red-timesteps
    green = int(greenSec/secPerStep);       // convert green-seconds to green-timesteps
    period = red+green;                     // compute length of red-green period
    
    this->direction = dir;
    
    // randomly initialize light to a specific point in red-green period
    RNG rand;
    currentT = int(rand.generateUniform()*period);
    
    myID = _id++;
}

// returns true if light is red; false otherwise
bool TrafficSignal::isRed() const {
    if (currentT < red)
        return true;
    
    return false;
}

// updates light signal by one time-step
void TrafficSignal::update() {
    currentT = (currentT+1) % period;   // incremet current time-step (looping over period if necessary)
}


int TrafficSignal::remainingRed() const {
    return std::max(0,red - currentT);
}

int TrafficSignal::getDirection() const {
    return direction;
}


void TrafficSignal::complement(int newDirection) {
    if (currentT < red) {
        currentT += red;
    }
    else
        currentT = green-(period-currentT);
    
    int temp = red;
    red = green;
    green = temp;
    
    
    this->direction = newDirection;
}



