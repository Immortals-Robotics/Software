#include <fstream>
#include "../ai09.h"


bool ai09::read_playBook ( const char* fileName )
{	
	ifstream file(fileName,ios::in|ios::binary);
	
	if ( !file.is_open() )
		return false;
	
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
	
	return read_playBook_str(buffer, length);
}

bool ai09::read_playBook_str ( char* buffer , int length )
{
	if ( !playBook )
		playBook = new PlayBook();
	
	if ( !playBook->ParseFromArray(buffer, length) )
	{
		delete playBook;
		playBook = NULL;
		return false;
	}
	
	if ( playBook->strategy_size() != playBook->weight_size() )
	{
		delete playBook;
		playBook = NULL;
		return false;
	}
	
	return true;
}

int step[6] = {0,0,0,0,0,0};
float lastAdv[6] = {0,0,0,0,0,0};
static int curr_str_id = -1;

bool recievers_reached = false;

void ai09::strategy_maker ( void )
{
	int str_id = int(currentPlayParam);
	if ( timer.time() < 0.5 )
	{
		curr_str_id = str_id;
        recievers_reached = false;
	}
	
	if ( curr_str_id == -1 )
	{
		HaltAll();
		return;
	}
	
	Strategy strategy = playBook->strategy(curr_str_id);
    cout << "STRATEGY: " << strategy.name() << endl;
	
	
	
	int xSgn = side;
	int ySgn = -1*sgn(ball.Position.Y);
	
	//cout << timer.time() << endl;
	if ( timer.time() < 1.0 )
	{
		for (int i = 0 ; i < 6 ; i ++ ) {
			step[i] = 0;
			lastAdv[i] = timer.time();
			//cout << "zeroed: " << i << endl;
		}
		float passAngle = 90-side*90;
		//tech_circle(attack, passAngle , 0, 0, 0, 1, 0, 0);
		circle_ball(attack, passAngle, 0, 0, 1.0f);
		return;
	}
	else
	{
		
		for (int i = 0 ; i < strategy.role_size() ; i ++ ) {
			if ( strategy.role(i).path_size() == 0 )
				continue;
			
			if ( step[i] >= strategy.role(i).path_size()-1 )
			{
				step[i] = strategy.role(i).path_size()-1;
				lastAdv[i] = timer.time();
				//cout << "zeroed: " << i << endl;
				continue;
			}
			
			if ( ( strategy.role(i).path(step[i]).type() == 1 ) || ( *stm2AInum[i]==attack ) )
			{
				if ( timer.time()-lastAdv[i] > strategy.role(i).path(step[i]).time() )
				{
					step[i] = min(strategy.role(i).path_size()-1, step[i]+1);
					lastAdv[i] = timer.time();
					//cout << "stepped: " << i << "	" << step[i] << endl;
				}
			}
			else {
				if (DIS(Vec2(strategy.role(i).path(step[i]).x()*xSgn, strategy.role(i).path(step[i]).y()*ySgn), OwnRobot[*stm2AInum[i]].State.Position) < strategy.role(i).path(step[i]).tolerance() ) {
					step[i] = min(strategy.role(i).path_size()-1, step[i]+1);
					lastAdv[i] = timer.time();
					//cout << "stepped: " << i << "	" << step[i] << endl;
				}
			}

		}
	}
	
    bool new_recievers_reached = true;
    
	for (int i = 0 ; i < 6 ; i ++ ) {
		//if ((*stm2AInum[i]==gk)||(*stm2AInum[i]==def)) {
		//	continue;
		//}
		
		if ( strategy.role(i).path_size() == 0 )
		{
            if (*stm2AInum[i]==gk)
                GKHi(gk);
            else if (*stm2AInum[i]==def)
            {
                DefHi(def);
            }
            else
                Halt(*stm2AInum[i]);
			continue;
		}
		
		else if (*stm2AInum[i]==attack) {
			int shoot = 0;
			int chip = 0;
			if (strategy.role(i).path(step[i]).type() == 0 ) {
				shoot = strategy.role(i).path(step[i]).tolerance();
			}
			else {
				chip = strategy.role(i).path(step[i]).tolerance();
			}
			
			//cout << "	daram mirinam: " << shoot << "	" << chip << endl;

            
            
            
			if (step[i]==strategy.role(i).path_size()-1 && recievers_reached) {
				float passAngle = AngleWith(Vec2(strategy.role(i).path(step[i]).x()*xSgn, strategy.role(i).path(step[i]).y()*ySgn),ball.Position);
				circle_ball(*stm2AInum[i], passAngle, shoot, chip, 1.0f);

			}
            else if (step[i]==strategy.role(i).path_size()-2) {
                float passAngle = AngleWith(Vec2(strategy.role(i).path(step[i]).x()*xSgn, strategy.role(i).path(step[i]).y()*ySgn),ball.Position);
                circle_ball(*stm2AInum[i], passAngle, 0, 0, 1.0f,140.0f);
            }
			else {
				float passAngle = AngleWith(Vec2(strategy.role(i).path(step[i]).x()*xSgn, strategy.role(i).path(step[i]).y()*ySgn),ball.Position);
				circle_ball(*stm2AInum[i], passAngle, 0, 0, 1.0f);
			}
			
		}
		
		else {
			if (1){//strategy.role(i).path(step[i]).needrrt()==0) {
				ERRTSetObstacles(*stm2AInum[i], 1, 1, 1, 1, 0,1);
				//ERRTSetObstacles(*stm2AInum[i], 0, 0, 1, 1, 0, 0);
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
            float dis_to_reach = DIS(OwnRobot[*stm2AInum[i]].State.Position, Vec2(strategy.role(i).path(step[i]).x(),strategy.role(i).path(step[i]).y()));
            if ((step[i]>=strategy.role(i).path_size()-2) || (dis_to_reach < 500))
                OwnRobot[*stm2AInum[i]].face(Vec2(-side*field_width, 0));
            else
                OwnRobot[*stm2AInum[i]].face(Vec2(strategy.role(i).path(step[i]).x(),strategy.role(i).path(step[i]).y()));
			ERRTNavigate2Point(*stm2AInum[i], Vec2(strategy.role(i).path(step[i]).x()*xSgn, strategy.role(i).path(step[i]).y()*ySgn), 0, strategy.role(i).path(step[i]).speed() , profile);
		}
		
		switch (strategy.role(i).afterlife()) {
			case 0:
				oneTouchType[*stm2AInum[i]] = gool;
				break;
			case 1:
				oneTouchType[*stm2AInum[i]] = oneTouch;
                if (strategy.role(i).path_size()==0)
                    allafPos[*stm2AInum[i]] = Vec2(0, 0);
                else
                    allafPos[*stm2AInum[i]] = Vec2( strategy.role(i).path(strategy.role(i).path_size()-1).x()*xSgn,strategy.role(i).path(strategy.role(i).path_size()-1).y()*ySgn );
                
                if (step[i]!=strategy.role(i).path_size()-1)
                    new_recievers_reached = false;
				break;
			case 2:
				oneTouchType[*stm2AInum[i]] = shirje;
				break;
			case 3:
				oneTouchType[*stm2AInum[i]] = allaf;
				if (*stm2AInum[i]==attack) {
					allafPos[*stm2AInum[i]] = ball.Position;
				}
				else {
					if (strategy.role(i).path_size()==0)
						allafPos[*stm2AInum[i]] = Vec2(0, 0);
					else
						allafPos[*stm2AInum[i]] = Vec2( strategy.role(i).path(strategy.role(i).path_size()-1).x()*xSgn,strategy.role(i).path(strategy.role(i).path_size()-1).y()*ySgn );
				}
				break;
			default:
				oneTouchType[*stm2AInum[i]] = oneTouch;
				break;
		}
	}
    
    recievers_reached = new_recievers_reached;
	
}