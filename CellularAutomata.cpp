//
//  CellularAutomata.cpp
//  CSE 6730 Project 1
//
//  Created by Karl Gemayel on 1/22/14.
//  Copyright (c) 2014 Karl Gemayel. All rights reserved.
//

#include "CellularAutomata.h"
#include <queue>
#include <assert.h>
#include <algorithm>
#include <map>
#include <math.h>

using namespace std;


/********************************************************\
            Constructors and Destructors
\********************************************************/

#pragma mark Constructor/Destructor

// Class constructor
CellularAutomata::CellularAutomata() {
    
    // initialize default values
    populationSize = 1500;
    remaining = populationSize;
    evacuated = 0;
    speedRNG = new RNG();           // init speed RNG
    
    // initialize grid
    initGrid();
}


// Class Destructor
CellularAutomata::~CellularAutomata() {
    // free the grid allocations
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            delete grid[i][j];
        }
        delete grid[i];
    }
    delete [] grid;
    
    // delete speed rng
    delete speedRNG;
}



#pragma mark -
#pragma mark Running the model

/********************************************************\
                    Running the Model
\********************************************************/

// Runs the simulation for the current configuration of building exits
int CellularAutomata::simulate() {
    int numOfSteps = 0;
    while (evacuated < populationSize) {
        update();
        numOfSteps++;
        
        if (numOfSteps > 15000)
            break;
    }
    
    return numOfSteps;
}


#pragma mark -
#pragma mark Permuting Exit Doors

/********************************************************\
                Permuting Exit Doors
\********************************************************/

// Splits a configuration string of door/side pair allocation into three strings, each
// representing one of the three sides. For example, if the doors are represented by
// one of three characters A, B, and C, and a barrier seperating two sides is represented
// by the pipe | character, then if
//                                 config = AB|C|ABA
// we get
//              left = AB       right = C       middle = ABA
void splitConfiguration(string config, string &left, string &middle, string & right) {
    left = ""; middle = ""; right = "";
    int idx = 0;        // index for side
    
    for (int i = 0; i < config.length(); i++) {
        if (config[i] == '|')
            idx += 1;
        else {
            if (idx == 0)
                left += config[i];
            else if (idx == 1)
                middle += config[i];
            else
                right += config[i];
        }
            
    }
}

// returns the length of the door A, B, or C
int getDoorLength(char door) {
    if (door == 'A')
        return 2;
    if (door == 'B')
        return 4;
    if (door == 'C')
        return 8;

    return 0;
}

// returns the length of symbols representing "free" locations on a building.
int getSymbolLength(char s) {
    if (s == '.')
        return 1;
    if (s == '-')
        return 5;
    if (s == '/')
        return 20;
    if (s == '>')
        return 50;
    if (s == ';')
        return 100;
    if (s == ':')
        return 200;
    
    return 0;
}

string getSideRepresentation(string doors, int sideLength, map<char, int> symbolCount) {
    string config = "";
    
    int remaining = sideLength;     // remaining number of points
    
    for (int i = 0; i < doors.length(); i++) {
        config += doors[i];
        remaining -= getDoorLength(doors[i]);
    }
    
    assert(remaining >= 0);
    
    // add the remaining empty cells
    for (map<char,int>::iterator it = symbolCount.begin(); it != symbolCount.end(); it++) {
        for (int i = 0; i < it->second; i++) {
            // make sure we're not going beyond side length
            if (remaining < getSymbolLength(it->first))
                break;
            
            config += it->first;
            remaining -= getSymbolLength(it->first);
        }
                
    }
    
    return config;
}

string getSideRepresentation(string doors, int sideLength) {
    string config = "";
    
    int remaining = sideLength;     // remaining number of points
    
    for (int i = 0; i < doors.length(); i++) {
        config += doors[i];
        remaining -= getDoorLength(doors[i]);
    }
    
    assert(remaining >= 0);
    
    // add the remaining empty cells
    while (remaining > 0) {
        config += '.';
        remaining--;
    }
    
    return config;
}


void CellularAutomata::setExitDoors(string left, string middle, string right) {
    // start by clearing all already existing exits
    resetExits();
//    for (int i = 0; i < numExits; i++) {
//        exits[i]->exit = false;
//        exits[i]
    
    
    
    // now add exit doors on their respective sides
    
    // add on left side
    int distance = 0;       // distance already covered
    for (int i = 0; i < left.length(); i++) {
        // count number of empty cells
        if (getSymbolLength(left[i]) > 0) {
            distance += getSymbolLength(left[i]);
            continue;
        }
        
        // otherwise, set door
        for (int j = 0; j < getDoorLength(left[i]); j++) {
            grid[height-distance-j-1][L5TH+SRS-1]->exit = true;
        }
        distance += getDoorLength(left[i]);     // account for space taken by door
    }
    
    // add on middle
    distance = 0;       // reset distance
    for (int i = 0; i < middle.length(); i++) {
        // count number of empty cells
        if (getSymbolLength(middle[i]) > 0) {
            distance += getSymbolLength(middle[i]);
            continue;
        }
        
        // otherwise, set door
        for (int j = 0; j < getDoorLength(middle[i]); j++) {
            grid[height-BSPRING-1][L5TH+SRS+distance+j]->exit = true;
        }
        
        distance += getDoorLength(middle[i]);     // account for space taken by door
    }
    
    // add on right side
    distance = 0;       // reset distance
    for (int i = 0; i < right.length(); i++) {
        // count number of empty cells
        if (getSymbolLength(right[i]) > 0) {
            distance += getSymbolLength(right[i]);
            continue;
        }
        
        // otherwise, set door
        for (int j = 0; j < getDoorLength(right[i]); j++) {
            grid[height-distance-j-1][width-R5TH-SRS]->exit = true;
        }
        
        distance += getDoorLength(right[i]);     // account for space taken by door
    }
}

int countNumOfDoors(string side) {
    int count = 0;
    for (int i = 0; i < side.length(); i++) {
        if (side[i] == 'A' || side[i] == 'B' || side[i] == 'C')
            count++;
    }
    return count;
}

int totalDoorLengthOnSide(string side) {
    int count = 0;
    for (int i = 0; i < side.length(); i++) {
        if (side[i] == 'A' || side[i] == 'B' || side[i] == 'C')
            count += getDoorLength(side[i]);
    }
    return count;
}


map<char,int> getLeftSymbols(int totalDoorsLength) {
    map<char,int> leftSymbols = {{'-',3}, {'>', 1}};
    switch (totalDoorsLength) {
        case 20:
            leftSymbols = {{'-',3}, {'>', 1}};
            break;
        case 18:
            leftSymbols = {{'-',3}, {'>', 1}};
            break;
        case 16:
            leftSymbols = {{'-',3}, {'>', 1}};
            break;
        case 14:
            leftSymbols = {{'/',1}, {'>', 1}};
            break;
        case 12:
            leftSymbols = {{'/',1}, {'>', 1}};
            break;
        case 10:
            leftSymbols = {{'-',1}, {'/',1}, {'>', 1}};
            break;
        case 8:
            leftSymbols = {{'-',1}, {'/',1}, {'>', 1}};
            break;
        case 6:
            leftSymbols = {{'-',1}, {'/',1}, {'>', 1}};
            break;
        default:
            leftSymbols = {{'/',3}, {'>', 1}};
            break;
    }
    
    return leftSymbols;
}


map<char,int> getRightSymbols(int totalDoorLengths) {
    return getLeftSymbols(totalDoorLengths);
}

map<char, int> getMiddleSymbols(int totalDoorsLength) {
    map<char,int> middleSymbols;
    switch (totalDoorsLength) {
        case 20:
            middleSymbols = {{'-',1}, {';', 1}, {':', 1}};
            break;
        case 18:
            middleSymbols = {{'-',1}, {';', 1}, {':', 1}};
            break;
        case 16:
            middleSymbols = {{'-',1}, {';', 1}, {':', 1}};
            break;
        case 14:
            middleSymbols = {{'-',2}, {';', 1}, {':', 1}};
            break;
        case 12:
            middleSymbols = {{'-',2}, {';', 1}, {':', 1}};
            break;
        case 10:
            middleSymbols = {{'-',3}, {';', 1}, {':', 1}};
            break;
        case 8:
            middleSymbols = {{'-',3}, {';', 1}, {':', 1}};
            break;
        case 6:
            middleSymbols = {{'-',3}, {';', 1}, {':', 1}};
            break;
        default:
            middleSymbols = {{'-',4}, {';', 1}, {':', 1}};
            break;
    }
    
    return middleSymbols;
}


void CellularAutomata::setLeftExit(string left, int& exitsPlaced) {
    
    int distance = 5;           // start 5 cells from top of building
    
    for (int i = 0; i < countNumOfDoors(left); i++) {
        char door = left[i];
        
        for (int j = 0; j < getDoorLength(door); j++) {
            exits[exitsPlaced] = grid[height-BSPRING-1+distance+j][L5TH+SRS-1];
            exits[exitsPlaced]->exit = true;
            exitsPlaced++;
        }
        
        distance+= getDoorLength(door) + 5;       // doors are separated by 5 cells
    }
}


void CellularAutomata::setRightExit(string right, int& exitsPlaced) {
    
    int distance = 5;           // start 5 cells from top of building
    
    for (int i = 0; i < countNumOfDoors(right); i++) {
        char door = right[i];
        
        for (int j = 0; j < getDoorLength(door); j++) {
            exits[exitsPlaced] = grid[height-BSPRING-1+distance+j][L5TH+SRS+M5TH];
            exits[exitsPlaced]->exit = true;
            exitsPlaced++;
        }
        
        distance+= getDoorLength(door) + 5;       // doors are separated by 5 cells
    }
}

void CellularAutomata::setMiddleExit(string middle, int& exitsPlaced) {
    
    int leftDistance = 5;           // start 5 cells from left of building
    int rightDistance = 5;          // start 5 cells from right of building
    bool left = true;       // start placing on left part
    
    for (int i = 0; i < countNumOfDoors(middle); i++) {
        char door = middle[i];
        
        if (left) {
            for (int j = 0; j < getDoorLength(door); j++) {
                exits[exitsPlaced] = grid[height-BSPRING-2][L5TH+SRS+leftDistance+j];
                exits[exitsPlaced]->exit = true;
                exitsPlaced++;
            }
        
            leftDistance+= getDoorLength(door) + 5;       // doors are separated by 5 cells
        }
        else {
            for (int j = 0; j < getDoorLength(door); j++) {
                exits[exitsPlaced] = grid[height-BSPRING-2][L5TH+SRS+M5TH-rightDistance-j];
                exits[exitsPlaced]->exit = true;
                exitsPlaced++;
            }
            
            rightDistance+= getDoorLength(door) + 5;       // doors are separated by 5 cells
        }
        
        left = !left;       // alternate sides
    }
}


void CellularAutomata::runParticularConfigurations() {
    
    size_t numOfConfigs = 5;
    string bestConfigurations[5];
    bestConfigurations[0] = "C|AAA|AB";
    bestConfigurations[1] = "C|ABA|AA";
    bestConfigurations[2] = "C|AAB|AA";
    bestConfigurations[3] = "CB|AAA|A";
    bestConfigurations[4] = "CA|BAA|A";
    
    
    int N = 50;
    int acc[N];
    int counter = 0;
    
    FILE* fstats = fopen("stats.out", "w");
    FILE* fconfigs = fopen("doorConfigs.out", "w");
    FILE* fraw = fopen("rawRuntimes.out", "w");
    
    for (int iter = 0; iter < numOfConfigs; iter++) {
        string currentConfig = bestConfigurations[iter];
        
        string left, middle, right;
        splitConfiguration(currentConfig, left, middle, right);
        
        
        resetExits();
        int exitsPlaced = 0;
        setLeftExit(left, exitsPlaced);
        setRightExit(right, exitsPlaced);
        setMiddleExit(middle, exitsPlaced);
        
        assert(exitsPlaced == numExits);
        
        double meanAcc = 0;
        int numOfTimeSteps = 0;
        // run N times, get avg and std. dev
        for (int i = 0; i < N; i++) {
            numOfTimeSteps = simulate();
            reset();
            meanAcc += numOfTimeSteps;
            acc[i] = numOfTimeSteps;
            
            cout << numOfTimeSteps << endl;
        }
        
        // calc mean
        double mean = meanAcc/((double)N);
        
        // calc std. dev
        double stdDev = 0;
        for (int i = 0; i < N; i++) {
            stdDev += pow((acc[i]-mean), 2);
        }
        
        stdDev = stdDev/(N-1);
        
        counter++;
        
        fprintf(fraw, "%d", counter);
        for (int i = 0; i < N; i++) {
            fprintf(fraw, ",%d", acc[i]);
        }
        fprintf(fraw, "\n");
        fflush(fraw);
        
        fprintf(fstats, "%d,%f,%f\n", counter, mean, stdDev);
        fflush(fstats);
        
        fprintf(fconfigs, "%d,%s,%s,%s\n", counter, left.c_str(), middle.c_str(), right.c_str());
        fflush(fconfigs);
    }
    
    fclose(fstats);
    fclose(fraw);
    fclose(fconfigs);
    
}

void CellularAutomata::findBestConfiguration() {
    /* Code Sketch:
     *
     * 1) Permute doors in one of three sets: left, middle, right. These
     *    sets represent the left, middle(top), and right sides of the
     *    building, respectively, onto which the doors will be placed.
     * 2) For each of these permutations, permute the doors on these
     *    three sides and run the simulation on each configuration.
     */
    
    // Let A, B, and C represent the doors of sizes small, medium, and large, respectively.
    // Moreover, let | represent a barrier, where two barriers will be needed to separate
    // the three sides: the first is a barrier between the left and middle parts, and the
    // second is a barrier between the middle and right parts.
    
    // We have four small doors, 1 medium, 1 large, and 2 barriers
    string sideConfigurations = "AAAABC||";
    
    // To perform the permutations without repeating similar configurations (where two
    // different 'A' occuring in the same position might be counted as different), we use
    // the <next_permutation> function in the C++ <algorithm> library. Since this function
    // returns the "next" permutation ordered lexicographically, we first sort our configuration
    // list and then run the next_permutation until all permutations have been reached. Note
    // that since the simultation runs of different door configurations are independent of
    // each other, generating the permutations in a sorted fashion does not incur any bias.
    
    sort(sideConfigurations.begin(), sideConfigurations.end());     // sort configuration lexicographically
    
    int counter = 0;
    int numOfTimeSteps = 0;
    int N = 1;
    int acc[N];     //
    string results = "";
    
    FILE* fstats = fopen("stats.out", "w");
    FILE* fconfigs = fopen("doorConfigs.out", "w");
    FILE* fraw = fopen("rawRuntimes.out", "w");
    
    // perform permutation to assign which doors go on which sides
    do {
        
        // Now that we have an assignement of doors for each side of the building, we execute
        // three nested loops of permutations: the ourtermost loop permutes the doors on the left
        // side of the building covering the entire left perimeter, the middle loop does the same
        // for the middle part of the building, and the innermost loop will cover the right side.
        // That way, we will have all possible permutations of doors on all sides of the building.
        
        // Start by getting the doors of each side.
        string leftDoors, middleDoors, rightDoors;
        splitConfiguration(sideConfigurations, leftDoors, middleDoors, rightDoors);
        
        // In order to permute the doors on one of the sides, we do the following. Assume that we
        // are permuting two doors, A and B, on left side of the building. Further assume that the
        // left side has length 10, and doors A and B have lengths 2 and 4, respectively. Now, we
        // represent a configuration as a string of length (#Side - #A+1 - #B+1) = (10 - 1 - 3) = 6,
        // where '.' characters represent locations without doors, and locations with doors are
        // represented by their respective doors (in this case, A or B). For example, if we placed
        // A 2 units from the right and B 6 units from the right, the corresponding string representation
        // will be: "..A.B." This representation allows us to easily permute the doors on any given side.
        
        //        map<char,int> leftSymbols = {{'-',3}, {'>', 1}};
        //        map<char,int> middleSymbols = {{'-',5}, {'/', 3}, {'>', 5}};
        //        map<char,int> rightSymbols = {{'-',3}, {'>', 1}};
        
        int leftDoorsLength = totalDoorLengthOnSide(leftDoors);
        int middleDoorsLength= totalDoorLengthOnSide(middleDoors);
        int rightDoorsLength = totalDoorLengthOnSide(rightDoors);
        
        map<char,int> leftSymbols = getLeftSymbols(leftDoorsLength);
        map<char,int> middleSymbols = getMiddleSymbols(middleDoorsLength);
        map<char,int> rightSymbols = getRightSymbols(rightDoorsLength);
        
        
        string left = getSideRepresentation(leftDoors, BSPRING, leftSymbols);
        int leftDoorCount = countNumOfDoors(left);
        
        string middle = getSideRepresentation(middleDoors, M5TH, middleSymbols);
        int middleDoorCount = countNumOfDoors(middle);
        
        string right = getSideRepresentation(rightDoors, BSPRING, rightSymbols);
        int rightDoorCount = countNumOfDoors(right);
        
        // make sure we have at max 2 doors on left or right sides, and max 4 in the middle
        if (leftDoorCount > 2 || rightDoorCount > 2 || middleDoorCount > 3 || rightDoorCount == 0 || leftDoorCount == 0)
            continue;
        
        
        //        cout << left << endl;
        //        cout << middle << endl;
//        cout << "Left: " << leftDoors << "\tMiddle: " << middleDoors << "\tRight: " << rightDoors << endl;
        
        
        // loop on left side of building
        do {
            // loop on middle perimeter of building
            do {
                
                // loop on right side of building
                do {
                    // now that we have a full configuration of all building sides, run the simulation
                    // on that configuration.
//                    setExitDoors(left, middle, right);
                    resetExits();
                    int exitsPlaced = 0;
                    setLeftExit(leftDoors, exitsPlaced);
                    setRightExit(rightDoors, exitsPlaced);
                    setMiddleExit(middleDoors, exitsPlaced);
                    
                    assert(exitsPlaced == numExits);
                    
                    double meanAcc = 0;
                    // run N times, get avg and std. dev
                    for (int i = 0; i < N; i++) {
//                        numOfTimeSteps = simulate();
                        printSimulation("sim.csv");
                        reset();
                        meanAcc += numOfTimeSteps;
                        acc[i] = numOfTimeSteps;
                    }
                    
                    // calc mean
                    double mean = meanAcc/((double)N);
                    
                    // calc std. dev
                    double stdDev = 0;
                    for (int i = 0; i < N; i++) {
                        stdDev += pow((acc[i]-mean), 2);
                    }
                    
                    stdDev = stdDev/(N-1);
                    
                    counter++;
                    
                    fprintf(fraw, "%d", counter);
                    for (int i = 0; i < N; i++) {
                        fprintf(fraw, ",%d", acc[i]);
                    }
                    fprintf(fraw, "\n");
                    fflush(fraw);
                    
                    fprintf(fstats, "%d,%f,%f\n", counter, mean, stdDev);
                    fflush(fstats);
                    
                    fprintf(fconfigs, "%d,%s,%s,%s\n", counter, leftDoors.c_str(), middleDoors.c_str(), rightDoors.c_str());
                    fflush(fconfigs);
                    
                    
                    // since permutation of empty sides is irrelevant
//                    if (rightDoorCount == 0)
//                        break;
                    break;      // don't permute on right side
                    
                } while (next_permutation(rightDoors.begin(), rightDoors.end()));
                
                
                // since permutation of empty sides is irrelevant
                if (middleDoorCount == 0)
                    break;
                
            } while (next_permutation(middleDoors.begin(), middleDoors.end()));
            
            
            // since permutation of empty sides is irrelevant
//            if (leftDoorCount == 0)
//                break;
            
            break;      // dont permute on left side
        } while (next_permutation(leftDoors.begin(), leftDoors.end()));
        
    } while (next_permutation(sideConfigurations.begin(), sideConfigurations.end()));
    
    cout << results << endl;
    cout << counter << endl;
    
    fclose(fstats);
    fclose(fraw);
    fclose(fconfigs);
}

//// finds the best configuration
//void CellularAutomata::findBestConfiguration() {
//    /* Code Sketch:
//     * 
//     * 1) Permute doors in one of three sets: left, middle, right. These
//     *    sets represent the left, middle(top), and right sides of the 
//     *    building, respectively, onto which the doors will be placed.
//     * 2) For each of these permutations, permute the doors on these
//     *    three sides and run the simulation on each configuration.
//     */
//    
//    // Let A, B, and C represent the doors of sizes small, medium, and large, respectively.
//    // Moreover, let | represent a barrier, where two barriers will be needed to separate
//    // the three sides: the first is a barrier between the left and middle parts, and the
//    // second is a barrier between the middle and right parts.
//    
//    // We have four small doors, 1 medium, 1 large, and 2 barriers
//    string sideConfigurations = "AAAABC||";
//    
//    // To perform the permutations without repeating similar configurations (where two
//    // different 'A' occuring in the same position might be counted as different), we use
//    // the <next_permutation> function in the C++ <algorithm> library. Since this function
//    // returns the "next" permutation ordered lexicographically, we first sort our configuration
//    // list and then run the next_permutation until all permutations have been reached. Note
//    // that since the simultation runs of different door configurations are independent of
//    // each other, generating the permutations in a sorted fashion does not incur any bias.
//    
//    sort(sideConfigurations.begin(), sideConfigurations.end());     // sort configuration lexicographically
//    
//    int counter = 0;
//    int numOfTimeSteps = 0;
//    int meanAcc = 0;
//    int N = 2;
//    int acc[N];     //
//    string results = "";
//    
//    FILE* fout = fopen("myout", "w");
//    
//    // perform permutation to assign which doors go on which sides
//    do {
//        
//        // Now that we have an assignement of doors for each side of the building, we execute
//        // three nested loops of permutations: the ourtermost loop permutes the doors on the left
//        // side of the building covering the entire left perimeter, the middle loop does the same
//        // for the middle part of the building, and the innermost loop will cover the right side.
//        // That way, we will have all possible permutations of doors on all sides of the building.
//        
//        // Start by getting the doors of each side.
//        string leftDoors, middleDoors, rightDoors;
//        splitConfiguration(sideConfigurations, leftDoors, middleDoors, rightDoors);
//        
//        // In order to permute the doors on one of the sides, we do the following. Assume that we
//        // are permuting two doors, A and B, on left side of the building. Further assume that the
//        // left side has length 10, and doors A and B have lengths 2 and 4, respectively. Now, we
//        // represent a configuration as a string of length (#Side - #A+1 - #B+1) = (10 - 1 - 3) = 6,
//        // where '.' characters represent locations without doors, and locations with doors are
//        // represented by their respective doors (in this case, A or B). For example, if we placed
//        // A 2 units from the right and B 6 units from the right, the corresponding string representation
//        // will be: "..A.B." This representation allows us to easily permute the doors on any given side.
//        
////        map<char,int> leftSymbols = {{'-',3}, {'>', 1}};
////        map<char,int> middleSymbols = {{'-',5}, {'/', 3}, {'>', 5}};
////        map<char,int> rightSymbols = {{'-',3}, {'>', 1}};
//        
//        int leftDoorsLength = totalDoorLengthOnSide(leftDoors);
//        int middleDoorsLength= totalDoorLengthOnSide(middleDoors);
//        int rightDoorsLength = totalDoorLengthOnSide(rightDoors);
//        
//        map<char,int> leftSymbols = getLeftSymbols(leftDoorsLength);
//        map<char,int> middleSymbols = getMiddleSymbols(middleDoorsLength);
//        map<char,int> rightSymbols = getRightSymbols(rightDoorsLength);
//        
//        
//        string left = getSideRepresentation(leftDoors, BSPRING, leftSymbols);
//        int leftDoorCount = countNumOfDoors(left);
//        
//        string middle = getSideRepresentation(middleDoors, M5TH, middleSymbols);
//        int middleDoorCount = countNumOfDoors(middle);
//        
//        string right = getSideRepresentation(rightDoors, BSPRING, rightSymbols);
//        int rightDoorCount = countNumOfDoors(right);
//        
//        if (leftDoorCount > 2 || rightDoorCount > 2 || middleDoorCount > 4)
//            continue;
//        
//        
////        cout << left << endl;
////        cout << middle << endl;
//        cout << "Left: " << leftDoors << "\tMiddle: " << middleDoors << "\tRight: " << rightDoors << endl;
//    
//        
//        // loop on left side of building
//        do {
//            // loop on middle perimeter of building
//            do {
//                
//                // loop on right side of building
//                do {
//                    // now that we have a full configuration of all building sides, run the simulation
//                    // on that configuration.
//                    setExitDoors(left, middle, right);
//                    
//                    // run N times, get avg and std. dev
//                    for (int i = 0; i < N; i++) {
//                        numOfTimeSteps = simulate();
//                        reset();
//                        meanAcc += numOfTimeSteps;
//                        acc[i] = numOfTimeSteps;
//                    }
//                    
//                    // calc mean
//                    double mean = meanAcc/((double)N);
//                    
//                    // calc std. dev
//                    double stdDev = 0;
//                    for (int i = 0; i < N; i++) {
//                        stdDev += pow((acc[i]-mean), 2);
//                    }
//                    
//                    stdDev = stdDev/(N-1);
//                    
//                    results += to_string(counter) + "," + to_string(mean) + "," + to_string(stdDev) + "\n";
//                    counter++;
//                    
//                    for (int i = 0; i < N; i++) {
//                        cout << to_string(acc[i]) << " ";
//                    }
//                    cout << endl;
//                    
//                    fprintf(fout, "%d\t%f\t%f\n", counter, mean, stdDev);
//                    fflush(fout);
//                    
//                    // since permutation of empty sides is irrelevant
//                    if (rightDoorCount == 0)
//                        break;
//                    
//                } while (next_permutation(right.begin(), right.end()));
//                
//                
//                // since permutation of empty sides is irrelevant
//                if (middleDoorCount == 0)
//                    break;
//                
//            } while (next_permutation(middle.begin(), middle.end()));
//            
//            
//            // since permutation of empty sides is irrelevant
//            if (leftDoorCount == 0)
//                break;
//        } while (next_permutation(left.begin(), left.end()));
//        
//    } while (next_permutation(sideConfigurations.begin(), sideConfigurations.end()));
//    
//    cout << results << endl;
//    cout << counter << endl;
//    
//    fclose(fout);
//}





#pragma mark -
#pragma mark Setting up

/********************************************************\
            Setting up the Cellular Automata
\********************************************************/

// Allocates memory for the entire grid and partitions it into cells. It
// also specifies the locations of the sidewalks, targets, and the default
// location of building exits. Finally, it calculates the shortest paths
// from all cells to each of the target cells.
//
// Note: The x-axis goes from left to right, while the y-axis goes from
//       top to bottom (i.e. grid(i,j) is above grid(i,j+1)).
void CellularAutomata::initGrid() {
    width = L5TH + M5TH + R5TH + 2*SIDEWALK;            // width of grid
    height = TSPRING + MSPRING + BSPRING + 2*SIDEWALK;  // height of grid
    
    grid = new Cell** [height];
    for (int i =0; i < height; i++) {
        grid[i] = new Cell*[width];
        for (int j = 0; j < width; j++) {
            // setup cell (i,j), where i indicates row, and j indicates column
            grid[i][j] = new Cell(i,j);         // init a new cell
            grid[i][j]->valid = false;           // for now, set as invalid
        }
    }
    
    // set valid cells
    setSidewalks();
    
    // set targets
    setTargets();
    
    // set exits
    setExits();
    
    // setup traffic signals
    setUpTrafficSignals();
    
    // setup shortest paths
    calculateShortestPaths();
}



// sets the cells representing the sidewalks as "valid"
void CellularAutomata::setSidewalks() {
    // first sidewalk on L5th and M5th streets
    for (int i = TSPRING; i < TSPRING+SIDEWALK; i++) {
        for (int j = 0; j < L5TH+SRS+M5TH+SRS; j++) {
            // ignore road part on north west peach tree
            if (j >= L5TH+SRS+M5TH+SIDEWALK && j < L5TH+SRS+M5TH+SIDEWALK+ROAD_WIDTH)
                continue;
            
            grid[i][j]->valid = true;
            allValidCells.push_back(grid[i][j]);
        }
    }
    
    // second sidewalk on L5th and M5th streets
    for (int i = TSPRING+SIDEWALK+ROAD_WIDTH; i < TSPRING+SRS; i++) {
        for (int j = 0; j < L5TH+SRS+M5TH+SRS; j++) {
            grid[i][j]->valid = true;
            allValidCells.push_back(grid[i][j]);
        }
    }
    
    // continuation of 5th street (R5TH) - first sidewalk
    int iR5TH = height-BSPRING-SRS-ARM_WEST_M5TH-SIDEWALK-ROAD_WIDTH-1;     // i location r5TH road
    for (int i = iR5TH-SIDEWALK; i < iR5TH; i++) {
        for (int j = L5TH+SRS+M5TH+SIDEWALK+ROAD_WIDTH; j < width; j++) {
            grid[i][j]->valid = true;
            allValidCells.push_back(grid[i][j]);
        }
    }
    
    // continuation of 5th street (R5TH) - second sidewalk
    for (int i = iR5TH+ROAD_WIDTH; i < iR5TH+ROAD_WIDTH+SIDEWALK; i++) {
        for (int j = L5TH+SRS+M5TH; j < width; j++) {
            grid[i][j]->valid = true;
            allValidCells.push_back(grid[i][j]);
        }
    }
    
    // ARMSTEAD first sidewalk
    for (int i = height-BSPRING-SRS-1; i < height-BSPRING-SIDEWALK-ROAD_WIDTH-1; i++) {
        for (int j = L5TH; j < L5TH+SRS+M5TH+SIDEWALK; j++) {
            grid[i][j]->valid = true;
            allValidCells.push_back(grid[i][j]);
        }
    }
    
    // ARMSTEAD second sidewalk
    for (int i = height-BSPRING-SIDEWALK-1; i < height-BSPRING-1; i++) {
        for (int j = L5TH; j < L5TH+SRS+M5TH+SIDEWALK; j++) {
            // ignore road part
            if (j >= L5TH+SIDEWALK && j < L5TH+SIDEWALK+ROAD_WIDTH)
                continue;
            
            grid[i][j]->valid = true;
            allValidCells.push_back(grid[i][j]);
        }
    }
    
    // now vertical sidewalks
    
    // SPRING first sidewalk
    for (int i = 0; i < height; i++) {
        for (int j = L5TH; j < L5TH+SIDEWALK; j++) {
            grid[i][j]->valid = true;
            allValidCells.push_back(grid[i][j]);
        }
    }
    
    // SPRING second sidewalk
    for (int i = 0; i < height; i++) {
        for (int j = L5TH+SIDEWALK+ROAD_WIDTH; j < L5TH+SRS; j++) {
            grid[i][j]->valid = true;
            allValidCells.push_back(grid[i][j]);
        }
    }
    
    // west peach tree first sidewalk
    for (int i = 0; i < height; i++) {
        for (int j = L5TH+SRS+M5TH; j < L5TH+SRS+M5TH+SIDEWALK; j++) {
            grid[i][j]->valid = true;
            allValidCells.push_back(grid[i][j]);
        }
    }
    
    // west peach tree second sidewalk
    for (int i = 0; i < height; i++) {
        for (int j = L5TH+SRS+M5TH+SIDEWALK+ROAD_WIDTH; j < L5TH+SRS+M5TH+SRS; j++) {
            grid[i][j]->valid = true;
            allValidCells.push_back(grid[i][j]);
        }
    }
}


void CellularAutomata::setExits() {
    numExits = 4*getDoorLength('A') + getDoorLength('B') + getDoorLength('C');
    exits = new Cell* [numExits];
    
    int numPlaced = 0;
    setLeftExit("CA", numPlaced);
    setMiddleExit("BAA", numPlaced);
    setRightExit("A", numPlaced);
    
    assert(numPlaced == numExits);      // sanity check
}

// sets the locations of the targets
void CellularAutomata::setTargets() {
    // set targets
    numTargets = 8*SIDEWALK;        // 4 locations, each filling an entire sidewalk width
    targets = new Cell* [numTargets];
    
    int idx = 0;
    
    // set target on L-5th 1
    for (int i = 0; i < SIDEWALK; i++) {
        targets[idx] = grid[TSPRING+i][0];
        targets[idx]->target = true;
        idx++;
    }
    
    // set target on L-5th 2
    for (int i = 0; i < SIDEWALK; i++) {
        targets[idx] = grid[TSPRING+SIDEWALK+ROAD_WIDTH+i][0];
        targets[idx]->target = true;
        idx++;
    }
    
    // set target on TSPRING 1
    for (int i = 0; i < SIDEWALK; i++) {
        targets[idx] = grid[0][L5TH+i];
        targets[idx]->target = true;
        idx++;
    }
    
    // set target on TSPRING 2
    for (int i = 0; i < SIDEWALK; i++) {
        targets[idx] = grid[0][L5TH+SIDEWALK+ROAD_WIDTH+i];
        targets[idx]->target = true;
        idx++;
    }
    
    // set target on Top-WestPeachTree 1
    for (int i = 0; i < SIDEWALK; i++) {
        targets[idx] = grid[0][L5TH+SIDEWALK+ROAD_WIDTH+SIDEWALK+M5TH+i];
        targets[idx]->target = true;
        idx++;
    }
    
    // set target on Top-WestPeachTree 2
    for (int i = 0; i < SIDEWALK; i++) {
        targets[idx] = grid[0][L5TH+SIDEWALK+ROAD_WIDTH+SIDEWALK+M5TH+SIDEWALK+ROAD_WIDTH+i];
        targets[idx]->target = true;
        idx++;
    }
    
    // set target on R-5th 1
    for (int i = 0; i < SIDEWALK; i++) {
        targets[idx] = grid[height-BSPRING-SIDEWALK-ROAD_WIDTH-SIDEWALK-ARM_WEST_M5TH-2-i][width-1];
        targets[idx]->target = true;
        idx++;
    }
    
    // set target on R-5th 2
    for (int i = 0; i < SIDEWALK; i++) {
        targets[idx] = grid[height-BSPRING-SIDEWALK-ROAD_WIDTH-SIDEWALK-ARM_WEST_M5TH-SIDEWALK-ROAD_WIDTH-2-i][width-1];
        targets[idx]->target = true;
        idx++;
    }
}


// sets up the traffic signals
void CellularAutomata::setUpTrafficSignals() {
    // north west signals
//    TrafficSignal *nw = new TrafficSignal(TS_NW_RED, TS_NW_GREEN, SEC_PER_STEP, NW_L);
    TrafficSignal *nw = new TrafficSignal(TS_NW_RED, TS_NW_GREEN, SEC_PER_STEP, NORTH);
    for (int i = 0; i < SIDEWALK; i++) {
        grid[TSPRING+SIDEWALK+ROAD_WIDTH-1][L5TH+i]->setTrafficSignal(new TrafficSignal(*nw));
    }
    
    for (int i = 0; i < SIDEWALK; i++) {
        grid[TSPRING+SIDEWALK+ROAD_WIDTH-1][L5TH+SIDEWALK+ROAD_WIDTH+i]->setTrafficSignal(new TrafficSignal(*nw));
    }
    
    // generate complement of nw signal
    TrafficSignal *nwc = new TrafficSignal(*nw);
    nwc->complement(WEST);
    for (int i = 0; i < SIDEWALK; i++) {
        grid[TSPRING+i][L5TH+SIDEWALK+ROAD_WIDTH-1]->setTrafficSignal(new TrafficSignal(*nwc));
    }
    
    for (int i = 0; i < SIDEWALK; i++) {
        grid[TSPRING+SIDEWALK+ROAD_WIDTH+i][L5TH+SIDEWALK+ROAD_WIDTH-1]->setTrafficSignal(new TrafficSignal(*nwc));
    }
    
    
    // north east signal
//    TrafficSignal *ne = new TrafficSignal(TS_NE_RED, TS_NE_GREEN, SEC_PER_STEP, NE_L);
    TrafficSignal *ne = new TrafficSignal(TS_NE_RED, TS_NE_GREEN, SEC_PER_STEP, NORTH);
    for (int i = 0; i < SIDEWALK; i++) {
        grid[TSPRING+SIDEWALK+ROAD_WIDTH-1][L5TH+SRS+M5TH+i]->setTrafficSignal(new TrafficSignal(*ne));
    }
    
    for (int i = 0; i < SIDEWALK; i++) {
        grid[TSPRING+SIDEWALK+ROAD_WIDTH-1][L5TH+SRS+M5TH+SIDEWALK+ROAD_WIDTH+i]->setTrafficSignal(new TrafficSignal(*ne));
    }
    
    // generate complement of ne signal
    TrafficSignal *nec = new TrafficSignal(*ne);
    nec->complement(WEST);
    for (int i = 0; i < SIDEWALK; i++) {
        grid[TSPRING+SIDEWALK+ROAD_WIDTH+i][L5TH+SRS+M5TH+SIDEWALK+ROAD_WIDTH-1]->setTrafficSignal(new TrafficSignal(*nec));
    }
    
    // east signal
//    TrafficSignal *e = new TrafficSignal(TS_E_RED, TS_E_GREEN, SEC_PER_STEP, E_T);
    TrafficSignal *e = new TrafficSignal(TS_E_RED, TS_E_GREEN, SEC_PER_STEP, EAST);
    for (int i = 0; i < SIDEWALK; i++) {
        grid[height-BSPRING-SIDEWALK-ROAD_WIDTH-SIDEWALK-ARM_WEST_M5TH-i-2][L5TH+SIDEWALK+ROAD_WIDTH+SIDEWALK+M5TH+SIDEWALK]->setTrafficSignal(new TrafficSignal(*e));
    }
    
    TrafficSignal *ec = new TrafficSignal(*e);
    ec->complement(NORTH);
    for (int i = 0; i < SIDEWALK; i++) {
        grid[height-BSPRING-SIDEWALK-ROAD_WIDTH-SIDEWALK-ARM_WEST_M5TH-ROAD_WIDTH-1][L5TH+SIDEWALK+ROAD_WIDTH+SIDEWALK+M5TH+SIDEWALK+ROAD_WIDTH+i]->setTrafficSignal(new TrafficSignal(*ec));
    }
    
    delete nw;
    delete ne;
    delete e;
}




#pragma mark -
#pragma mark Updating

/********************************************************\
            Updating the Cellular Automata
\********************************************************/


// Updates the entire grid by one "time-step".
void CellularAutomata::update() {
    size_t numOfValidCells = allValidCells.size();
    
    int idx [numOfValidCells];     // index vector of valid cells
    for (int i = 0; i < numOfValidCells; i++)
        idx[i] = i;
    
    // randomly permute the index list
    random_shuffle(&idx[0], &idx[numOfValidCells]);
    
    // update cells in permuted order
    for (int i = 0; i < numOfValidCells; i++) {
        updateCell(allValidCells[idx[i]]);
    }
}



// updates cell
void CellularAutomata::updateCell(Cell *current) {
    
    assert(current != NULL);            // sanity check
    
    // can't update invalid cells
    if (!current->valid)
        return;
    
    current->update();
    
    // can't work with unoccupid cells unless they are exits
    if (!current->isOccupied()) {
        // if cell is an unoccupied exit and there are more people inside the
        // building, then let one person exit
        if (current->exit && remaining > 0) {
            current->enter(new Pedestrian(speedRNG, current->getShortestPathIndex()), true);      // occupy exit with new pedestrian from building
            remaining--;                // decrement people in building
            return;
        }
        else
            return;
    }
    else {
        // skip pedestrians who can't move this turn (i.e. because of their speeds)
        if (!current->getPedestrian()->canMove())
            return;
        
        // if cell is occupied and target and pedestrian can move, clear it
        if (current->target) {
            Pedestrian* p = current->leave();
            delete p;       // since pedestrian is evacuated
            evacuated++;
            assert(evacuated <= populationSize);        // sanity check
            return;
        }
    }
    
    // otherwise, if cell is occupied, update it
    // according to pedestrian dynamics rules
    
    double minScore = INT_MAX;
    Cell* minCell = NULL;
    
    double minShortestScore = INT_MAX;
    Cell *minShortestCell = NULL;
    
    
    // if cell is occupied, choose a vacant cell from neighbours
    for (int i = current->getI()-1; i <= current->getI()+1; i++) {
        for (int j = current->getJ()-1; j <= current->getJ()+1; j++) {
            // ignore self
            if (i == current->getI() && j == current->getJ())
                continue;
            
            // skip invalid neighbours
            if (!isValidNeighbourPos(i, j))
                continue;
            
            // skip occupied neighours since cell can be occupied by just one pedestrian
            if (grid[i][j]->isOccupied())
                continue;
            
            // skip neighbours that require pedestrian to pass red light
            if (current->passesRedLight(i, j))
                continue;
            
            // skip neighbours with higher scores (i.e. further away from target)
            if (current->getPedestrianShortestPath() < grid[i][j]->getShortestPath(current->getPedestrian()->getPath()) && current->getScore(current->getPedestrian()->getPath()) < grid[i][j]->getScore(current->getPedestrian()->getPath()))
                continue;
            
            // if the current neighbour's score is less than the minimum currently
            // found, record it
            if (grid[i][j]->getScore(current->getPedestrian()->getPath()) < minScore) {
                minScore = grid[i][j]->getScore(current->getPedestrian()->getPath());
                minCell = grid[i][j];
            }
            
    
            if (grid[i][j]->getShortestPath(current->getPedestrian()->getPath()) < minShortestScore) {
                minShortestScore = grid[i][j]->getShortestPath(current->getPedestrian()->getPath());
                minShortestCell = grid[i][j];
            }
            
        }
    }
    
    if (minCell != NULL) {
        // move occupant from current cell to neightbour cell
        minCell->enter(current->leave(), false);
    }
    else if (minShortestCell != NULL) {
        minShortestCell->enter(current->leave(), false);
    }
}



// Compares two (pointers to) Cell objects by their shortest path to a target.
struct compareShortestPath {
    bool operator ()(const Cell *c1, const Cell *c2) const {
        return c1->getShortestPathBasedState() > c2->getShortestPathBasedState();
    }
};



// Purpose : Calculate shortest path to a set of targets
void CellularAutomata::calculateShortestPaths() {
    bool reset = true;
    for (int i = 0; i < numTargets; i++) {
        dijkstra(targets[i], reset);
    }
}

// Purpose : Calculate shortest path from source to all other cells.
void CellularAutomata::dijkstra(Cell *source, bool reset) {

    // get path index based on source
    int pathIndex;
    if (source->getI() == 0) {
        // then source is either NW or NE
        if (source->getJ() >= L5TH && source->getJ() < L5TH+SIDEWALK)
            pathIndex = NW_L;
        else if (source->getJ() >= L5TH+SIDEWALK+ROAD_WIDTH && source->getJ() < L5TH+SIDEWALK+ROAD_WIDTH+SIDEWALK) {
            pathIndex = NW_R;
        }
        else if (source->getJ() >= L5TH+SRS+M5TH && source->getJ() < L5TH+SRS+M5TH+SIDEWALK)
            pathIndex = NE_L;
        else
            pathIndex = NE_R;
    }
    else if (source->getJ() == 0) {
        if (source->getI() < TSPRING+SIDEWALK)
            pathIndex = W_T;
        else
            pathIndex = W_B;
    }
    else {
        if (source->getI() < height-BSPRING-SRS-ARM_WEST_M5TH-SIDEWALK-ROAD_WIDTH)
            pathIndex = E_T;
        else
            pathIndex = E_B;
    }

    
    
    if (reset) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                grid[i][j]->setPathLength(pathIndex, INT_MAX);   // infinite score
                grid[i][j]->previous = NULL;     // undefined previous
            }
        }
    }
    
    bool debug2 = false;
    bool debug = false;
    if (debug2) {
        cout << "Path Index = " << pathIndex << endl;
    }
        
    std::priority_queue<Cell*, vector<Cell*>,compareShortestPath> pq;
    
    // setup source
    source->setPathLength(pathIndex, 0);
    source->previous = NULL;
    pq.push(source);
    
    if (debug)
        cout << "Source: (" << source->getI() << ", " << source->getJ() << ")" << endl;
    
    source->setCurrentPath(pathIndex);
    
    // while queue is not empty
    while (!pq.empty()) {
        Cell *u = pq.top();
        pq.pop();           // pop from queue
        
        if (debug)
            cout << "Cell (" << u->getI() << ", " << u->getJ() << ") popped. Shortest path = " << u->getShortestPath(pathIndex) << endl;
        
        // look at neighbors of u
        for (int i = u->getI()-1; i <= u->getI()+1; i++) {
            for (int j = u->getJ()-1; j <= u->getJ()+1; j++) {
                
                // ignore self
                if (i == u->getI() && j == u->getJ())
                    continue;
                
                // check if valid neighbour
                if (!isValidNeighbourPos(i,j))
                    continue;
                
                grid[i][j]->setCurrentPath(pathIndex);
                
                
                // relax neighbour
                if (grid[i][j]->getShortestPath(pathIndex) > u->getShortestPath(pathIndex) + 1) {
                    grid[i][j]->setPathLength(pathIndex, u->getShortestPath(pathIndex) + 1);
                    
                    if (debug)
                        cout << "Relaxed: (" << grid[i][j]->getI() << ", " << grid[i][j]->getJ() << ") = " << grid[i][j]->getShortestPath(pathIndex) << endl;
                    
                    grid[i][j]->previous = u;
                    pq.push(grid[i][j]);
                }
            }
        }
    }
}



#pragma mark -
#pragma mark Functional Utilities

/********************************************************\
                Functional Utilities
\********************************************************/

// Returns true if two cells (pointed to by pointers) are located at the same
// position on the grid.
inline bool equal(Cell* c1, Cell* c2) {
    if (c1->getI() == c2->getI() && c1->getJ() == c2->getJ())
        return true;
    return false;
}


// returns true if the cell (i,j) is a valid cell
inline bool CellularAutomata::isValidNeighbourPos(int i, int j) {
    if (i < 0 || j < 0 || i >= height || j >= width)
        return false;
    
    if (!grid[i][j]->valid)
        return false;
    
    return true;
}

// prints the grid of pedestrian locations
void CellularAutomata::printOccupationGrid() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j]->isOccupied())
                std::cout << "1";
            else {
                if (grid[i][j]->target)
                    std::cout << "t";           // target
                else if (grid[i][j]->exit)
                    std::cout << "e";           // exit
                else if (grid[i][j]->hasTrafficLight())
                    std::cout << "T";
                else if (grid[i][j]->valid)
                    std::cout << "-";           // empty valid
                else
                    cout << "x";                // invalid
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


/* A function to count the number of digits in an integer
 * Note: This only works for positive integers.
 */
int countDigits2(int number) {
    if (number < 10) {
        return 1;
    }
    int count = 0;
    while (number > 0) {
        number /= 10;
        count++;
    }
    return count;
}

int countDigits(double number) {
    char buffer [50];
    int n;
    n=sprintf (buffer, "%f",number);
    return n;
}

void CellularAutomata::printMatlabGrid() {
    
    FILE* fout = fopen("grid.csv", "w");
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j > 0)
                fprintf(fout, ",");
            
            if (grid[i][j]->hasTrafficLight()) {
                fprintf(fout, "2");        // traffic light
            }
            else if (grid[i][j]->valid)
                fprintf(fout, "1");         // empty valid
            else
                fprintf(fout, "0");                // invalid
            
        }
        fprintf(fout, "\n");
    }
    fprintf(fout, "\n");
    
    fclose(fout);
}

// Prints the grid of cell scores
void CellularAutomata::printGrid() {
    int spaceLength = countDigits2(int(width*height));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int spaceUsed = 0;
            if (grid[i][j]->valid) {
                std::cout << grid[i][j]->getShortestPath(NE_L) << " ";
                spaceUsed = countDigits2(int(grid[i][j]->getShortestPath(NE_L)));

            }
            else {
                std::cout << "-";
                spaceUsed = 0;
            }
            
            for (int s = spaceUsed; s < spaceLength; s++) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}


void CellularAutomata::printIteration(FILE *f) {
    // look for all pedestrians and print their locations
    for (int i = 0; i < allValidCells.size(); i++) {
        if (allValidCells[i]->isOccupied()) {
            fprintf(f, "%d,%d,%d\n", allValidCells[i]->getI(), allValidCells[i]->getJ(), allValidCells[i]->getPedestrian()->getSpeed());
        }
    }
}

// prints the simulation run by printing location of pedestrians at each
// iteration.
void CellularAutomata::printSimulation(string fname) {
    FILE* fout = fopen(fname.c_str(), "w");
    
    int iter = 0;
    while (evacuated < populationSize) {
        update();
        printIteration(fout);
        fprintf(fout, "-1,-1,-1\n");
        fflush(fout);
        ++iter;
        if (iter == 15000)
            break;
    }
    
    cout << "Num of Iterations = " << iter << endl;
    
    fclose(fout);
}


