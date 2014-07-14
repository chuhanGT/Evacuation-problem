//
//  RNG.cpp
//  CSE 6730 Project 1
//
//  Created by Joanne Torcivia on 1/22/14.
//  Copyright (c) 2014 Joanne Torcivia. All rights reserved.
//

#include "RNG.h"
#include <math.h>
#include <stdio.h>

// constructor to initialize model parameters
RNG::RNG() {
    
    // setup model parameters
    a=2593;
    c=24349;
    m=65536;
    
    // initialize seed
    seed=(time(NULL) % 100000);
//    seed=(343234342431 % 100000);

    x_i=seed; // set x_0 to initial seed value

}

// generate a new random number
double RNG::generateUniform() {
    
    // compute next random number
    int x_j = ((a*x_i+c) % (m));
    x_i = x_j;
    
    return x_j/65536.0;
}


double RNG::generateExponential() {
    double num = generateUniform();
    return -log(1-num);
}


double RNG::generateBinomial() {
    double r = generateUniform();
    
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


void RNG::generateSample(string distribution, int sampleLength, int numberOfBins, string fname) {
    // function pointer
    double (RNG::*generator)() = NULL;
    
    if (distribution == "exponential")
        generator = &RNG::generateExponential;
    else if (distribution == "uniform")
        generator = &RNG::generateUniform;
    else if (distribution == "binomial")
        generator = &RNG::generateBinomial;
    else
        return;
    
    // generate <sampleLength> random numbers from assigned distribution, normalize them,
    // and put them in <numberOfBins> equal-sized bins.
    double numbers[sampleLength];
    double max = -1;
    
    for (int i = 0; i < sampleLength; i++) {
        numbers[i] = (this->*generator)();
        if (max < numbers[i])
            max = numbers[i];
    }
    
    // normalize
    for (int i = 0; i < sampleLength; i++)
        numbers[i] = numbers[i]/max;
    
    // put in bins
    int bins[numberOfBins];
    for (int i = 0; i < numberOfBins; i++)
        bins[i] = 0;
    
    double binWidth = 1.0/numberOfBins;
    for (int i = 0; i < sampleLength; i++) {
        double current = numbers[i];
        
        for (int j = 0; j < numberOfBins; j++) {
            if (current <= (j+1)*binWidth) {
                bins[j] += 1;
                break;
            }
        }
    }
    
    // print to csv file
    FILE* csv = std::fopen(fname.c_str(), "w");
    for (int i = 0; i < numberOfBins; i++)
        fprintf(csv, "%d\n", bins[i]);
    fclose(csv);
    
    
}





