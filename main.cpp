//
//  main.cpp
//  CSE 6730 Project 1
//
//  Created by Karl Gemayel on 1/22/14.
//  Copyright (c) 2014 Karl Gemayel. All rights reserved.
//

#include <iostream>
#include "CellularAutomata.h"

int main(int argc, const char * argv[])
{
    CellularAutomata ca;
    
    int numOfSteps = ca.simulate();
    
    cout << "Number of time-steps: " << numOfSteps << endl;
}

