#include "vision.h"
#include "ANN.h"
#include "Geom.h"
#include "timer.h"
#include "doublefann.h"

#include <fstream>
ofstream* nikRastKarde;

RobotState states[15][MAX_ROBOTS];
WorldState wstates[15];

int currentStateIndex=0;
int firstRunCount=0;
Timer t;
int isSaved=0;

struct fann * ann;

#define ANN_DEBUG 0

void VisionModule::InitANN ( void )
{
	
	//ann = fann_create_standard(2,3,5 ,2);
	ann = fann_create_from_file("./ANN.net");
	fann_set_activation_function_hidden(ann, FANN_ELLIOT_SYMMETRIC);	
	fann_set_activation_function_output(ann, FANN_LINEAR);
	fann_set_training_algorithm(ann , FANN_TRAIN_INCREMENTAL);
	fann_set_learning_rate(ann , 0.05);	
	t.start();
	
	nikRastKarde = new ofstream ( "nik_rast_karde" );
}

int getPreIndex(int current , int i)
{
	if(current - i < 0)
	{
		return current - i + 15;
	}
	return current - i;
}

int getPreCMDIndex(int current , int i , int maxCMDIndex)
{
	if(current - i < 0)
	{
		return current - i + maxCMDIndex;
	}
	return current - i; }


int cmdIndex=0;

void VisionModule::RunANN (WorldState * currentState)
{
		
	double input[7];
	fann_type * output;
	double trainData[2];
	
	currentStateIndex++;
	if(currentStateIndex >= 15)
		currentStateIndex=0;
	
	for (int i = 0 ; i < MAX_ROBOTS ; i ++) {
		states[currentStateIndex][i] = robotState[0][i];
	}
	wstates[currentStateIndex] = *currentState;
	
	if (firstRunCount < 30)
	{
		firstRunCount++;
		return;		
	}
	
#if ANN_DEBUG
	cout<<"ANN Output : "<< robotState[0][8].Position.X <<"    "<<robotState[0][8].Position.Y<<endl;
	cout<<"ANN Mid T  : "<< states[currentStateIndex][8].Position.X  <<"    "<<states[currentStateIndex][8].Position.Y <<endl;
	cout<<"ANN Train  : "<< states[currentStateIndex][6].Position.X  <<"    "<<states[currentStateIndex][6].Position.Y <<endl;
#endif
	
	int currentX = states[getPreIndex(currentStateIndex,0)][6].Position.X;
	int currentY = states[getPreIndex(currentStateIndex,0)][6].Position.Y;
	int currentW = states[getPreIndex(currentStateIndex,0)][6].Angle;
	//int currentCMD = states[getPreIndex(currentStateIndex,6)].lastCMDS[6][10].X;
	//int maxCMDIndex = states[getPreIndex(currentStateIndex,6)].lastCMDS[6][10].Y;
	
#if ANN_DEBUG
	cout<<"Command Lists : ";
#endif
	
	for(int i=0;i<1;i++)
	{
		input[7*i + 0] = (states[getPreIndex(currentStateIndex,i+1)][6].Position.X - currentX)/100.0f;
		input[7*i + 1] = (states[getPreIndex(currentStateIndex,i+1)][6].Position.Y - currentY)/100.0f;		
		//input[7*i + 2] = sinDeg(states[getPreIndex(currentStateIndex,i+1)][6].Angle - currentW);
		//input[7*i + 3] = cosDeg(states[getPreIndex(currentStateIndex,i+1)][6].Angle - currentW);		
		//int cmdIndex   = wstates[getPreIndex(currentStateIndex,i+1)].lastCMDS[6][10].X;
		//input[7*i + 4] = wstates[getPreIndex(currentStateIndex,i+1)].lastCMDS[6][cmdIndex].X / 100.0f;
#if ANN_DEBUG
		cout<<states[getPreIndex(currentStateIndex,i)].lastCMDS[6][cmdIndex].X/10.0f<<"   ";
#endif
		//input[7*i + 5] = wstates[getPreIndex(currentStateIndex,i)].lastCMDS[6][cmdIndex].Y / 100.0f;
		//input[7*i + 6] = wstates[getPreIndex(currentStateIndex,i)].lastCMDS[6][cmdIndex].Z / 100.0f;
		/*
		 input[7*i + 4] = states[currentStateIndex].lastCMDS[6][getPreCMDIndex(currentCMD,i+6,maxCMDIndex)].X / 100.0f;
		 input[7*i + 5] = states[currentStateIndex].lastCMDS[6][getPreCMDIndex(currentCMD,i+6,maxCMDIndex)].Y / 100.0f;
		 input[7*i + 6] = states[currentStateIndex].lastCMDS[6][getPreCMDIndex(currentCMD,i+6,maxCMDIndex)].Z / 180.0f;*/
	}
#if ANN_DEBUG
	cout<<endl;
#endif
	
	output = fann_run(ann,input);
	
	//robotState[0][6].Position.X = currentX + output[0]*500;
	//robotState[0][6].Position.Y = currentY +output[1]*500;
	//robotState[0][8].Angle = 0.5 * 3.14;//currentW + output[2]*180;
	//robotState[0][8].seenState = Seen;
	
	robotState[0][6].Position.X += output[0]*100;
	robotState[0][6].Position.Y += output[1]*100;
	
	robotState[0][8].Position.X = robotState[0][6].Position.X;
	robotState[0][8].Position.Y = robotState[0][6].Position.Y;
}

void VisionModule::PredictWithANN (WorldState * currentState)
{
	currentStateIndex++;
	if(currentStateIndex >= 15)
		currentStateIndex=0;
	
	for (int i = 0 ; i < MAX_ROBOTS ; i ++) {
		states[currentStateIndex][i] = robotState[0][i];
	}
	wstates[currentStateIndex] = *currentState;
	
	if (firstRunCount < 30)
	{
		firstRunCount++;
		return;		
	}
	
	return;
}
void VisionModule::TrainANN ( float f)
{
	
	if (firstRunCount < 30)
	{
		firstRunCount++;
		return;		
	}
	
	double input[21];
	fann_type * output;
	double trainData[2];
	
	#if ANN_DEBUG
		cout<<"ANN Output : "<< robotState[0][8].Position.X <<"    "<<robotState[0][8].Position.Y<<endl;
		cout<<"ANN Mid T  : "<< states[currentStateIndex][8].Position.X  <<"    "<<states[currentStateIndex][8].Position.Y <<endl;
		cout<<"ANN Train  : "<< states[currentStateIndex][6].Position.X  <<"    "<<states[currentStateIndex][6].Position.Y <<endl;
	#endif
	
	int currentX = states[getPreIndex(currentStateIndex,6)][6].Position.X;
	int currentY = states[getPreIndex(currentStateIndex,6)][6].Position.Y;
	int currentW = states[getPreIndex(currentStateIndex,6)][6].Angle;
	//int currentCMD = states[getPreIndex(currentStateIndex,6)].lastCMDS[6][10].X;
	//int maxCMDIndex = states[getPreIndex(currentStateIndex,6)].lastCMDS[6][10].Y;
	
	#if ANN_DEBUG
		cout<<"Command Lists : ";
	#endif
	
	for(int i=0;i<1;i++)
	{
		input[7*i + 0] = (states[getPreIndex(currentStateIndex,i+7)][6].Position.X - currentX)/100.0f;
		input[7*i + 1] = (states[getPreIndex(currentStateIndex,i+7)][6].Position.Y - currentY)/100.0f;		
		//input[7*i + 2] = sinDeg(states[getPreIndex(currentStateIndex,i+7)][6].Angle - currentW);
		//input[7*i + 3] = cosDeg(states[getPreIndex(currentStateIndex,i+7)][6].Angle - currentW);		
		int cmdIndex   = wstates[getPreIndex(currentStateIndex,i+7)].lastCMDS[6][10].X;
		//input[7*i + 4] = wstates[getPreIndex(currentStateIndex,i+7)].lastCMDS[6][cmdIndex].X / 10.0f;
		#if ANN_DEBUG
			cout<<states[getPreIndex(currentStateIndex,i+7)].lastCMDS[6][cmdIndex].X/10.0f<<"   ";
		#endif
		//input[7*i + 5] = wstates[getPreIndex(currentStateIndex,i+7)].lastCMDS[6][cmdIndex].Y / 10.0f;
		//input[7*i + 6] = wstates[getPreIndex(currentStateIndex,i+7)].lastCMDS[6][cmdIndex].Z / 10.0f;
		
		*nikRastKarde << input[7*i+0];
		*nikRastKarde << "  ";
		*nikRastKarde << input[7*i+1];
		*nikRastKarde << "  ";
		*nikRastKarde << input[7*i+2];
		*nikRastKarde << "  ";
		*nikRastKarde << input[7*i+3];
		*nikRastKarde << "  ";
		*nikRastKarde << input[7*i+4];
		*nikRastKarde << "  ";
		*nikRastKarde << input[7*i+5];
		*nikRastKarde << "  ";
		*nikRastKarde << input[7*i+6];
		*nikRastKarde << "  ";
		/*
		input[7*i + 4] = states[currentStateIndex].lastCMDS[6][getPreCMDIndex(currentCMD,i+6,maxCMDIndex)].X / 100.0f;
		input[7*i + 5] = states[currentStateIndex].lastCMDS[6][getPreCMDIndex(currentCMD,i+6,maxCMDIndex)].Y / 100.0f;
		input[7*i + 6] = states[currentStateIndex].lastCMDS[6][getPreCMDIndex(currentCMD,i+6,maxCMDIndex)].Z / 180.0f;*/
	}
	*nikRastKarde << endl;
	#if ANN_DEBUG
		cout<<endl;
	#endif
	
	output = fann_run(ann,input);
	
	robotState[0][8].Position.X = currentX + output[0]*100;
	robotState[0][8].Position.Y = currentY +output[1]*100;
	robotState[0][8].Angle = 0.5 * 3.14;//currentW + output[2]*180;
	robotState[0][8].seenState = Seen;
	
	robotState[0][9].Position.X = states[currentStateIndex][6].Position.X;
	robotState[0][9].Position.Y = currentY;//states[currentStateIndex][6].Position.Y;
	robotState[0][9].Angle = 0.5 * 3.14;//currentW + output[2]*180;
	robotState[0][9].seenState = Seen;
	
	trainData[0] = ( states[currentStateIndex][6].Position.X - currentX) / 100.0f;
	trainData[1] = ( states[currentStateIndex][6].Position.Y - currentY)/ 100.0f;
	
	cout<<"Ouputs :  "<<output[0]<<"    "<<output[1]<<endl;
	cout<<"Train  :  "<<trainData[0]<<"    "<<trainData[1]<<endl;
	
	*nikRastKarde << trainData[0] << "  ";
	*nikRastKarde << trainData[1] << endl;
	//trainData[2] = ( states[currentStateIndex].OwnRobot[6].Angle - currentW) / 180.0f; 
	
	
	if (robotState[0][6].seenState==Seen) {
		fann_train(ann,input,trainData);
		
		
	}
	fann_save(ann , "./ANN.net");
	
	//nikRastKarde->close();
}
