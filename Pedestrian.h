//
//  Pedestrian.h
//  CSE 6730 Project 1
//
//  Created by Karl Gemayel on 2/12/14.
//  Copyright (c) 2014 Karl Gemayel. All rights reserved.
//

#ifndef __CSE_6730_Project_1__Pedestrian__
#define __CSE_6730_Project_1__Pedestrian__

#include <iostream>
#include <map>
#include "RNG.h"
#include "TrafficSignal.h"

// path indeces
//#define NW 0
//#define NE 1
//#define E 2
//#define W 3

#define W_T 0
#define W_B 1
#define NW_L 2
#define NW_R 3
#define NE_L 4
#define NE_R 5
#define E_T 6
#define E_B 7


#define MAX_STILL_TIME 10;      // time-steps willing to stand still


class Pedestrian {
    
private:
    int generateRandomSpeed(RNG* rand);  // randomly generates a speed based on some distribution
    
    int signalID;       // id's the traffic signal we are at
    int path;
    int speed;          // the speed of a pedestrian measured by number of time-steps per move
    int speedWait;      // number of remaining timesteps pedestrian needs to wait before moving
    int signalWait;         // number of remaining timesteps to wait at signal.
    int standingStillWait;  // number of timesteps pedestrian is willing to stand still before changing path.
    std::map<int, int> pathWaitRequirement;     // keeps record of time required to wait before being able to choose a certain path
    
public:
    
    /**
     * Constructor that initializes pedestrian with a randomly selected speed.
     */
    Pedestrian();
    
    /**
     * Constructor that initializes a pedestrian with a specific speed.
     *
     * @param speed the number of time-steps that pass per one move
     */
    Pedestrian(int speed);
    
    /**
     * Constructor that initializes a pedestrian.
     *
     * @param rng a random number generator
     * @param shortestPaths a list of shortest paths
     */
    Pedestrian(RNG* rng, const map<int,double> shortestPaths);
    
    /**
     * Constructor that initializes a pedestrian.
     *
     * @param rng a random number generator
     * @param targetIndex index of pedestrian's initial target
     */
    Pedestrian(RNG* rng, int targetIndex);
    
    /**
     * Update the properties by one time-step.
     */
    void update();
    
    /**
     * Check if the pedestrian can move based on speed.
     *
     * @return Returns true if pedestrian can move; false otherwise.
     */
    bool canMove() const;
    
    
    /**
     * Indicate that pedestrian has just moved.
     */
    void moved();
    
    /**
     * Indicate that pedestrian has moved, identifying whether new cell has
     * a traffic signal.
     */
    void moved(const TrafficSignal* signal);
    
    /**
     * Get the pedestrian's speed.
     *
     * @return pedestrian's speed.
     */
    int getSpeed() const;
    
    /**
     * Change the pedestrian's preferred path, making sure the pedestrian doesn't
     * return to the previous one until a certain amount of time has elapsed.
     *
     * @param waitPath time to wait before being able to return to original path.
     */
    void changePath(int time, const std::map<int,double> shortestPaths, bool resetStillWaitTime);
    
    /**
     * Gets the path which the pedestrian is on.
     *
     * @return the pedestrian's current path.
     */
    int getPath() const;
    
    // returns true if pedestrian is waiting for signal
    /**
     * Checks to see if the pedestrian is still waiting for signal to turn green.
     *
     * @return true if pedestrian is still waiting for signal to turn green; false otherwise.
     */
    bool waitingForSignal() const;
    
    // returns true if pedestrian cannot bear standing still any longer
    /**
     * Checks if the pedestrian feels the need to move (because they haven't moved
     * for a "long" time)
     *
     * @return true if pedestrian needs to move; false otherwise.
     */
    bool needsToMove() const;
};

#endif /* defined(__CSE_6730_Project_1__Pedestrian__) */
