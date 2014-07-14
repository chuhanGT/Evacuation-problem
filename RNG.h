//
//  RNG.h
//  CSE 6730 Project 1
//
//  Created by Joanne Torcivia on 1/22/14.
//  Copyright (c) 2014 Joanne Torcivia. All rights reserved.
//

#ifndef __CSE_6730_Project_1__RNG__
#define __CSE_6730_Project_1__RNG__

#include <iostream>

using namespace std;

class RNG {
    int a;
    int c;
    int m;
    int seed;
    int x_i;
    
public:
    /**
     * Constructor that initializes a (pseudo) random number generator.
     */
    RNG();
    
    /**
     * Generate a random number from a (0,1) uniform distribution.
     *
     * @return uniformly generated number between 0 and 1.
     */
    double generateUniform();
    
    /**
     * Generate a random number from an exponential distribution
     *
     * @return random number generated from exponential distribution.
     */
    double generateExponential();
    
    /**
     * Generate a random number from a (discrete) binomial distribution
     *
     * @return random number generated from binomial distribution.
     */
    double generateBinomial();
    
    /**
     * Generate a sample of random numbers, put them into equal sized bins, and print the
     * bin frequencies to a file.
     *
     * @param distribution the distribution from which to sample.
     * @param sampleLength the number of samples to generate
     * @param numberOfBins the number of bins
     * @param fname the name of the output file
     */
    void generateSample(string distribution, int sampleLength, int numberOfBins, string fname);
    
};

#endif /* defined(__CSE_6730_Project_1__RNG__) */
