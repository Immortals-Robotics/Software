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
	float angle = Angle(Vec2(-side*field_width, 0) - ball.Position);

	TVec2 angle_v = Vec2(cosDeg(angle), sinDeg(angle));

	TVec2 P0 = Vec2(1000, 2000);
	TVec2 P2 = ball.Position - angle_v * 60.0f;
	TVec2 P1 = P2 - angle_v * 500.0f;
	float t = timer.time() - 3.0f;

	float len = blen(&P0, &P1, &P2);

	float max_t = 1.5f * len / 2500.0f;

	t /= max_t;
	t = min(t, 1.0f);
	
	if (t < 0.0f)
	{
		OwnRobot[attack].target.Angle = 0;
		OwnRobot[attack].target.velocity.x = 0;
		OwnRobot[attack].target.velocity.y = 0;

		ERRTSetObstacles(attack, false);
		ERRTNavigate2Point(attack, P0, false, 100, &VELOCITY_PROFILE_MAMOOLI);
		return;
	}
	else
	{
		OwnRobot[attack].target.Angle = angle;

		//P0 = OwnRobot[attack].State.Position;
		//t = 0.5f;

		/*t = pow(t, 0.8f);*/

		TVec2 B = P0 * pow(1 - t, 2.0f) + P1 * 2 * (1 - t)*t + P2 * pow(t, 2.0f);
		TVec2 V = (P1 - P0) * 2.0f * (1.0f - t) + (P2 - P1) * 2.0f * t;
		V = Normalize(V) * t * 500.0f;
		
		OwnRobot[attack].target.velocity.x = V.X;
		OwnRobot[attack].target.velocity.y = V.Y;
		
		Navigate2Point(attack, B, false, 100, &VELOCITY_PROFILE_KHARAKI);

		OwnRobot[attack].Shoot(50);

		return;
	}

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
	Navigate2Point(attack, Vec2(2500 + hhh * 1700, -2000));
	Navigate2Point(dmf, Vec2(2500 + hhh * 1700, -1500));

	tetta +=0.1f;

	if ( tetta >= 360 )
		tetta = 0;
	//tetta = 135;
}