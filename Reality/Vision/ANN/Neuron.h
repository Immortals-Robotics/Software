#ifndef NEURON_H
#define NEURON_H

#include "stdlib.h"
#include "time.h"
#include "math.h"

class Neuron
{
public:
    Neuron(int inputsCount);
    ~Neuron();

    void setCount(int i);
    float * weights;
    float output;
    float run(float * inputs);
    int count;
};

#endif
