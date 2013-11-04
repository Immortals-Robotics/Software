#ifndef ANN_H
#define ANN_H

#include "Neuron.h"
#include "iostream"

class ANN
{
public:
    ANN(int ic , int oc , int lc , int elc);
    ~ANN();
    void Qlearning();
    void backprop(float y);

    void run( float * inputData);

    float * inputs;
    float * outputs;
    float * trainSet;
    int inputCount;
    int outputCount;
    int layerCount;
    int eachLayerCount;
    Neuron *** node;
    float ** tmpOutput;
    float ** sigma;



private:
};

#endif
