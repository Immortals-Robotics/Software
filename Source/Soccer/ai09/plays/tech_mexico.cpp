#include "../ai09.h"

void ai09::tech_mexico ( void )
{
	static int state = 0;
	TowarDestination(Vec2(0,0));

//	if (timer.time() < 0.5)
//	{
//		state = 0;
//		return;
//	}
//
//
//	if (state == 0){
//		state = WaitToInterceptBall(state);
//	}
//	else if(state == 1){
//		Halt(attack);
//		return;
//		tech_circle(attack, AngleWith(Vec2(4500, 0), ball.Position), 40);
//		if(ball.velocity.magnitude > 7000)
//			state++;
//	}
//	else if(state == 2)
//	{
//		ERRTSetObstacles(attack);
//		ERRTNavigate2Point(attack,Vec2(0,0));
//		if(DIS(OwnRobot[attack].State.Position,Vec2(0)) < 50 )
//			state = 0 ;
//	}
}

int ai09::WaitToInterceptBall(int state) {
	float t = 2;
	float time_needed;
	int x = 400;
	while (x > 0) {
		auto dest = predictBallForwardAI(t);
		time_needed = calculateRobotReachTime(attack, dest, &VELOCITY_PROFILE_MAMOOLI);
		if (t - 1.2 < time_needed && time_needed <= t) {
			if(DIS(OwnRobot[attack].State.Position,dest) > 500)
			{
				OwnRobot[attack].target.Angle = AngleWith(dest, ball.Position);
				ERRTSetObstacles(attack);
				AddCircle(ball.Position.X, ball.Position.Y, 150);
				ERRTNavigate2Point(attack, dest);
			}
			else
			{
				OwnRobot[attack].Dribble(1);
				OwnRobot[attack].face(ball.Position);
				Navigate2Point(attack,ball.Position);
			}
			if (ballReached())
				return state + 1;
			else
				return state;
		}
		else if (t > time_needed) {
			t -= 0.5;
		}
		else if (t < time_needed) {
			t += 0.5;
		}
	x--;
	}
}
bool ai09::ballReached()
{
	if (DIS(ball.Position,OwnRobot[attack].State.Position)< 200.f && ball.velocity.magnitude < 200)
	{
		return true;
	}
	return false;
}
void ai09::TowarDestination(TVec2 dest)
{
	static int state;
	if (timer.time() < 0.5)
	{
		state = 0;
		return;
	}
	if (state == 0)
	{

		if (fabs(NormalizeAngle(
				AngleWith(OwnRobot[attack].State.Position, ball.Position) - OwnRobot[attack].State.Angle)) < 10 &&
			DIS(OwnRobot[attack].State.Position, ball.Position) < 100)
		{
			state = 1;
		}
		else
		{
			OwnRobot[attack].face(ball.Position);
			Navigate2Point(attack, ball.Position);
			OwnRobot[attack].Dribble(1);
		}
	}
	if (state == 1)
	{
		state = MoveToDest(dest,state);
	}
	if (state == 2)
	{
		Navigate2Point(attack, dest);
		OwnRobot[attack].Dribble(1);
		/*if (DIS(OwnRobot[attack].State.Position, Vec2(0)) < 100)
		{
			state = 0;
			Halt(attack);
		}*/
	}
}
int ai09::MoveToDest(TVec2 dest,int state)
{
	OwnRobot[attack].Dribble(1);

	if(fabs(NormalizeAngle(AngleWith(OwnRobot[attack].State.Position,dest) - AngleWith(OwnRobot[attack].State.Position,ball.Position))) > 30)
	{
		cout << "Angle" << OwnRobot[attack].State.Angle << endl;
		auto target_angle = NormalizeAngle(OwnRobot[attack].State.Angle + 20.f);
		Navigate2Point(attack, OwnRobot[attack].State.Position + Vec2(sinDeg(target_angle)*1500,cosDeg(target_angle)+1500));
		OwnRobot[attack].target.Angle = target_angle;
	}
	else if(fabs(NormalizeAngle(AngleWith(OwnRobot[attack].State.Position,dest) - AngleWith(OwnRobot[attack].State.Position,ball.Position))) < 15)
	{
		state = 2;
	}

	if (fabs(NormalizeAngle(AngleWith(OwnRobot[attack].State.Position, ball.Position) - OwnRobot[attack].State.Angle)) > 15 ||
		DIS(OwnRobot[attack].State.Position, ball.Position) > 150)
		state = 0;
	return state;
}