//
//  Pedestrian.cpp
//  CSE 6730 Project 1
//
//  Created by Karl Gemayel on 2/12/14.
//  Copyright (c) 2014 Karl Gemayel. All rights reserved.
//

#include "Pedestrian.h"
#include <assert.h>
#include "RNG.h"

// create pedestrian with random speed
Pedestrian::Pedestrian() {
//    speed = generateRandomSpeed();
    speedWait = speed;
}

// create pedestrian with specific speed
Pedestrian::Pedestrian(int speed) {
    assert(speed > 0);       // sanity check
    this->speed = speed;
    speedWait = speed;
}

Pedestrian::Pedestrian(RNG* rng, const map<int,double> shortestPaths) {
    speed = generateRandomSpeed(rng); // FIXME: remove
    speedWait = speed;
    
    standingStillWait = MAX_STILL_TIME;
    signalID = -1;
    
    signalWait = 0;
    
    pathWaitRequirement[W_T] = 0;
    pathWaitRequirement[W_B] = 0;
    pathWaitRequirement[NW_L] = 0;
    pathWaitRequirement[NW_R] = 0;
    pathWaitRequirement[NE_L] = 0;
    pathWaitRequirement[NE_R] = 0;
    pathWaitRequirement[E_T] = 0;
    pathWaitRequirement[E_B] = 0;
    
    // find the shortest path and set pedestrian to it
    int minPathIndex = -1;
    double minPathLength = INT_MAX;
    for (map<int,double>::const_iterator it = shortestPaths.begin(); it != shortestPaths.end(); it++) {
        if (it->second < minPathLength) {
            minPathLength = it->second;
            minPathIndex = it->first;
        }
    }
    
    path = minPathIndex;
}


/**
 * Constructor that initializes a pedestrian.
 *
 * @param rng a random number generator
 * @param targetIndex index of pedestrian's initial target
 */
Pedestrian::Pedestrian(RNG* rng, int targetIndex) {
    speed = generateRandomSpeed(rng); // FIXME: remove
    speedWait = speed;
    
    standingStillWait = MAX_STILL_TIME;
    signalID = -1;
    
    signalWait = 0;
    
    pathWaitRequirement[W_T] = 0;
    pathWaitRequirement[W_B] = 0;
    pathWaitRequirement[NW_L] = 0;
    pathWaitRequirement[NW_R] = 0;
    pathWaitRequirement[NE_L] = 0;
    pathWaitRequirement[NE_R] = 0;
    pathWaitRequirement[E_T] = 0;
    pathWaitRequirement[E_B] = 0;
    
    path = targetIndex;

}


// Update the remaining time to wait before moving by one time-step.
void Pedestrian::update() {
    
    // decrement all non-zero path-related waiting times by one
    map<int,int>::iterator it = pathWaitRequirement.begin();
    while (it != pathWaitRequirement.end()) {
        
        // if waiting time has reached zero, skip it
        if (it->second != 0) {
            it->second -= 1;
        }
        
        it++;
    }
    
    // decrement traffic signal related wait time
    signalWait = max(signalWait-1,0);      // decrement wait but keep >= 0
    
    // decrement speed related wait
    speedWait = max(speedWait-1,0);         // decrement wait but keep >= 0
    
    // decrement standing still wait
    standingStillWait = max(standingStillWait-1,0);     // decrement wait by keep >= 0
}

// Checks whether pedestrian waited at least appropriate time before moving.
bool Pedestrian::canMove() const {
    return speedWait == 0;
}


// Indicates that person has moved by reseting waiting time. REMOVE
void Pedestrian::moved() {
    assert(speedWait == 0);  // sanity check - make sure pedestrian waited approriately
    speedWait = speed;
}


void Pedestrian::moved(const TrafficSignal* signal) {
    assert(speedWait == 0);
    speedWait = speed;           // reset speed related wait
    standingStillWait = MAX_STILL_TIME;
    
    // if pedestrian just moved to traffic signal area, decide how much time will be spent
    // waiting for traffic light to change green.
    if (signal != NULL) {
        if (signal->isRed() && signalID != signal->getID()) {
            signalWait = 400; //generateExponential();
            signalID = signal->getID();
        }
        else
            signalID = signal->getID();
    }
    else {
        // if cell does not contain signal
        signalID = -1;
    }
}


int Pedestrian::generateRandomSpeed(RNG* rand) {
    double r = rand->generateUniform();
    
    int randSpeed;
    
    // choose random speed based on rules (see report)
    if (r < 0.03)
        randSpeed = 1;
    else if (r < 0.14)
        randSpeed = 2;
    else if (r < 0.36)
        randSpeed = 3;
    else if (r < 0.64)
        randSpeed = 4;
    else if (r < 0.86)
        randSpeed = 5;
    else if (r < 0.97)
        randSpeed = 6;
    else
        randSpeed = 7;
        
    return randSpeed;
}


int Pedestrian::getSpeed() const{
    return speed;
}



void Pedestrian::changePath(int maxTime, map<int,double> shortestPaths, bool resetStillWaitTime) {
    
    if (resetStillWaitTime)
        standingStillWait = MAX_STILL_TIME;
    
    // if current path has waiting time of zero, set the corresponding waiting time
    map<int,int>::iterator it = pathWaitRequirement.find(path);
    if (it->second == 0)
        it->second = maxTime;
    
    // find the index of the shortest path with waiting time 0. If none exist,
    // then stay on current path
    int minPathIndex = -1;
    double minPathLength = INT_MAX;
    
    for (map<int,double>::iterator sp_it = shortestPaths.begin(); sp_it != shortestPaths.end(); sp_it++) {
        if (pathWaitRequirement[sp_it->first] == 0 && sp_it->second < minPathLength) {
            minPathLength = sp_it->second;
            minPathIndex = sp_it->first;
        }
    }
    
    // if a new path has been chosen, set it as current path
    if (minPathIndex != -1)
        path = minPathIndex;
    
    // if min has not been found (i.e. all path's have a waiting time larger than 0), stay on current path
    
}


int Pedestrian::getPath() const {
    return path;
}


// returns true if pedestrian is waiting for signal
bool Pedestrian::waitingForSignal() const {
    return signalWait > 0;
}

// returns true if pedestrian cannot bear standing still any longer
bool Pedestrian::needsToMove() const {
    return standingStillWait == 0;
}














