//
//  Path.h
//  CSE 6730 Project 1
//
//  Created by Karl Gemayel on 2/27/14.
//  Copyright (c) 2014 Karl Gemayel. All rights reserved.
//

#ifndef __CSE_6730_Project_1__Path__
#define __CSE_6730_Project_1__Path__

#include <iostream>
#include <map>

using namespace std;

#define W_T 0
#define W_B 1
#define NW_L 2
#define NW_R 3
#define NE_L 4
#define NE_R 5
#define E_T 6
#define E_B 7


class Path {
    
    map<int, double> paths; // (pathIndex, pathLength) pairs
    
public:
    
    /**
     * Constructor: Initialize path object.
     */
    Path();
    
    /**
     * Get shortest path length.
     *
     * @return the shortest path length.
     */
    double getShortestPath() const;
    
    /**
     * Get index of shortest path.
     *
     * @return the index of the shortest path.
     */
    int getShortestPathIndex() const;
    
    /**
     * Get the path's length.
     *
     * @param pathIndex the path's index
     * @return the path's length.
     */
    double getPathLength(int pathIndex) const;
    
    /**
     * Sets the length of the path.
     *
     * @param pathIndex the path's index
     * @param pathLength the path's length
     */
    void setPathLength(int pathIndex, double pathLength);
    
    /**
     * Get all paths.
     *
     * @return all path-index and path-length pairs.
     */
    const map<int,double> getPaths();
    
};

#endif /* defined(__CSE_6730_Project_1__Path__) */
