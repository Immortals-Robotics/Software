#include "../ai09.h"

void ai09::tech_mexico ( void )
{
	static int state = 0;
	if (state == 0){
		int state = WaitToInterceptBall(state);
	}
	else if(state == 1){
		OwnRobot[attack].Dribble(1);
		OwnRobot[attack].face(Vec2(-side*(field_width),0));
		OwnRobot[attack].Shoot(60);
		state++;
	}
	else if(state == 3)
	{
		ERRTSetObstacles(attack);
		ERRTNavigate2Point(attack,Vec2(0,0));
		state++;
	}
}

int ai09::WaitToInterceptBall(int state) {
	float t = 2;
	float time_needed;
	while (1) {
		auto dest = predictBallForwardAI(t);
		time_needed = calculateRobotReachTime(attack, dest, &VELOCITY_PROFILE_MAMOOLI);
		if (t - 1.2 < time_needed && time_needed <= t) {
			OwnRobot[attack].target.Angle = AngleWith(dest, ball.Position);
			ERRTSetObstacles(attack);
			AddCircle(ball.Position.X, ball.Position.Y, 150);
			ERRTNavigate2Point(attack, dest);
			if (ballReached(dest))
				return state + 1;
		}
		else if (t > time_needed) {
			t -= 1;
		}
		else if (t < time_needed) {
			t += 1;
		}

	}
}
bool ai09::ballReached(TVec2 dest)
{
	if (DIS(ball.Position,dest)< 100.f && ball.velocity.magnitude < 50)
	{
		return true;
	}
	return false;
}