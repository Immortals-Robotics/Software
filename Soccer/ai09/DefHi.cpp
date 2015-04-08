#include "ai09.h"

void ai09::DefHi ( int robot_num , TVec2 * defendTarget , bool stop )
{	
	//side = -side;
	if ( !defendTarget )
		defendTarget = &(ball.Position);
	
	
	const float max_def_move_to_intercept = 400.0f;
	const float max_near_asshole_dis = 750.0f;
	const float max_ball_handler_asshole_dis = 400.0f;
	const float max_shoot_blocker_dis = 1500.0f;
	const float shoot_blocker_r = 200.0f;
	const float max_fast_ball_spd = 5000.0f;
	const float max_own_attacker_dis = 400.0f;
	
	bool ballAriving = false;
	bool oneTouchNear = false;
	bool interceptNear = false;
	bool assholeNear = false;
	bool assholeHasBall = false;
	bool oppGoalOpen = false;
	bool ballMovingFast = false;
	bool ownAttackHasBall = false;
	bool ballIsToGoal = false;
	
	TVec2 target = DefGhuz(defendTarget);
	
	ballAriving = oneTouchDetector[robot_num].IsArriving(40,80);
	
	TVec2 oneTouchPos = CalculatePassPos(robot_num,Vec2(-side*field_width, 0),OwnRobot[robot_num].State.Position);
	
	if (DIS(oneTouchPos, target) < max_def_move_to_intercept ) {
		oneTouchNear = true;
	}
	
	if (DIS(ball.Position, target) < max_def_move_to_intercept ) {
		interceptNear = true;
	}
	
	int nearestAsshole = findNearestAsshole(target, -1, true);
	assholeNear = DIS ( OppRobot[nearestAsshole].Position , target ) < max_near_asshole_dis;
	
	int ballHandlerAsshole = findNearestAsshole(ball.Position, -1, true);
	assholeHasBall = DIS ( OppRobot[ballHandlerAsshole].Position , ball.Position ) < max_ball_handler_asshole_dis;
	
	oppGoalOpen = true;
	Line ballGoalLine = Line::makeLineFromTwoPoints(VecPosition(-field_width*side,0),VecPosition(target.X,target.Y));
	for (int i = 0; i < MAX_ROBOTS; i ++) {
		if ( OppRobot[i].seenState == CompletelyOut )
			continue;
		if ( ( fabs ( OppRobot[i].Position.X ) > field_width ) ||
			( fabs ( OppRobot[i].Position.Y ) > field_height ) )
			continue;
		if (DIS(OppRobot[i].Position, target)>max_shoot_blocker_dis)
			continue;
		
		if (ballGoalLine.getDistanceWithPoint(VecPosition(OppRobot[i].Position.X,OppRobot[i].Position.Y))<shoot_blocker_r) {
			oppGoalOpen = false;
			break;
		}
	}
	if (oppGoalOpen) {
		for (int i = 0; i < 6; i ++) {
			if ( OwnRobot[i].State.seenState == CompletelyOut )
				continue;
			if ( ( fabs ( OwnRobot[i].State.Position.X ) > field_width ) ||
				( fabs ( OwnRobot[i].State.Position.Y ) > field_height ) )
				continue;
			if (DIS(OwnRobot[i].State.Position, target)>max_shoot_blocker_dis)
				continue;
			
			if (ballGoalLine.getDistanceWithPoint(VecPosition(OwnRobot[i].State.Position.X,OwnRobot[i].State.Position.Y))<shoot_blocker_r) {
				oppGoalOpen = false;
				break;
			}
		}
	}
	
	ballMovingFast = ball.velocity.magnitude > max_fast_ball_spd;
	
	ownAttackHasBall = DIS ( OwnRobot[attack].State.Position , ball.Position ) < max_own_attacker_dis;
	if (OwnRobot[attack].State.seenState == CompletelyOut) {
		ownAttackHasBall = false;
	}
	
	ballIsToGoal = ballIsGoaling();
	ballIsToGoal = false;
	
	cout << "DefHi:	" << ballAriving << "	" << oneTouchNear << "	" << assholeNear << "	" << assholeHasBall << "	" << ballMovingFast << "	" << ownAttackHasBall << "	" << ballIsToGoal << "	" << 
	oppGoalOpen << endl;
	
	if ( 
		( false ) &&
		( ballAriving ) &&
		( oneTouchNear ) &&
		( !assholeNear ) &&
		( !assholeHasBall ) &&
		( !ballMovingFast ) &&
		( !ownAttackHasBall ) &&
		( !ballIsToGoal )
		)
	{
		WaitForPass(robot_num , !oppGoalOpen );
	}
	else if (
			 ( interceptNear ) &&
			 ( !assholeHasBall ) &&
			 ( !ballMovingFast ) &&
			 ( !ownAttackHasBall ) &&
			 ( !ballIsToGoal ) &&
			 ( !gkIntercepting ) &&
			 ( !stop )
			 )
	{
		ERRTSetObstacles ( robot_num , 0 , 1 , 1 , 0 , 0 );
		//tech_circle(robot_num,sgn(ball.Position.Y)*side*60 ,0,15,false);
		tech_circle(robot_num,AngleWith ( ball.Position , Vec2 ( side * (field_width+110) , 0 ) ) ,0,500,true,0,0,0);
	}
	else {
		ERRTSetObstacles ( robot_num , stop , true , true , false );
		OwnRobot[robot_num].face ( Vec2 ( (*defendTarget).X , (*defendTarget).Y ) );
		ERRTNavigate2Point(robot_num, target, 0, 100, &VELOCITY_PROFILE_MAMOOLI);
	}
	//side = -side;
}