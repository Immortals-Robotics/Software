#include "Neuron.h"
#include <iostream>

Neuron::Neuron(int inputsCount)
{
    weights = new float[inputsCount];
    count = inputsCount;
	std::cout<<"kard"<<std::endl;
    for(int i=0;i<inputsCount;i++)
    {
        weights[i]=rand()%1000;
        weights[i]/=1000.0;
    }
}

#define alpha 0.6

float Neuron::run(float *inputs)
{
    float sum=0;
	float max=0;
    for(int i=0;i<count;i++)
    {
        sum+=weights[i]*inputs[i];
		max+=inputs[i];
    }
    //sum = (1.0/exp(alpha*sum))+1;
    //sum = 1.0/sum;
	sum = sum / max;
    //std::cout<< sum<<std::endl;
    if(sum > 1)
        return 1;
    return sum;
}

void Neuron::setCount(int i)
{
    int count=i;
}
