#include "../ai09.h"

void ai09::NormalPlayDef ( void )
{
	ManageAttRoles();
	
	MarkManager(false);

	for (std::map<int*,int>::const_iterator i=markMap.begin(); i!=markMap.end(); ++i) {
		int opp = i->second;
		int own = *i->first;
		
		if (0) {//oneTouchDetector[own].IsArriving()) {
			WaitForPass(own, false);
		}
		else
		{
		
			if (opp==-1) {
                int oppAttacker = findKickerOpp(-1);

				OwnRobot[own].face(Vec2(-side*field_width, 0));
				ERRTSetObstacles(own, 0, 1, 1, 0);

				if (own==dmf) {
					ERRTNavigate2Point(dmf, PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), 1800), 0, 100, &VELOCITY_PROFILE_MAMOOLI);
				}
				else if (own==mid1) {
                    if (oppAttacker != -1)
                        Mark2Goal(own,oppAttacker, 500);
                    else
						ERRTNavigate2Point(own, Vec2(ball.Position.X, 1000), 0, 100, &VELOCITY_PROFILE_MAMOOLI);
				}
				else if (own==mid2) {
                    if (oppAttacker != -1)
                        Mark2Goal(own,oppAttacker, 500);
                    else
						ERRTNavigate2Point(own, Vec2(ball.Position.X, -1000), 0, 100, &VELOCITY_PROFILE_MAMOOLI);
				}
			}
			else {
				Mark(own, opp, 500);
			}
		}
	}
	
    TVec2 openAngle = calculateOpenAngleToGoal(ball.Position, attack);
    float shootAngle = shootAngle = NormalizeAngle( 180+openAngle.X);
    
	float shoot_pow = 1;
	float chip_pow = 1;
	if (DIS(OwnRobot[attack].State.Position,ball.Position) > 400 ) {
		chip_pow = 1;
	}
	if (goal_blocked(ball.Position, 200, 90)) {
		chip_pow = 50;
	}
	else if (!goal_blocked(ball.Position, 3000, 130)) {
		shoot_pow = 50 - OwnRobot[attack].State.velocity.magnitude * 0.005;;
		chip_pow = 0;
	}
	else
	{
		shoot_pow = 0;
		chip_pow = 10;
	}

	// chip the ball out if in a dangerous position
#if 1
    if (attackFuckingAngle() && findKickerOpp(-1)) {
		shootAngle = AngleWith(ball.Position, Vec2(side*field_width, 0));
		shoot_pow = 1;
		chip_pow = 0;
	}
#endif

    if (0)//findKickerOpp(-1))
    {
        shootAngle = AngleWith(ball.Position, OwnRobot[attack].State.Position);
        shoot_pow = 0;
        chip_pow = 500;
        
        OwnRobot[attack].face(ball.Position);
        ERRTSetObstacles(attack);
        ERRTNavigate2Point(attack, ball.Position,1,100,&VELOCITY_PROFILE_KHARAKI);
        OwnRobot[attack].Chip(500);
    }
    else
    {
        debugDraw = true;
        tech_circle(attack, shootAngle, shoot_pow, chip_pow, 1, 0, 0, 1);
        //circle_ball(attack, 90, 80, 0, 1.0f);
        debugDraw = false;
    }
}