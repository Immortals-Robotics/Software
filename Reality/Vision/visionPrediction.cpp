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
	
	//ann = fann_create_standard(3,42,20 ,2);
	ann = fann_create_from_file("./ANN.net");
	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);	
	fann_set_activation_function_output(ann, FANN_LINEAR_PIECE_SYMMETRIC);
	fann_set_training_algorithm(ann , FANN_TRAIN_INCREMENTAL);
	fann_set_learning_rate(ann , 0.1);	
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


int cmdIndex = 0;

#define predictFrame 7
#define predictFrameTrain 7

void VisionModule::RunANN (WorldState * currentState)
{
	
	double input[42];
	fann_type * output;
	
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
	
	for (int rbt = 0 ; rbt < 7 ; rbt++) 
	{
		int currentX = states[getPreIndex(currentStateIndex,0)][rbt].Position.X;
		int currentY = states[getPreIndex(currentStateIndex,0)][rbt].Position.Y;
		int currentW = states[getPreIndex(currentStateIndex,0)][rbt].Angle;				
	
		for(int i=0;i<6;i++)
		{
			input[7*i + 0] = (states[getPreIndex(currentStateIndex,i+1)][rbt].Position.X - currentX)/500.0f;
			input[7*i + 1] = (states[getPreIndex(currentStateIndex,i+1)][rbt].Position.Y - currentY)/500.0f;		
			input[7*i + 2] = sinDeg(states[getPreIndex(currentStateIndex,i+1)][rbt].Angle - currentW);
			input[7*i + 3] = cosDeg(states[getPreIndex(currentStateIndex,i+1)][rbt].Angle - currentW);		
			int cmdIndex   = wstates[getPreIndex(currentStateIndex,i+1)].lastCMDS[rbt][10].X;
			input[7*i + 4] = wstates[getPreIndex(currentStateIndex,i+1)].lastCMDS[rbt][cmdIndex].X / 100.0f;
			input[7*i + 5] = wstates[getPreIndex(currentStateIndex,i+1)].lastCMDS[rbt][cmdIndex].Y / 100.0f;
			input[7*i + 6] = wstates[getPreIndex(currentStateIndex,i+1)].lastCMDS[rbt][cmdIndex].Z / 100.0f;		
		}	
		output = fann_run(ann,input);
		
		if(rbt==6)
		{
			robotState[0][8].Position.X = robotState[0][rbt].Position.X; 
			robotState[0][8].Position.Y = robotState[0][rbt].Position.Y;
			robotState[0][8].Angle = 0.5 * 3.14;//currentW + output[2]*180;
			robotState[0][8].seenState = Seen;
			cout<<"ANN RUN X : " << output[0]<<endl;
			cout<<"ANN RUN Y : " << output[1]<<endl;
		}
		
		robotState[0][rbt].Position.X = states[getPreIndex(currentStateIndex,0)][rbt].Position.X + output[0]*500;
		robotState[0][rbt].Position.Y = states[getPreIndex(currentStateIndex,0)][rbt].Position.Y + output[1]*500;
		
	
	}
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
float bozoo;
long int counterBoz=1;

void VisionModule::TrainANN ( float f)
{
	
	if (firstRunCount < 30)
	{
		firstRunCount++;
		return;		
	}
	
	double input[42];
	fann_type * output;
	double trainData[2];
	int rbt=6;
	
	if(states[currentStateIndex][rbt].seenState!=Seen)
	{
		//states[currentStateIndex][rbt].Position.X = states[getPreIndex(currentStateIndex,1)][rbt].Position.X;
		//states[currentStateIndex][rbt].Position.Y = states[getPreIndex(currentStateIndex,1)][rbt].Position.Y;		
		return;
	}
	
	#if ANN_DEBUG
		cout<<"ANN Output : "<< robotState[0][8].Position.X <<"    "<<robotState[0][8].Position.Y<<endl;
		cout<<"ANN Mid T  : "<< states[currentStateIndex][8].Position.X  <<"    "<<states[currentStateIndex][8].Position.Y <<endl;
		cout<<"ANN Train  : "<< states[currentStateIndex][6].Position.X  <<"    "<<states[currentStateIndex][6].Position.Y <<endl;
	#endif
	
	int currentX = states[getPreIndex(currentStateIndex,predictFrameTrain)][rbt].Position.X;
	int currentY = states[getPreIndex(currentStateIndex,predictFrameTrain)][rbt].Position.Y;
	int currentW = states[getPreIndex(currentStateIndex,predictFrameTrain)][rbt].Angle;

	for(int i=0;i<6;i++)
	{
		input[7*i + 0] = (states[getPreIndex(currentStateIndex,i+predictFrameTrain)][rbt].Position.X - currentX)/500.0f;
		input[7*i + 1] = (states[getPreIndex(currentStateIndex,i+predictFrameTrain)][rbt].Position.Y - currentY)/500.0f;		
		input[7*i + 2] = sinDeg(states[getPreIndex(currentStateIndex,i+predictFrameTrain)][rbt].Angle - currentW);
		input[7*i + 3] = cosDeg(states[getPreIndex(currentStateIndex,i+predictFrameTrain)][rbt].Angle - currentW);		
		int cmdIndex   = wstates[getPreIndex(currentStateIndex,i+predictFrameTrain)].lastCMDS[rbt][10].X;
		input[7*i + 4] = wstates[getPreIndex(currentStateIndex,i+predictFrameTrain)].lastCMDS[rbt][cmdIndex].X / 100.0f;
		input[7*i + 5] = wstates[getPreIndex(currentStateIndex,i+predictFrameTrain)].lastCMDS[rbt][cmdIndex].Y / 100.0f;
		input[7*i + 6] = wstates[getPreIndex(currentStateIndex,i+predictFrameTrain)].lastCMDS[rbt][cmdIndex].Z / 100.0f;
		
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
		
	}
	*nikRastKarde << endl;
	#if ANN_DEBUG
		cout<<endl;
	#endif
	
	output = fann_run(ann,input);
	
	currentX = states[getPreIndex(currentStateIndex,predictFrameTrain)][rbt].Position.X;
	currentY = states[getPreIndex(currentStateIndex,predictFrameTrain)][rbt].Position.Y;
	currentW = states[getPreIndex(currentStateIndex,predictFrameTrain)][rbt].Angle;
	
	robotState[0][8].Position.X = currentX + output[0]*500;
	robotState[0][8].Position.Y = currentY +output[1]*500;
	robotState[0][8].Angle = 0.5 * 3.14;//currentW + output[2]*180;
	robotState[0][8].seenState = Seen;
	
	robotState[0][9].Position.X = states[currentStateIndex][2].Position.X;
	robotState[0][9].Position.Y = states[currentStateIndex][2].Position.Y;
	robotState[0][9].Angle = 0.5 * 3.14;//currentW + output[2]*180;
	robotState[0][9].seenState = Seen;
	
	trainData[0] = ( states[currentStateIndex][rbt].Position.X - currentX) / 500.0f;
	trainData[1] = ( states[currentStateIndex][rbt].Position.Y - currentY)/ 500.0f;
	
	cout<<"Ouputs :  "<<output[0]<<"    "<<output[1]<<endl;
	cout<<"Train  :  "<<trainData[0]<<"    "<<trainData[1]<<endl;
	
	bozoo = bozoo * counterBoz;
	bozoo += sqrt(pow(output[0]-trainData[0],2) + pow(output[1]-trainData[1],2));
	counterBoz ++;
	bozoo = bozoo / counterBoz;
	
	cout << "ERROR SUM : "<<bozoo<<endl;
	
	*nikRastKarde << trainData[0] << "  ";
	*nikRastKarde << trainData[1] << endl;
	//trainData[2] = ( states[currentStateIndex].OwnRobot[6].Angle - currentW) / 180.0f; 
	
	
	if (robotState[0][rbt].seenState==Seen) 
	{
		fann_train(ann,input,trainData);
	}
	fann_save(ann , "./ANN.net");
	
	//nikRastKarde->close();
}
