#include "trapezoid.h"
#include "../../Common/distance.h"

inline float sign ( float num )
{
	if ( num >= 0.0f )
		return 1.0f;
	return -1.0f;
}

TrapezoidPlanner::TrapezoidPlanner ( )
{
	init ( Vec2 ( 0.0f ) , Vec2 ( 0.0f ) , Vec2 ( 0.0f ) , 0.0f );
}

TrapezoidPlanner::TrapezoidPlanner ( TVec2 _max_acc , TVec2 _max_dec , TVec2 _max_spd , float _period )
{
	init ( _max_acc , _max_dec , _max_spd , _period );
}

void TrapezoidPlanner::init ( TVec2 _max_acc , TVec2 _max_dec , TVec2 _max_spd , float _period )
{
	init_state = NULL;
	final_state = NULL;

	max_acc = _max_acc;
	max_dec = _max_dec;
	max_spd = _max_spd;

	period = _period;

	oldAns = Vec3 ( 0.0f );
}


float TrapezoidPlanner::Plan1D ( float d , float v , float tmp_max_spd , float tmp_max_acc , float tmp_max_dec )
{
	float ans = 0.0f;
	//If the current velocity has a different sign than the difference in positions, decelerate to a complete stop
	if ( v * d < 0 )
	{
		ans = fabs ( v ) - ( 1.0f * tmp_max_dec * period );
		if ( ans < 0 )
			ans = 0.0f;
		ans *= sign ( v );
	}

	//If the current velocity will overshoot the target, decelerate to a complete stop
	else if ( ( v * v ) / ( 2.0f * tmp_max_dec ) > fabs ( d ) )
	{
		ans = fabs ( v ) - ( tmp_max_dec * period );
		if ( ans < 0 )
			ans = 0.0f;
		ans *= sign ( v );
	}

	//If the current velocity exceeds the maximum, decelerate to the maximum.
	else if ( fabs ( v ) > tmp_max_spd )
	{
		ans = fabs ( v ) - ( tmp_max_dec * period );
		if ( ans < tmp_max_spd )
			ans = tmp_max_spd;
		ans *= sign ( v );
	}

	//Calculate a triangular velocity profile that will close the gap.
	else
	{
		ans = fabs ( v ) + ( tmp_max_acc * period );

		//If the peak of the triangular profile exceeds the maximum speed, calculate a trapezoidal velocity profile.
		if ( ans > tmp_max_spd )
			ans = tmp_max_spd;

		if ( ( ans * ans ) / ( 2.0f * tmp_max_dec ) > fabs ( d ) )
		{
			ans = sqrt ( 2.0f * tmp_max_dec * fabs ( d ) );
		}
		ans *= sign ( d );
	}

	return ans;
}

TVec3 TrapezoidPlanner::Plan (  RobotState * _init , RobotState * _final )
{
	init_state = _init;
	final_state = _final;

	if ( ( init_state == NULL ) || ( final_state == NULL ) )
		return Vec3 ( 0.0f );

	TVec3 ans = Vec3 ( 0.0f );

	ans.Z = init_state->Angle-final_state->Angle;
	while(ans.Z>180)
		ans.Z-=360;
	while(ans.Z<-180)
		ans.Z+=360;
	ans.Z *= 3.0f;
	if ( ans.Z > 180 )
		ans.Z = 180;
	if ( ans.Z < -180 )
		ans.Z = -180;

	if ( fabs ( oldAns.X - init_state->velocity.x ) > 1000.0f )
		oldAns.X = ( oldAns.X + init_state->velocity.x ) / 2.0f;
	if ( fabs ( oldAns.Y - init_state->velocity.y ) > 1000.0f )
		oldAns.Y = ( oldAns.Y + init_state->velocity.y ) / 2.0f;

	float target_dis = DIS ( init_state->Position , final_state->Position );
	TVec2 tmp_max_speed;
	tmp_max_speed.X = max_spd.X * fabs(final_state->Position.X-init_state->Position.X) / target_dis;
	tmp_max_speed.Y = max_spd.Y * fabs(final_state->Position.Y-init_state->Position.Y) / target_dis;

	ans.X = Plan1D ( final_state->Position.X-init_state->Position.X , oldAns.X , tmp_max_speed.X , max_acc.X , max_dec.X );
	ans.Y = Plan1D ( final_state->Position.Y-init_state->Position.Y , oldAns.Y , tmp_max_speed.Y , max_acc.Y , max_dec.Y );

	oldAns = ans;

	ans.X /= 42.5f;
	ans.Y /= 42.5f;

	return ans;

}