//
//  Cell.cpp
//  CSE 6730 Project 1
//
//  Created by Karl Gemayel on 2/6/14.
//  Copyright (c) 2014 Karl Gemayel. All rights reserved.
//

#include <assert.h>
#include "Cell.h"


// constructor
Cell::Cell(int i, int j) {
    this->i = i;
    this->j = j;
    
    shortestPaths = new Path();
//    shortestPath[W_T] = INT_MAX;
//    shortestPath[W_B] = INT_MAX;
//    shortestPath[NW_L] = INT_MAX;
//    shortestPath[NW_R] = INT_MAX;
//    shortestPath[NE_L] = INT_MAX;
//    shortestPath[NE_R] = INT_MAX;
//    shortestPath[E_T] = INT_MAX;
//    shortestPath[E_B] = INT_MAX;
    
    occupied = false;       // not occupied
    target = false;         // not target
    exit = false;           // not exit
    trace = new Trace();
    signal = NULL;
    pedestrian = NULL;      // since not occupied
}

// destructor
Cell::~Cell() {
    delete trace;
    delete shortestPaths;
    if (signal != NULL)
        delete signal;
}


// Cell becomes occupied
void Cell::enter(Pedestrian *p, bool fromBuilding) {
    assert(occupied == false && pedestrian == NULL);      // sanity check
    occupied = true;
    pedestrian = p;
    
    if (!fromBuilding)
        //        pedestrian->moved();        // indicate that pedestrian has moved (resets waiting time)
        pedestrian->moved(signal);
}


// Cell becomes unoccupied
Pedestrian* Cell::leave() {
    assert(occupied == true && pedestrian != NULL);       // sanity check
    occupied = false;
    Pedestrian* tmp = pedestrian;
    pedestrian = NULL;
    return tmp;
}


bool Cell::passesRedLight(int newI, int newJ) const {
    assert(isOccupied());       // sanity check
    
    // if no signal, return false
    if (signal == NULL)
        return false;
    
    if (!signal->isRed())
        return false;
    
    if (signal->getDirection() == NORTH) {
        if (newI < this->i)
            return true;
    }
    else if (signal->getDirection() == SOUTH) {
        if (newI > this->i)
            return true;
    }
    else if (signal->getDirection() == EAST) {
        if (newJ > this->j)
            return true;
    }
    else if (signal->getDirection() == WEST) {
        if (newJ < this->j)
            return true;
    }
    else {
        cout << "Direction unknown" << endl;
    }
    
    return false;
    
    
}


// updates cell properties by one time-step
void Cell::update() {
    // update trace and pedestrian one time-step
    trace->update();
    
    if (pedestrian != NULL)
        pedestrian->update();
    
    // update traffic signal (if any)
    if (signal != NULL)
        signal->update();
    
    if (signal != NULL && isOccupied()) {
        if (!pedestrian->waitingForSignal()) {
            pedestrian->changePath(signal->remainingRed(), shortestPaths->getPaths(), false);
        }
    }
    else if (isOccupied()) {
        if (pedestrian->needsToMove()) {
            pedestrian->changePath(10, shortestPaths->getPaths(), false);
        }
    }
}




#pragma mark -
#pragma mark Setters

void Cell::setTrafficSignal(TrafficSignal *ts) {
    signal = ts;
}

// Set the length of a particular path to a target starting from this cell.
void Cell::setPathLength(int targetIndex, double pathLength) {
//    shortestPath[targetIndex] = pathLength;
    shortestPaths->setPathLength(targetIndex, pathLength);
}

void Cell::setCurrentPath(int path) {
    currPath = path;
}

#pragma mark -
#pragma mark Check State

// Returns true if cell is occupied; false otherwise
bool Cell::isOccupied() const {
    return occupied;
}


// Returns true if cell has traffic signal; false otherwise.
bool Cell::hasTrafficLight() const {
    return signal != NULL;
}


#pragma mark -
#pragma mark Getters

// Get vertical index i
int Cell::getI() const { return i; }

// Get horizontal index j
int Cell::getJ() const { return j; }

// Computes the score of the cell
double Cell::getScore() const {
//    return shortestPath.at(pedestrian->getPath()) * (1.0/trace->getValue());
    return shortestPaths->getPathLength(pedestrian->getPath()) * (1.0/trace->getValue());
}

// Computes score of the cell for a particular target
double Cell::getScore(int targetIndex) const {
    return shortestPaths->getPathLength(targetIndex) * (1.0/trace->getValue());
}


//Get the pointer to the pedestrian. If cell is unoccupied, then NULL is returned.
const Pedestrian* Cell::getPedestrian() const {
    return pedestrian;
}

// Get the shortest path from this cell to the target specified by the pedestrian;
double Cell::getPedestrianShortestPath() const{
    assert(isOccupied());
//    return shortestPath.at(pedestrian->getPath());
    return shortestPaths->getPathLength(pedestrian->getPath());
}

// Get the length of the shortest path from this cell to the nearest target.
double Cell::getShortestPath() const {
    return shortestPaths->getShortestPath();
//    double minPath = INT_MAX;
//    for (std::map<int,double>::const_iterator it = shortestPath.begin(); it != shortestPath.end(); it++) {
//        if (it->second < minPath)
//            minPath = it->second;
//    }
//    
//    return minPath;
}


// Get the length of the shortest path from this cell to the target.
double Cell::getShortestPath(int targetIndex) const {
    return shortestPaths->getPathLength(targetIndex);
}


double Cell::getShortestPathBasedState() const {
    return shortestPaths->getPathLength(currPath);
}


int Cell::getShortestPathIndex() const {
    return shortestPaths->getShortestPathIndex();
}



//////////////////////// delete
////void Cell::updateFloorField() {
//    trace->update();
//}
//
//
//
//
//// returns true if pedestrian should stop because of red traffic signal (if any)
////bool Cell::shouldStop(int newI, int newJ) {
//    if (signal == NULL)
//        return false;
//    
//    // if signal is green, return false
//    //    if (!signal->isRed())
//    if (!passesRedLight(newI, newJ))
//        return false;
//    
//    // Signal is now red. Check to see if pedestrian wants to change path.
//    // If pedestrian is done waiting for the signal to change, then change
//    // path to next best.
//    if (!pedestrian->waitingForSignal()) {
//        pedestrian->changePath(signal->remainingRed(), shortestPath, false);
//    }
//    else if (pedestrian->needsToMove()) {
//        pedestrian->changePath(10, shortestPath, false);
//    }
//    
//    
//    return true;
//}


