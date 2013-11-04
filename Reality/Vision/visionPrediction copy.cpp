#include "vision.h"
#include "ANN.h"
#include "Geom.h"
#include "timer.h"

ANN * neural[MAX_ROBOTS];
WorldState states [15];
int currentStateIndex=0;
int runCounter = 0;
Timer t;

void VisionModule::InitANN ( void )
{
	for(int i=0;i<MAX_ROBOTS;i++)
	{
		neural[i] = new ANN(42,3,3,45);
	}
	t.start();
}

int getPreIndex(int current , int i)
{
	if(current - i < 0)
	{
		return current - i + 13;
	}
	return current - i;
}

int cmdIndex=0;

void VisionModule::RunANN ( WorldState * currentState)
{
	if(runCounter < 30)
		runCounter++;

	currentStateIndex++;
	if(currentStateIndex==14)
		currentStateIndex=0;
	
	states[currentStateIndex] = *currentState;

	if(runCounter < 20)
		return;
	
	
	for(int robotNum = 0;robotNum<MAX_ROBOTS;robotNum++)
	{
		if(currentState->OwnRobot[robotNum].seenState != CompletelyOut)
		{
				neural[robotNum]->inputs[0] = states[currentStateIndex].OwnRobot[robotNum].Position.X;
				neural[robotNum]->inputs[1] = states[currentStateIndex].OwnRobot[robotNum].Position.Y;
				neural[robotNum]->inputs[2] = sinDeg(states[currentStateIndex].OwnRobot[robotNum].Angle);
				neural[robotNum]->inputs[3] = cosDeg(states[currentStateIndex].OwnRobot[robotNum].Angle);
				cmdIndex = states[currentStateIndex].lastCMDS[robotNum][10].X;			
				neural[robotNum]->inputs[4] = states[currentStateIndex].lastCMDS[robotNum][cmdIndex].X;
				neural[robotNum]->inputs[5] = states[currentStateIndex].lastCMDS[robotNum][cmdIndex].Y;
				neural[robotNum]->inputs[6] = states[currentStateIndex].lastCMDS[robotNum][cmdIndex].Z;
				neural[robotNum]->run(neural[robotNum]->inputs);
		}
	}
	
	return;

	for(int robotNum = 0;robotNum<MAX_ROBOTS;robotNum++)
	{
		if(currentState->OwnRobot[robotNum].seenState != CompletelyOut)
		{
			for(int frame = 6;frame>0;frame--)
			{
				neural[robotNum]->inputs[(6 - frame)*7 + 0] = states[getPreIndex(currentStateIndex,frame)].OwnRobot[robotNum].Position.X;
				neural[robotNum]->inputs[(6 - frame)*7 + 1] = states[getPreIndex(currentStateIndex,frame)].OwnRobot[robotNum].Position.Y;
				neural[robotNum]->inputs[(6 - frame)*7 + 2] = sinDeg(states[getPreIndex(currentStateIndex,frame)].OwnRobot[robotNum].Angle);
				neural[robotNum]->inputs[(6 - frame)*7 + 3] = cosDeg(states[getPreIndex(currentStateIndex,frame)].OwnRobot[robotNum].Angle);
				if(states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][10].X - frame < 0){					
					cmdIndex = states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][10].X - frame + states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][10].Y;
				}
				else{
					cmdIndex = states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][10].X - frame;
				}

				neural[robotNum]->inputs[(6 - frame)*7 + 4] = states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][cmdIndex].X;
				neural[robotNum]->inputs[(6 - frame)*7 + 5] = states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][cmdIndex].Y;
				neural[robotNum]->inputs[(6 - frame)*7 + 6] = states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][cmdIndex].Z;
			}
			neural[robotNum]->run(neural[robotNum]->inputs);
		}
	}
}

void VisionModule::PredictWithANN ( WorldState * currentState)
{
	
	RunANN(currentState);
	robotState[0][8].Position.X = (neural[6]->outputs[0] - 0.5)*6050;
	robotState[0][8].Position.Y = (neural[6]->outputs[1] - 0.5)*4050;
	robotState[0][8].Angle = (neural[6]->outputs[2] - 0.5)*360;
	robotState[0][8].seenState = Seen;
	
	return ;
	
	for(int robotNum = 0;robotNum<MAX_ROBOTS;robotNum)
	{
		if(currentState->OwnRobot[robotNum].seenState != CompletelyOut)
		{
			currentState->OwnRobot[robotNum].Position.X = neural[robotNum]->outputs[0];
			currentState->OwnRobot[robotNum].Position.Y = neural[robotNum]->outputs[1];
			//currentState->OwnRobot[robotNum].Angle = neural[robotNum]->outputs[2];
		}
	}
	
}
void VisionModule::TrainANN ( float f)
{
	if(t.time() > 20)
	{
		cout<<"Learn End..."<<endl;
		return;
	}
	
	for(int robotNum = 0;robotNum<MAX_ROBOTS;robotNum++)
	{
		if(states[currentStateIndex].OwnRobot[robotNum].seenState != CompletelyOut)
		{
			for(int frame = 13;frame>7;frame--)
			{
				neural[robotNum]->inputs[(13 - frame)*7 + 0] = states[getPreIndex(currentStateIndex,frame)].OwnRobot[robotNum].Position.X;
				neural[robotNum]->inputs[(13 - frame)*7 + 1] = states[getPreIndex(currentStateIndex,frame)].OwnRobot[robotNum].Position.Y;
				neural[robotNum]->inputs[(13 - frame)*7 + 2] = sinDeg(states[getPreIndex(currentStateIndex,frame)].OwnRobot[robotNum].Angle);
				neural[robotNum]->inputs[(13 - frame)*7 + 3] = cosDeg(states[getPreIndex(currentStateIndex,frame)].OwnRobot[robotNum].Angle);
				if(states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][10].X - frame < 0){					
					cmdIndex = states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][10].X - frame + states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][10].Y;
				}
				else{
					cmdIndex = states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][10].X - frame;
				}
				
				neural[robotNum]->inputs[(13 - frame)*7 + 4] = states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][cmdIndex].X;
				neural[robotNum]->inputs[(13 - frame)*7 + 5] = states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][cmdIndex].Y;
				neural[robotNum]->inputs[(13 - frame)*7 + 6] = states[getPreIndex(currentStateIndex,frame)].lastCMDS[robotNum][cmdIndex].Z;
			}
			neural[robotNum]->run(neural[robotNum]->inputs);
		}
	}
		
	for(int robotNum = 0;robotNum<MAX_ROBOTS;robotNum++)
	{
		if(states[currentStateIndex].OwnRobot[robotNum].seenState != CompletelyOut)
		{
			neural[robotNum]->trainSet[0] = (states[currentStateIndex].OwnRobot[robotNum].Position.X + 3025)/6050.0;
			neural[robotNum]->trainSet[1] = (states[currentStateIndex].OwnRobot[robotNum].Position.Y +2025)/4050.0;
			neural[robotNum]->trainSet[2] = (states[currentStateIndex].OwnRobot[robotNum].Angle +180 ) /360.0;
			cout<<"Position X Robot Num "<<robotNum<<" : "<<states[currentStateIndex].OwnRobot[robotNum].Position.X;
			cout<<"       "<<(neural[robotNum]->outputs[0] - 0.5)*6050;
			cout<<"    "<<(neural[robotNum]->outputs[0]-(states[currentStateIndex].OwnRobot[robotNum].Position.X + 3025)/6050.0)<<endl;
			cout<<"Position Y Robot Num "<<robotNum<<" : "<<states[currentStateIndex].OwnRobot[robotNum].Position.Y;
			cout<<"       "<<(neural[robotNum]->outputs[1] - 0.5)*4050;
			cout<<"     "<<(neural[robotNum]->outputs[1]-(states[currentStateIndex].OwnRobot[robotNum].Position.Y + 2025)/4050.0)<<endl;
			cout<<"Position Angel Robot Num "<<robotNum<<" : "<<states[currentStateIndex].OwnRobot[robotNum].Angle;
			cout<<"       "<<(neural[robotNum]->outputs[2] - 0.5)*360;
			cout<<"     "<<(neural[robotNum]->outputs[2]-(states[currentStateIndex].OwnRobot[robotNum].Angle + 180)/360.0)<<endl;
			neural[robotNum]->backprop(f);
		}
	}
}
