#include "ANN.h"


ANN::ANN(int ic , int oc , int lc , int elc)
{
    layerCount = lc;
    inputCount = ic;
    outputCount = oc;
    eachLayerCount =elc;
    node = new Neuron ** [lc];
    outputs = new float[oc];
    trainSet = new float[oc];
    inputs = new float[ic];

    tmpOutput = new float* [layerCount];
    for(int i=0;i<layerCount-1;i++)
    {
        tmpOutput[i]=new float[eachLayerCount];
    }
    tmpOutput[layerCount-1]=new float[outputCount];

    sigma = new float* [layerCount];
    for(int i=0;i<layerCount-1;i++)
    {
        sigma[i]=new float[eachLayerCount];
    }
    sigma[layerCount-1]=new float[outputCount];


    node[0] = new Neuron*[eachLayerCount];
    for(int i=0;i<eachLayerCount;i++)
    {
        node[0][i]= new Neuron(inputCount);
    }


    for(int i=1;i<=lc-2;i++)
    {
        node[i]=new Neuron*[eachLayerCount];
        for(int j=0;j<eachLayerCount;j++)
        {
            node[i][j]= new Neuron(eachLayerCount);
        }
    }

    node[lc-1] = new Neuron*[outputCount];
    for(int i=0;i<outputCount;i++)
    {
        node[lc-1][i]= new Neuron(eachLayerCount);
    }
	std::cout<<"Boz"<<std::endl;

}

void ANN::run(float *inputData)
{

    for(int i=0;i<inputCount;i++)
    {
        tmpOutput[0][i] = node[0][i]->run(inputs);
        //std::cout<<"Inputs Are : "<<inputs[i]<<std::endl;
        //std::cout<<"Output "<<i<<" : "<<tmpOutput[0][i]<<std::endl;
    }


    for(int i=1;i<=layerCount-2;i++)
    {
        for(int j=0;j<eachLayerCount;j++)
        {
            tmpOutput[i][j]=node[i][j]->run(tmpOutput[i-1]);
        }
    }

    for(int i=0;i<outputCount;i++)
    {
        tmpOutput[layerCount-1][i]=node[layerCount-1][i]->run(tmpOutput[layerCount-2]);
    }

    outputs = tmpOutput[layerCount-1];
	
	std::cout<<std::endl<<"ANN Input  : "<<inputs[0]<<"        "<<inputs[1]<<"        ";//<<inputs[2]<<"        ";
	std::cout<<std::endl<<"ANN Output : "<<outputs[0]<<"        "<<outputs[1]<<"        ";//<<outputs[2]<<"        ";
	std::cout<<std::endl<<"ANN Train : "<<trainSet[0]<<"        "<<trainSet[1]<<"        "<<std::endl;//<<trainSet[2]<<"        "<<std::endl;
 
}

void ANN::backprop(float y)
{

    for(int j=0;j<outputCount;j++)
    {
        float o = tmpOutput[layerCount-1][j];
        float d = trainSet[j];
        float e = d-o;
        sigma[layerCount-1][j]=(1-o)*o*(d-o);
    }

    for(int k=layerCount-2;k>=0;k--)
    {
        for(int j=0;j<eachLayerCount;j++)
        {

              float o = tmpOutput[k][j];
              float tt=0;
              int w =eachLayerCount;
              if(k==layerCount-2)
                  w = outputCount;

              for(int r=0;r<w;r++)
              {
                   tt+=sigma[k+1][r]*node[k+1][r]->weights[j];
              }
              sigma[k][j]= o*(1-o)*tt;
        }
    }    

    for(int k=0;k<layerCount;k++)
    {
        int t=eachLayerCount;
        if(k==layerCount-1)
            t=outputCount;

        for(int p=0;p<t;p++)
        {
            for(int i=0;i<node[k][p]->count;i++)
            {
                float oi = inputs[i];
                if(k>0)
                    oi = tmpOutput[k-1][i];
                node[k][p]->weights[i]+=(y)*oi*sigma[k][p];
            }
            node[k][t-1]->weights[node[k][t-1]->count-1]+=y*sigma[k][t-1];
        }
    }

}
