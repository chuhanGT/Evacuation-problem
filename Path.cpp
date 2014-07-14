//
//  Path.cpp
//  CSE 6730 Project 1
//
//  Created by Karl Gemayel on 2/27/14.
//  Copyright (c) 2014 Karl Gemayel. All rights reserved.
//

#include "Path.h"
#include <assert.h>

/**
 * Constructor: Initialize path object.
 */
Path::Path() {
    
    paths[W_T] = INT_MAX;
    paths[W_B] = INT_MAX;
    paths[NW_L] = INT_MAX;
    paths[NW_R] = INT_MAX;
    paths[NE_L] = INT_MAX;
    paths[NE_R] = INT_MAX;
    paths[E_T] = INT_MAX;
    paths[E_B] = INT_MAX;
}


/**
 * Sets the length of the path.
 *
 * @param pathIndex the path's index
 */
void Path::setPathLength(int pathIndex, double pathLength) {
    paths[pathIndex] = pathLength;
}

/**
 * Get shortest path length.
 *
 * @return the shortest path length.
 */
double Path::getShortestPath() const {
    return paths.at(getShortestPathIndex());
}

/**
 * Get index of shortest path.
 *
 * @return the index of the shortest path.
 */
int Path::getShortestPathIndex() const {
    int minIndex = 0;
    int minPathLength = INT_MAX;
    
    for (std::map<int,double>::const_iterator it = paths.begin(); it != paths.end(); it++) {
        if (it->second < minPathLength) {
            minIndex = it->first;
            minPathLength = it->second;
        }
    }
    
    return minIndex;
}

/**
 * Get the path's length.
 *
 * @param pathIndex the path's index
 * @return the path's length.
 */
double Path::getPathLength(int pathIndex) const {
    map<int,double>::const_iterator it = paths.find(pathIndex);
    if (it == paths.end())
        cout << pathIndex << endl;
    assert(it != paths.end());          // sanity check
    return it->second;
    
}




/**
 * Get all paths.
 *
 * @return all path-index and path-length pairs.
 */
const map<int,double> Path::getPaths() {
    return paths;
}
