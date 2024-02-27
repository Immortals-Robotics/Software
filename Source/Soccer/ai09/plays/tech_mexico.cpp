#include "../ai09.h"
bool ai09::ballReached(TVec2 dest)
{
	if (DIS(ball.Position.X,ball.Position.Y,dest.X,dest.Y) < 100.f)
	{
		return true;
	}
	return false;
}
void ai09::tech_mexico ( void )
{
	static int state = 0;
	if (state == 0){
		WaitToInterceptBall(state);
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