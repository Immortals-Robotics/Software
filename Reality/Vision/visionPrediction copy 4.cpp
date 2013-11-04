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
	//ann = fann_create_standard(3,42,42 ,16);
	ann = fann_create_from_file("./ANN.net");
	fann_set_activation_function_hidden(ann, FANN_GAUSSIAN);	
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_training_algorithm(ann , FANN_TRAIN_RPROP);
	fann_set_learning_rate(ann , 0.01);	
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
	return current - i;
}


int cmdIndex=0;

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
	
	for(int i=0;i<6;i++)
	{
		input[7*i + 0] = (states[getPreIndex(currentStateIndex,i+1)][6].Position.X - currentX)/100.0f;
		input[7*i + 1] = (states[getPreIndex(currentStateIndex,i+1)][6].Position.Y - currentY)/100.0f;		
		input[7*i + 2] = sinDeg(states[getPreIndex(currentStateIndex,i+1)][6].Angle - currentW);
		input[7*i + 3] = cosDeg(states[getPreIndex(currentStateIndex,i+1)][6].Angle - currentW);		
		int cmdIndex   = wstates[getPreIndex(currentStateIndex,i+1)].lastCMDS[6][10].X;
		input[7*i + 4] = wstates[getPreIndex(currentStateIndex,i+1)].lastCMDS[6][cmdIndex].X / 100.0f;
#if ANN_DEBUG
		cout<<states[getPreIndex(currentStateIndex,i)].lastCMDS[6][cmdIndex].X/10.0f<<"   ";
#endif
		input[7*i + 5] = wstates[getPreIndex(currentStateIndex,i)].lastCMDS[6][cmdIndex].Y / 100.0f;
		input[7*i + 6] = wstates[getPreIndex(currentStateIndex,i)].lastCMDS[6][cmdIndex].Z / 100.0f;
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
	
	robotState[0][6].Position.X += (output[0] + output[1]*2 + output[2]*4 + output[3]*8 + output[4]*16 + output[5]*32 + output[6]*64);
	robotState[0][6].Position.Y += (output[7] + output[8]*2 + output[9]*4 + output[10]*8 + output[11]*16 + output[12]*32 + output[13]*64);
	
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
	
	double input[42];
	fann_type * output;
	double trainData[16];
	
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
	
	for(int i=0;i<6;i++)
	{
		input[7*i + 0] = (states[getPreIndex(currentStateIndex,i+7)][6].Position.X - currentX)/100.0f;
		input[7*i + 1] = (states[getPreIndex(currentStateIndex,i+7)][6].Position.Y - currentY)/100.0f;		
		input[7*i + 2] = 0;//sinDeg(states[getPreIndex(currentStateIndex,i+7)][6].Angle - currentW);
		input[7*i + 3] = 0;//cosDeg(states[getPreIndex(currentStateIndex,i+7)][6].Angle - currentW);		
		int cmdIndex   = wstates[getPreIndex(currentStateIndex,i+7)].lastCMDS[6][10].X;
		input[7*i + 4] = wstates[getPreIndex(currentStateIndex,i+7)].lastCMDS[6][cmdIndex].X / 100.0f;
		#if ANN_DEBUG
			cout<<states[getPreIndex(currentStateIndex,i+7)].lastCMDS[6][cmdIndex].X/10.0f<<"   ";
		#endif
		input[7*i + 5] = wstates[getPreIndex(currentStateIndex,i+7)].lastCMDS[6][cmdIndex].Y / 100.0f;
		input[7*i + 6] = wstates[getPreIndex(currentStateIndex,i+7)].lastCMDS[6][cmdIndex].Z / 100.0f;
		
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
		
	int deltaX = 0;//(output[1] + output[2]*2 + output[3]*4 + output[4]*8 + output[5]*16 + output[6]*32 + output[7]*64);
	int deltaY = 0;//(output[9] + output[10]*2 + output[11]*4 + output[12]*8 + output[13]*16 + output[14]*32 + output[15]*64);
	
	for(int i=0;i<6;i++)
	{
		cout<<"Input "<<i<<" : ";
		for(int j=0;j<7;j++)
		{
			cout<<input[7*i + j]<<"    ";
		}
		cout<<endl;
	}
	
	for(int i=0;i<7;i++)
	{
		if(output[i+1] > 0.3)
			deltaX += pow(2,i);
		
		if(output[i+9] > 0.3)
			deltaY += pow(2,i);

	}
	
	if(output[0] < 0)
		deltaX = - deltaX;
	if(output[8] < 0)
		deltaY = -deltaY;
	
	cout<<"delta x : "<<deltaX<<endl;
	robotState[0][8].Position.X = currentX + deltaX;
	robotState[0][8].Position.Y = currentY + deltaY;
	robotState[0][8].Angle = 0.5 * 3.14;
	robotState[0][8].seenState = Seen;
	
	robotState[0][9].Position.X = states[currentStateIndex][6].Position.X;
	robotState[0][9].Position.Y = states[currentStateIndex][6].Position.Y;
	robotState[0][9].Angle = 0.5 * 3.14;
	//robotState[0][9].seenState = Seen;
	
	trainData[0]=1;
	trainData[8]=1;
	int x =  ( states[currentStateIndex][6].Position.X - currentX);
	int y =  ( states[currentStateIndex][6].Position.Y - currentY);
	
	if((states[currentStateIndex][6].Position.X - currentX)<0)
	{
		//trainData[0] = -1;
		x= -x;
	}	
	if((states[currentStateIndex][6].Position.Y - currentY)<0)
	{
		//trainData[8] = -1;
		y= -y;
	}
	
	for(int i=0;i<7;i++)
	{
		trainData[i+1] = x %2;
		x = x/2;
		
		trainData[i+9] = y % 2;
		y = y/2;
	}
	
	cout<<"outputs : ";
	for(int i=0;i<8;i++)
	{
		cout<<output[i]<<"  ";
	}
	cout<<endl;
	
	cout<<"trainSet : ";
	for(int i=0;i<8;i++)
	{
		cout<<trainData[i]<<"  ";
	}
	cout<<"X :  "<< ( states[currentStateIndex][6].Position.X - currentX)<<endl;
	
	//cout<<"Ouputs :  "<<output[0]<<"    "<<output[1]<<endl;
//	cout<<"Train  :  "<<trainData[0]<<"    "<<trainData[1]<<endl;
//
	for(int i=0;i<15;i++)
		*nikRastKarde <<(int)trainData[i] << "  ";
	*nikRastKarde << (int)trainData[15] << endl;
	//trainData[2] = ( states[currentStateIndex].OwnRobot[6].Angle - currentW) / 180.0f; 
	
	
	if (robotState[0][6].seenState==Seen) {
		fann_train(ann,input,trainData);
		
		
	}
	fann_save(ann , "./ANN.net");
	
	//nikRastKarde->close();
}
