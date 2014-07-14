//
//  CellularAutomata.h
//  CSE 6730 Project 1
//
//  Created by Karl Gemayel on 1/22/14.
//  Copyright (c) 2014 Karl Gemayel. All rights reserved.
//

#ifndef __CSE_6730_Project_1__CellularAutomata__
#define __CSE_6730_Project_1__CellularAutomata__

#include <iostream>
#include <assert.h>
#include <vector>
#include "Cell.h"
//#include "TrafficSignal.h"
//#include "TargetLocation.h"
//#include "Direction.h"

using namespace std;

#define VERBOSE false

// Traffic Lights
#define TS_NW_RED 27
#define TS_NW_GREEN 69
#define TS_NE_RED 25
#define TS_NE_GREEN 60
#define TS_E_RED 38
#define TS_E_GREEN 35

#define SEC_PER_STEP 1.0


// Map Dimensions
#define L5TH 65         // left 5th street
#define M5TH 325        // middle 5th street
#define R5TH 80         // right 5th street

#define TSPRING 50//100     // top spring street
#define MSPRING 250     // middle spring street
#define BSPRING 85      // bottom spring street

#define ARM_WEST_M5TH 115

#define SIDEWALK 4          // width of sidewalk
#define ROAD_WIDTH 5        // width of the road that separates two sidewalks
#define SRS (SIDEWALK+ROAD_WIDTH+SIDEWALK)

/********************************* Map Area **********************************\
 *                                                                           *
 * In the below map representation, the different symbols represent the      *
 * following:                                                                *
 *  - Consecutive dots (i.e. '....' going both horizontally or vertically)   *
 *    represent sidewalks on which pedestrians walk.                         *
 *  - The space between the parallel lines represents roads. Pedestrians are *
 *    not allowed on the roads except where we have a 'T' or a '.' located   *
 *    in the middle of the road, which pedestrian crossings with and without *
 *    pedestrian crossing signals, respectively.                             *
 *  - The 'T' characters represent pedestrian signals.                       *
 *                                                                           *
 *****************************************************************************
 *                                                                           *
 *         .| |.                                               .| |.         *
 *         .| |. <-- T-Spring                                  .| |.         *
 *  ........| |.................................................| |.         *
 *  _______.|T|._______________________________________________.| |.         *
 *  _______T   T_______________________________________________T| |.         *
 *  ........|T|..................Middle 5th Street..............|T|.         *
 *         .| |.                                               .| |.         *
 *         .| |.                                               .| |.         *
 *         .| |.                                               .| |.         *
 *         .| |.                            West Peachtree --> .| |.         *
 *         .| |.                                               .| |.         *
 *         .| |.                                               .| |.         *
 *         .| |.                                               .| |.         *
 *         .| |.  <-- M-Spring                                 .| |........  *
 *         .| |.                                               .| |._______  *
 *         .| |.                                       ___     .|  T_______  *
 *         .| |.                                        |      .|T|........  *
 *         .| |.                                        |      .| |.  R-5th  *
 *         .| |.                                        |      .| |.         *
 *         .| |.                                  ARM_WEST_M5TH.| |.         *
 *         .| |.                                        |      .| |.         *
 *         .|.|.........................................|.......| |.         *
 *         .| |.________________________________________|______.| |.         *
 *         .|  ._______________________________________________.  |.         *
 *         .| |.....................ARMSTEAD....................| |.         *
 *         .| |. <-- B-Spring                                  .| |.         *
 *         .| |.                                               .| |.         *
 *                                                                           *
\*****************************************************************************/


class CellularAutomata {
    
public:
    /**
     * Constructor that initializes the Cellular Automata grid based on the map.
     */
    CellularAutomata ();
    
    // class destructor - clears out memory allocations
    ~CellularAutomata ();
    
    /**
     * Starts the simulation of an evacuation, and returns the total number of 
     * time-steps taken for all pedestrians to be successfully evacuated.
     *
     * @return number of time-steps take for the complete evacuation of all pedestrians.
     */
    int simulate();
    
    /**
     * Runs the simulation on a large subset of all permutations of the doors on the
     * three sides of the building, writing the results to a file.
     */
    void findBestConfiguration();
    
    /**
     * Sets the building exit doors on the three sides of the building according to 
     * a specific configuration described by the parameters.
     *
     * @param left door placement configuration for the left side of the building
     * @param middle door placement configuration for the middle of the building
     * @param right door placement configuration for the right side of the building
     */
    void setExitDoors(string left, string middle, string right);
    
    
    /****************************************************\
     *               Printing/Debugging                 *
    \****************************************************/
    
    // prints the grid of pedestrian locations
    void printOccupationGrid();
    
    // prints grid of cell scores
    void printGrid();
    
    // print
    void printIteration(FILE* f);
    void printSimulation(string fname);
    
    // creates a small grid for debugging
    void initSmallGrid();
    
    /**
     * Creates a csv file that describes the locations of the sidewalks and traffic lights
     * on the map. This file can then be loaded (by something like Matlab's imshow() function)
     * to create a visual representation of the map.
     */
    void printMatlabGrid();

    void runParticularConfigurations();
    
private:
    Cell ***grid;       // the 2D grid
    int cellSize;       // length of a side of the cell
    int width;          // grid width
    int height;         // grid height
    
    int numTargets;     // number of target cells
    Cell **targets;     // list of target cells
    int numExits;       // number of exits
    Cell **exits;       // list of building exits
    
    vector<Cell*> allValidCells;
    
    // setting up the cell
    void setSidewalks();
    
    int populationSize;             // total number of people
    int remaining;                  // remaining people to exit the building
    int evacuated;                  // number of people who have evacuated the area

    
    
    void update();                          // update model state
    void updateCell(Cell *);                // update cell
    void updateCellPedest(Cell *current);
    void initGrid();                        // initialize the grid
    void calculateShortestPaths();          // calculate shortest path from each cell to a target
    void dijkstra(Cell*, bool);             // calculate shortest path from source to all other cells
    bool isValidNeighbourPos(int i, int j); // checks if cell (i,j) is a valid cell
    void setUpTrafficSignals();             // sets up traffic signals
    void setTargets();                      // sets up the target locations
    void setExits();
    void reset() {
        remaining = populationSize;
        evacuated = 0;
    }
    
    void resetExits() {
        for (int i = 0; i < numExits; i++) {
            exits[i]->exit = false;
            exits[i] = NULL;
        }
    }
    
    void setLeftExit(string left, int &exitsPlaced);
    void setMiddleExit(string middle, int &exitsPlaced);
    void setRightExit(string right, int &exitsPlaced);
    
    RNG *speedRNG;

};

#endif /* defined(__CSE_6730_Project_1__CellularAutomata__) */
