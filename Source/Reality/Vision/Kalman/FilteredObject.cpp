// FilteredObject.cpp: implementation of the FilteredObject class.
//
//////////////////////////////////////////////////////////////////////

#include "FilteredObject.h"
#include <iostream>
#include <fstream>

#include <math.h>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const float VELOCITY_THRES = 0.5;

FilteredObject::FilteredObject()
{
  usageCount = 0;
}

FilteredObject::FilteredObject(float A[2][2], float B[2], float C[2][2], float D[2],float lossMat[2])
{
  // setup up the initial variables
  for (int i=0;i<2;i++)
  {
    Bimp[i] = B[i];
    Dimp[i] = D[i];
    lossVec[i] = lossMat[i];
    for (int k=0;k<2;k++)
    {
      Aimp[i][k] = A[i][k];
      Cimp[i][k] = C[i][k];
    }
  }
  usageCount = 0;
}

void FilteredObject::initialize(const char* filename1, const char* filename2)
{
  // open the file 1
  ifstream matFile(filename1);
  if (matFile.is_open())
  {
    matFile>>Aimp[0][0];matFile>>Aimp[0][1];
    matFile>>Aimp[1][0];matFile>>Aimp[1][1];
    matFile>>Bimp[0];matFile>>Bimp[1];  
    matFile>>Cimp[0][0];matFile>>Cimp[0][1];
    matFile>>Cimp[1][0];matFile>>Cimp[1][1]; 
    matFile>>Dimp[0];matFile>>Dimp[1];
    matFile>>lossVec[0];matFile>>lossVec[1];
    // close the file
    matFile.close();
    usageCount = 0;

    // calculate the inverse of C matrix
    // inv(C)= inv(a b) =  (1/(ad-bc)) (d  -b)
    //            (c d)                (-c  a)

    float determinant = 1.0f/(Cimp[0][0]*Cimp[1][1]-Cimp[0][1]*Cimp[1][0]);
    CimpInv[0][0] = determinant*Cimp[1][1];
    CimpInv[0][1] = -determinant*Cimp[0][1];
    CimpInv[1][0] = -determinant*Cimp[1][0];
    CimpInv[1][1] = determinant*Cimp[0][0];

  }
  else
  {
    cout<<"Invalid initialization file 1"<<endl;
  }

  // open the file 2
  ifstream matFile2(filename2);
  if (matFile2.is_open())
  {
    matFile2>>AimpP[0][0];matFile2>>AimpP[0][1];
    matFile2>>AimpP[1][0];matFile2>>AimpP[1][1];
    matFile2>>BimpP[0];matFile2>>BimpP[1];  
    matFile2>>CimpP[0][0];matFile2>>CimpP[0][1];
    matFile2>>CimpP[1][0];matFile2>>CimpP[1][1]; 
    matFile2>>DimpP[0];matFile2>>DimpP[1];
    matFile2>>lossVecP[0];matFile2>>lossVecP[1];
    // close the file
    matFile2.close();
  }
  else
  {
    cout<<"Invalid initialization file 1"<<endl;
  }
}

void FilteredObject::initializePos(float pos[2])
{
  filtState[0][0] = lossVec[0]*pos[0]; filtState[0][1] = lossVec[1]*pos[0]; 
  filtState[1][0] = lossVec[0]*pos[1]; filtState[1][1] = lossVec[1]*pos[1]; 

  filtStateP[0][0] = lossVecP[0]*pos[0]; filtStateP[0][1] = lossVecP[1]*pos[0]; 
  filtStateP[1][0] = lossVecP[0]*pos[1]; filtStateP[1][1] = lossVecP[1]*pos[1]; 
}


FilteredObject::~FilteredObject()
{
  // No resources to reclaim. Currently redundant function
}


void FilteredObject::updatePosition(float z[2], float filtOut[2][2])
{
    
  float filtOutP[2][2];

  if (usageCount==0) 
    initializePos(z);
  usageCount++;
  /***********************************************
   filtOut   = Cimp*filtState + Dimp*posMeasure
  ************************************************/
  // x axis 
  filtOut[0][0] = Cimp[0][0]*filtState[0][0]+Cimp[0][1]*filtState[0][1] + Dimp[0]*z[0];
  filtOut[0][1] = Cimp[1][0]*filtState[0][0]+Cimp[1][1]*filtState[0][1] + Dimp[1]*z[0];
  filtOutP[0][0] = CimpP[0][0]*filtStateP[0][0]+CimpP[0][1]*filtStateP[0][1] + DimpP[0]*z[0];
  filtOutP[0][1] = CimpP[1][0]*filtStateP[0][0]+CimpP[1][1]*filtStateP[0][1] + DimpP[1]*z[0];
  
  
  // y axis
  filtOut[1][0] = Cimp[0][0]*filtState[1][0]+Cimp[0][1]*filtState[1][1] + Dimp[0]*z[1];
  filtOut[1][1] = Cimp[1][0]*filtState[1][0]+Cimp[1][1]*filtState[1][1] + Dimp[1]*z[1];
  filtOutP[1][0] = CimpP[0][0]*filtStateP[1][0]+CimpP[0][1]*filtStateP[1][1] + DimpP[0]*z[1];
  filtOutP[1][1] = CimpP[1][0]*filtStateP[1][0]+CimpP[1][1]*filtStateP[1][1] + DimpP[1]*z[1];
  
  // add in the check to revert to the fast filter state if certain conditions are met
  // filtState = CimpInv * (filtOutP - Dimp*z)
  // x-axis
  if (fabs(filtOut[0][1]-filtOutP[0][1])>VELOCITY_THRES)
  {
    // x-axis
    filtState[0][0] = CimpInv[0][0]*(filtOutP[0][0]-Dimp[0]*z[0]) +
                      CimpInv[0][1]*(filtOutP[0][1]-Dimp[1]*z[0]);
    filtState[0][1] = CimpInv[1][0]*(filtOutP[0][0]-Dimp[0]*z[0]) +
                      CimpInv[1][1]*(filtOutP[0][1]-Dimp[1]*z[0]);
    // since there is a change in filter state, update output again
    // x-axis
    filtOut[0][0] = Cimp[0][0]*filtState[0][0]+Cimp[0][1]*filtState[0][1] + Dimp[0]*z[0];
    filtOut[0][1] = Cimp[1][0]*filtState[0][0]+Cimp[1][1]*filtState[0][1] + Dimp[1]*z[0];
  }
    
   
  if (fabs(filtOut[1][1]-filtOutP[1][1])>VELOCITY_THRES)
  { 
    // y-axis
    filtState[1][0] = CimpInv[0][0]*(filtOutP[1][0]-Dimp[0]*z[1]) +
                      CimpInv[0][1]*(filtOutP[1][1]-Dimp[1]*z[1]);
    filtState[1][1] = CimpInv[1][0]*(filtOutP[1][0]-Dimp[0]*z[1]) +
                      CimpInv[1][1]*(filtOutP[1][1]-Dimp[1]*z[1]);

    // y axis
    filtOut[1][0] = Cimp[0][0]*filtState[1][0]+Cimp[0][1]*filtState[1][1] + Dimp[0]*z[1];
    filtOut[1][1] = Cimp[1][0]*filtState[1][0]+Cimp[1][1]*filtState[1][1] + Dimp[1]*z[1];
  }

  /***********************************************
  filtState = Aimp*filtState + Bimp*posMeasure
  ************************************************/
  // x axis

  // Aimp*filtState + Bimp*posMeasure
  
  // remember temporary matrix fast and applying the update
  float tempfiltState[2][2]; float tempfiltStateP[2][2];
  
  tempfiltState[0][0] = Aimp[0][0]*filtState[0][0] +Aimp[0][1]*filtState[0][1] + Bimp[0]*z[0];
  tempfiltState[0][1] = Aimp[1][0]*filtState[0][0] +Aimp[1][1]*filtState[0][1] + Bimp[1]*z[0];
  tempfiltStateP[0][0] = AimpP[0][0]*filtStateP[0][0] +AimpP[0][1]*filtStateP[0][1] + BimpP[0]*z[0];
  tempfiltStateP[0][1] = AimpP[1][0]*filtStateP[0][0] +AimpP[1][1]*filtStateP[0][1] + BimpP[1]*z[0];

  // y axis
  tempfiltState[1][0] =Aimp[0][0]*filtState[1][0] +Aimp[0][1]*filtState[1][1] + Bimp[0]*z[1];
  tempfiltState[1][1] =Aimp[1][0]*filtState[1][0] +Aimp[1][1]*filtState[1][1] + Bimp[1]*z[1];
  tempfiltStateP[1][0] =AimpP[0][0]*filtStateP[1][0] +AimpP[0][1]*filtStateP[1][1] + BimpP[0]*z[1];
  tempfiltStateP[1][1] =AimpP[1][0]*filtStateP[1][0] +AimpP[1][1]*filtStateP[1][1] + BimpP[1]*z[1];

  // copy the components of the temp back to the actual

  filtState[0][0] = tempfiltState[0][0]; filtState[0][1] = tempfiltState[0][1];
  filtState[1][0] = tempfiltState[1][0]; filtState[1][1] = tempfiltState[1][1];

  filtStateP[0][0] = tempfiltStateP[0][0]; filtStateP[0][1] = tempfiltStateP[0][1];
  filtStateP[1][0] = tempfiltStateP[1][0]; filtStateP[1][1] = tempfiltStateP[1][1];


}



  
	
