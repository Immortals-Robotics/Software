#include <fstream>
#include "ai09.h"


void ai09::read_playBook ( const char* fileName )
{
	ifstream file(fileName,ios::in|ios::binary);
	
	if ( !file.is_open() )
		return;
	
	// get length of file:
	file.seekg (0, ios::end);
	int length = file.tellg();
	cout << length << " ";
	file.seekg (0, ios::beg);
	
	// allocate memory:
	char* buffer = new char [length];
	
	// read data as a block:
	file.read (buffer,length);
	file.close();
	
	playBook->ParseFromArray(buffer, length);
}

int step[6] = {0,0,0,0,0,0};
float lastAdv[6] = {0,0,0,0,0,0};

void ai09::strategy_maker ( const Strategy& strategy )
{
	GK_Ghuz(gk);
	DefGhuz(def);
	
	int xSgn = side;
	int ySgn = -1*sgn(ball.Position.Y);
	
	cout << timer.time() << endl;
	if ( timer.time() < 0.5 )
	{
		for (int i = 0 ; i < 6 ; i ++ ) {
			step[i] = 0;
			lastAdv[i] = timer.time();
			cout << "zeroed: " << i << endl;
		}
	}
	else
	{
		for (int i = 0 ; i < strategy.role_size() ; i ++ ) {
			if ( strategy.role(i).path_size() == 0 )
				continue;
			
			if ( ( strategy.role(i).path(step[i]).type() == 1 ) || ( *stm2AInum[i]==attack ) )
			{
				if ( timer.time()-lastAdv[i] > strategy.role(i).path(step[i]).time() )
				{
					step[i] = min(strategy.role(i).path_size()-1, step[i]+1);
					lastAdv[i] = timer.time();
					cout << "stepped: " << i << "	" << step[i] << endl;
				}
			}
			else {
				if (DIS(Vec2(strategy.role(i).path(step[i]).x()*xSgn, strategy.role(i).path(step[i]).y()*ySgn), OwnRobot[*stm2AInum[i]].State.Position) < strategy.role(i).path(step[i]).tolerance() ) {
					step[i] = min(strategy.role(i).path_size()-1, step[i]+1);
					lastAdv[i] = timer.time();
					cout << "stepped: " << i << "	" << step[i] << endl;
				}
			}

		}
	}
	
	for (int i = 0 ; i < 6 ; i ++ ) {
		if ( strategy.role(i).path_size() == 0 )
		{
			Halt(*stm2AInum[i]);
			continue;
		}
		
		if ((*stm2AInum[i]==gk)||(*stm2AInum[i]==def)) {
			continue;
		}
		
		if (*stm2AInum[i]==attack) {
			int shoot = 0;
			int chip = 0;
			if (strategy.role(i).path(step[i]).type() == 0 ) {
				shoot = strategy.role(i).path(step[i]).tolerance();
			}
			else {
				chip = strategy.role(i).path(step[i]).tolerance();
			}

			float dt = ( timer.time()-lastAdv[i] );
			float waitTime = 0.3f;
			if (step[i]==strategy.role(i).path_size()-1) {
				/*if (dt<waitTime) {
					tech_circle(*stm2AInum[i], AngleWith(Vec2(strategy.role(i).path(step[i]).x()*xSgn, strategy.role(i).path(step[i]).y()*ySgn),ball.Position), 0, 0, 0, 1, 0, 1);
				}
				else if (dt<waitTime*(strategy.role(i).path(step[i]).type() == 0?0:2)) {
					tech_circle(*stm2AInum[i], AngleWith(Vec2(strategy.role(i).path(step[i]).x()*xSgn, strategy.role(i).path(step[i]).y()*ySgn),ball.Position), 0, 0, 0, 1, 1, 1);
				}
				else{
					tech_circle(*stm2AInum[i], AngleWith(Vec2(strategy.role(i).path(step[i]).x()*xSgn, strategy.role(i).path(step[i]).y()*ySgn),ball.Position), shoot, chip, 0, 1, 0, 0);
				}*/
				float passAngle = AngleWith(Vec2(strategy.role(i).path(step[i]).x()*xSgn, strategy.role(i).path(step[i]).y()*ySgn),ball.Position);
				tech_circle(*stm2AInum[i], passAngle, shoot, chip, 1, 1, 0, 0);
			}
			else {
				tech_circle(*stm2AInum[i], AngleWith(Vec2(strategy.role(i).path(step[i]).x()*xSgn, strategy.role(i).path(step[i]).y()*ySgn),ball.Position), 0, 0, 0, 1, 0, 0);
			}
			
		}
		
		else {
			if (1){//strategy.role(i).path(step[i]).needrrt()==0) {
				ERRTSetObstacles(*stm2AInum[i], 1, 1, 1, 1, 0,1);
			}
			else {
				ERRTSetObstacles(*stm2AInum[i], 1, 1, 0, 0, 0,1);
			}
			
			
			VelocityProfile* profile;
			switch (strategy.role(i).path(step[i]).profile()) {
				case 0:
					profile = &VELOCITY_PROFILE_AROOM;
					break;
				case 1:
					profile = &VELOCITY_PROFILE_MAMOOLI;
					break;
				case 2:
					profile = &VELOCITY_PROFILE_KHARAKI;
					break;
				default:
					profile = &VELOCITY_PROFILE_MAMOOLI;
					break;
			}
			OwnRobot[*stm2AInum[i]].face(Vec2(-side*3025, 0));
			ERRTNavigate2Point(*stm2AInum[i], Vec2(strategy.role(i).path(step[i]).x()*xSgn, strategy.role(i).path(step[i]).y()*ySgn), 0, strategy.role(i).path(step[i]).speed() , profile);
		}
	}
	
}