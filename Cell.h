//
//  Cell.h
//  CSE 6730 Project 1
//
//  Created by Karl Gemayel on 2/6/14.
//  Copyright (c) 2014 Karl Gemayel. All rights reserved.
//

#ifndef __CSE_6730_Project_1__Cell__
#define __CSE_6730_Project_1__Cell__

#include <iostream>
#include <map>

#include "TrafficSignal.h"
#include "Pedestrian.h"
#include "Trace.h"
#include "Path.h"

#ifndef INT_MAX
    #include <limits.h>
#endif

// path indeces
#ifndef W_T
#define W_T 0
#define W_B 1
#define NW_L 2
#define NW_R 3
#define NE_L 4
#define NE_R 5
#define E_T 6
#define E_B 7
#endif

class Cell {
    
private:
    int i;          // location vertically on grid
    int j;          // location horizontally on grid
    
    bool occupied;  // indicates if cell is occupied
    
    int currPath;
    
    Path *shortestPaths;
    
    Trace *trace;               // floor field trace
    TrafficSignal *signal;      // traffic signal
    Pedestrian *pedestrian;     // pedestrian occupying current cell
    
    
public:
    
    bool target;    // is target?
    bool exit;      // is exit?
    bool valid;     // true if cell is valid
    
    
    Cell *previous;

    
    /**
     * Constructor that initializes the cell with its location, and sets its 
     * default parameters.
     *
     * @param i cell location on vertical axis
     * @param j cell location on horizontal axis
     */
    Cell(int i, int j);
    
    // Destructor
    ~Cell();
    
    
    /**
     * The pedestrian occupies the cell (if it is not already occupied).
     *
     * @param p a pointer to the pedestrian that is about to occupy the cell.
     * @param fromBuilding true if pedestrian came to this cell by exiting a building; false otherwise
     */
    void enter(Pedestrian *p, bool fromBuilding=false);  // occupant enters cell
    
    /**
     * The pedestrian already occupying the cell leaves.
     *
     * @return A pointer to the pedestrian who just left the cell.
     */
    Pedestrian* leave();        // occupant leaves cell
    
    
    /**
     * Update the cell and its constituents, such as traffic signal, floor field, pedestrian, etc...
     */
    void update();      // updates cell propertiers
    
    
    /**
     * Checks whether moving from the current cell to the cell at position (newI, newJ) involves
     * passing a red light.
     *
     * @param newI vertical coordinate of new cell
     * @param newJ horizontal coordinate of new cell
     * @return true if a red light is passed when moving to the new cell; false otherwise.
     */
    bool passesRedLight(int newI, int newJ) const;
    
    
    /****************************************************\
     *                      Getters                     *
    \****************************************************/
    
    /**
     * Computes the cell's score based on its distance to the nearest target and other properties.
     *
     * @return the cell's score.
     */
    double getScore() const;
    
    /**
     * Get the score of a particular path based on its distance to a particular and other properties.
     *
     * @param targetIndex the index to the specified target
     * @return the path's score
     */
    double getScore(int targetIndex) const;
    
    /**
     * Get the pedestrian occupying this cell.
     *
     * @return pointer to the pedestrian. If cell is unoccupied, then NULL is returned.
     */
    const Pedestrian* getPedestrian() const;
    
    
    /**
     * Get the length of the shortest path from this cell to the nearest target.
     *
     * @return the shortest path to nearest target.
     */
    double getShortestPath() const;
    
    /**
     * Get the length of the shortest path from this cell to a specified target
     *
     * @param targetIndex the index of the target
     * @return the length of the shortest path
     */
    double getShortestPath(int targetIndex) const;
    
    /**
     * Get the shortest path from this cell to the target specified by the pedestrian.
     *
     * @pre Cell is occupied.
     * @return the shortest path to the pedestrian's target.
     */
    double getPedestrianShortestPath() const;
    
    /**
     * Get the length of the shortest path from this cell to the target specified by the
     * cell's current state.
     *
     * @return the shortest path to target.
     */
    double getShortestPathBasedState() const;
    
    int getShortestPathIndex() const;
    
    
    /**
     * Get the cell's vertical index on the grid.
     *
     * @return cell's vertical index.
     */
    int getI() const;
    
    /**
     * Get the cell's horizontal index on the grid.
     *
     * @return cell's horizontal index.
     */
    int getJ() const;
    

    
    
    
    /****************************************************\
     *                    Check State                   *
    \****************************************************/
    
    /**
     * Checks whether current cell has a traffic light.
     *
     * @return true if cell has traffic signal; false otherwise.
     */
    bool hasTrafficLight() const;
    
    /**
     * Checks whether the cell is occupied with a pedestrian.
     *
     * @return Returns true if cell is occupied.
     */
    bool isOccupied() const;          // returns true if cell is occupied
    
    
    /****************************************************\
     *                      Setters                     *
    \****************************************************/
    
    /**
     * Sets a traffic signal to the cell.
     *
     * @param ts the traffic signal
     */
    void setTrafficSignal(TrafficSignal *ts);
    
    
    /**
     * Set the length of a particular path starting from this cell.
     *
     * @param path the path
     * @param pathLength the length of the path
     */
    void setPathLength(int path, double pathLength);
    
    /**
     * Set's the cell's current target.
     *
     * @param path index of target
     */
    void setCurrentPath(int path);
};


#endif /* defined(__CSE_6730_Project_1__Cell__) */
