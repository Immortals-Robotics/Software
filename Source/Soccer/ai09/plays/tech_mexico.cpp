#include "../ai09.h"

float blen(TVec2* p0, TVec2* p1, TVec2* p2)
{
	TVec2 a, b;
	a.X = p0->X - 2 * p1->X + p2->X;
	a.Y = p0->Y - 2 * p1->Y + p2->Y;
	b.X = 2 * p1->X - 2 * p0->X;
	b.Y = 2 * p1->Y - 2 * p0->Y;
	float A = 4 * (a.X*a.X + a.Y*a.Y);
	float B = 4 * (a.X*b.X + a.Y*b.Y);
	float C = b.X*b.X + b.Y*b.Y;

	float Sabc = 2 * sqrt(A + B + C);
	float A_2 = sqrt(A);
	float A_32 = 2 * A*A_2;
	float C_2 = 2 * sqrt(C);
	float BA = B / A_2;

	return (A_32*Sabc +
		A_2*B*(Sabc - C_2) +
		(4 * C*A - B*B)*log((2 * A_2 + BA + Sabc) / (BA + C_2))
		) / (4 * A_32);
};

void ai09::tech_mexico ( void )
{
#if 0
	if (timer.time() < 3.0f)
	{
		OwnRobot[attack].target.Angle = 0;
		OwnRobot[attack].target.velocity.x = 0;
		OwnRobot[attack].target.velocity.y = 0;

		ERRTSetObstacles(attack, true);
		ERRTNavigate2Point(attack, Vec2(1000, 2000), false, 100, &VELOCITY_PROFILE_MAMOOLI);
		return;
	}
	else
	{
		float angle = Angle(Vec2(-side*field_width, 0) - ball.Position);

		TVec2 angle_v = Vec2(cosDeg(angle), sinDeg(angle));

		float d_angle =
			fabs(NormalizeAngle(Angle(ball.Position - OwnRobot[attack].State.Position) - angle));

		TVec2 P0 = OwnRobot[attack].State.Position;
		TVec2 P2 = ball.Position + angle_v * 00.0f;
		TVec2 P1 = P2 - angle_v * 500.0f;

		float len = blen(&P0, &P1, &P2);

		float t = 500.0f / len;
		t = min(1.0f, max(0.0f, t));

		cout << "d_angle: " << d_angle << endl;

		OwnRobot[attack].target.Angle = angle;

		//P0 = OwnRobot[attack].State.Position;
		//t = 0.5f;

		/*t = pow(t, 0.8f);*/

		TVec2 B = P0 * pow(1 - t, 2.0f) + P1 * 2 * (1 - t)*t + P2 * pow(t, 2.0f);

		TVec2 V = (P1 - P0) * 2.0f * (1.0f - t) + (P2 - P1) * 2.0f * t;

		if (d_angle < 20)
			V = ball.Position - OwnRobot[attack].State.Position;

		V = Normalize(V) * 100.0f;

		OwnRobot[attack].target.velocity.x = V.X;
		OwnRobot[attack].target.velocity.y = V.Y;
		
		Navigate2Point(attack, B, false, 100, &VELOCITY_PROFILE_KHARAKI);

		OwnRobot[attack].Shoot(50);

		return;
	}
#elif 1
	float tetta = timer.time() * 0.5f;

	float hhh = sin(tetta);
	float ggg = cos(tetta);

	if ( ggg < 0 )
		ggg = -1;
	else
		ggg = 1;

	if ( hhh < 0 )
		hhh = -1;
	else
		hhh = 1;

	OwnRobot[attack].target.Angle = 0;
	OwnRobot[dmf].target.Angle = 0;

	for (int i = 0; i < 6; i ++)
	{
		OwnRobot[i].target.Angle = 90;
		Navigate2Point(i, Vec2((i + 1) * 450, 2800 * hhh));
	}

	/*Navigate2Point(attack, Vec2(2200 + hhh * 2000, -2000));
	Navigate2Point(dmf, Vec2(2200 + hhh * 2000, -1500));*/

	

	tetta +=0.1f;

	if ( tetta >= 360 )
		tetta = 0;
	//tetta = 135;
#elif 0
	recievePass(attack, Vec2(500, -1500));
#elif 0
	circle_ball(attack, AngleWith(Vec2(-side*field_width, 0), ball.Position), 35, 0, 1.0f);
#endif
}