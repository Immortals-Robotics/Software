#include "ai09.h"

void ai09::recievePass ( int robot_num , TVec2 staticPos , bool chip )
{
    const float contStrStaticTime = 1.0f;
    
	//oneTouchType[robot_num] = oneTouch;
	if (timer.time()>0.7) {
		chip_head = 200;
	}
	if (oneTouchType[robot_num] == allaf )
		staticPos = allafPos[robot_num];
    if (oneTouchType[robot_num] == oneTouch && timer.time()<contStrStaticTime)
        staticPos = allafPos[robot_num];
	
	if (timer.time()>2.5) {
		oneTouchTypeUsed[robot_num] = true;
	}
	
	float angleTol, maxBallAngle;
	
	if (oneTouchType[robot_num] == oneTouch) {
		angleTol = 45;
		maxBallAngle = 150;
	}
	else if (oneTouchType[robot_num] == shirje) {
		//angleTol = 55;
		//maxBallAngle = 140;
		angleTol = 45;
		maxBallAngle = 90;
	}
	else if (oneTouchType[robot_num] == gool) {
		angleTol = 35;
		maxBallAngle = 110;
	}
	else{ //probably allaf!!!
		angleTol = 0;
		maxBallAngle = 0;
	}
	
	float distCoeff = DIS(ball.Position, OwnRobot[robot_num].State.Position) / 1500.0f;
	distCoeff = max ( 0.8 , distCoeff );
	distCoeff = min ( 1.2 , distCoeff );
	//angleTol *= distCoeff;
	if (0)//(oneTouchType[robot_num] == shirje)&&(oneTouchDetector[robot_num].IsArriving(OwnRobot[dmf].State.Position,angleTol, maxBallAngle)))
	{
		WaitForPass(robot_num, 0, &OwnRobot[attack].State.Position);
	}
	/*else if ((oneTouchType[robot_num] == shirje)&&(DIS(ball.Position, OwnRobot[robot_num].State.Position)<450)&&(timer.time()<4.0))
	{
		int targetRobotNum = (robot_num==lmf)?rmf:lmf;
		float passAngle = AngleWith ( Vec2 ( -side*2500 , -sgn ( ball.Position.Y ) * 1700 ) , ball.Position );
		//float passAngle = AngleWith ( OwnRobot[targetRobotNum].State.Position , ball.Position );
		//tech_circle(robot_num, passAngle, 0, 120, 1, 1, 0, 1);
		circle_ball(robot_num, passAngle, 0, 80, 1.0f);
	}*/
			 
	else if ((oneTouchDetector[robot_num].IsArriving(angleTol, maxBallAngle))&&(oneTouchType[robot_num] != allaf)) {
		oneTouchTypeUsed[robot_num] = true;
		if (oneTouchType[robot_num] == oneTouch) {
            if (timer.time()<contStrStaticTime)
                WaitForPass(robot_num, chip,NULL,&staticPos);
            else
                WaitForPass(robot_num, chip);
		}
		else if (oneTouchType[robot_num] == shirje) {
			WaitForOmghi(robot_num,chip);
			//WaitForPass(robot_num, 0, &OwnRobot[dmf].State.Position);
		}
		else if (oneTouchType[robot_num] == gool) {
			WaitForGool(robot_num,chip);
		}
		else{ //probably allaf!!!
			ERRTSetObstacles ( robot_num , false , true , true , true );
			OwnRobot[robot_num].face(Vec2(-side*field_width, 0));
			//OwnRobot[robot_num].target.Angle=-90;
			ERRTNavigate2Point ( robot_num , staticPos ,0 , 100,&VELOCITY_PROFILE_MAMOOLI);
		}
	}
	else {
		if (oneTouchTypeUsed[robot_num]) {
			oneTouchTypeUsed[robot_num] = false;
			oneTouchType[robot_num] = oneTouch;
		}
		ERRTSetObstacles ( robot_num , true , true , true , true );
		OwnRobot[robot_num].face(Vec2(-side*field_width, 0));
		//OwnRobot[robot_num].target.Angle=-90;
		ERRTNavigate2Point ( robot_num , staticPos ,0 , 100,&VELOCITY_PROFILE_MAMOOLI);
	}
}
