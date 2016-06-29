#include "Robot.h"
#include "../../Common/distance.h"
#include "helpers.h"
#include <iostream>
using namespace std;


TVec2 vdes_ann;

/*TVec2 max_spd = Vec2 ( 100.0f );
 TVec2 max_dec = Vec2 ( 8.5f );
 TVec2 max_acc = Vec2 ( 3.0f );
 float max_w_acc = 30.0f;
 float max_w_dec = 70.0f;*/

TVec3 Robot::MotionPlan ( RobotState state , RobotState target , float speed , bool accurate , TVec3 * cmd , VelocityProfile * velocityProfile )
{
	/*TVec2 max_spd = Vec2 ( 100.0f );
	TVec2 max_dec = Vec2 ( 2.3f );
	TVec2 max_acc = Vec2 ( 1.6f );
	float max_w_acc = 40.0f;
	float max_w_dec = 140.0f;*/

	TVec2 max_spd = velocityProfile->max_spd;
	TVec2 max_dec = velocityProfile->max_dec;
	TVec2 max_acc = velocityProfile->max_acc;
	float max_w_acc = velocityProfile->max_w_acc;
	float max_w_dec = velocityProfile->max_w_dec;

	if ( oldRobot )
	{
		max_spd = Vec2 ( 100.0f );
		max_dec = Vec2 ( 5.2f );
		max_acc = Vec2 ( 2.6f );
		max_w_acc = 15.0f;
		max_w_dec = 200.0f;
	}

	static TVec3 oldAns[12] = {Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f)};

	TVec3 ans;
	ans.Z = target.Angle;
	while(ans.Z>180)
	{
		ans.Z-=360;
	}
	while(ans.Z<-180)
	{
		ans.Z+=360;
	}
	ans.Z /= 5.0f;
	if ( oldRobot )
	{

	//ans.Z *= 3.0f;

	ans.Z = sqrt ( 2.0f * max_w_dec * fabs( ans.Z ) ) * sgn ( ans.Z );

	 //ans.X += Vel_offset.X;
	 if ( ans.Z * oldAns[state.vision_id].Z < 0 )
	 {
	 float tmp = oldAns[state.vision_id].Z + max_w_dec * sgn ( ans.Z );
	 //float tmp = oldAns[state.vision_id].X + 20.0f * max_acc.X * sgn ( ans.X );

	 if ( tmp * ans.Z > 0 )
	 {
	 tmp = max_w_acc * sgn ( ans.Z );
	 if ( fabs ( tmp ) > fabs ( ans.Z ) )
	 tmp = ans.Z;
	 }

	 ans.Z = tmp;
	 }
	 else
	 {
	 if ( fabs ( ans.Z ) > fabs ( oldAns[state.vision_id].Z ) + max_w_acc )
	 {
	 ans.Z = ( fabs ( oldAns[state.vision_id].Z ) + max_w_acc ) * sgn ( ans.Z );
	 }
	 }
	 if ( fabs ( ans.Z ) > 180 )
	 {
	 ans.Z = 180.0f * sgn ( ans.Z );
	 }

	 if ( ans.Z > 180 )
	 ans.Z = 180;
	 if ( ans.Z < -180 )
	 ans.Z = -180;
	}

	target.Position = target.Position - state.Position;

	/*float trans_rad = ( 90.0f - state.Angle ) * ( 3.1415f / 180.0f );
	 target.Position = Vec2 (
	 cos(trans_rad)*target.Position.X - sin(trans_rad)*target.Position.Y ,
	 sin(trans_rad)*target.Position.X + cos(trans_rad)*target.Position.Y );*/

	TVec2 tmp_max_speed;

	/*if ( fabs(target.Position.X) > fabs(target.Position.Y) )
	 {
	 tmp_max_speed.X = max_spd.X;
	 tmp_max_speed.Y = max_spd.X * fabs(target.Position.Y)/fabs(target.Position.X);

	 tmp_max_speed.Y = max ( 0 , min ( tmp_max_speed.Y , max_spd.Y ) );
	 }
	 else
	 {
	 tmp_max_speed.Y = max_spd.Y;
	 tmp_max_speed.X = max_spd.Y * fabs(target.Position.X)/fabs(target.Position.Y);

	 tmp_max_speed.X = max ( 0 , min ( tmp_max_speed.X , max_spd.X ) );
	 }*/

	float target_dis = DIS ( Vec2 ( 0.0f ) , target.Position );
	tmp_max_speed.X = max_spd.X * fabs(target.Position.X) / target_dis;
	//max_acc.X *= fabs(target.Position.X) / target_dis;
	//max_dec.X *= fabs(target.Position.X) / target_dis;

	tmp_max_speed.Y = max_spd.Y * fabs(target.Position.Y) / target_dis;
	//max_acc.Y *= fabs(target.Position.Y) / target_dis;
	//max_dec.Y *= fabs(target.Position.Y) / target_dis;

	float tmp_vel_mag = DIS ( Vec2(0.0f) , tmp_max_speed );

	if ( tmp_vel_mag > speed )
	{
		tmp_max_speed.X *= speed / tmp_vel_mag;
		tmp_max_speed.Y *= speed / tmp_vel_mag;
	}

	//tmp_max_speed = max_spd;

	/*if ( fabs ( target.velocity.x ) > 100 )
	 target.velocity.x = 100.0f * sgn ( target.velocity.x );
	 if ( fabs ( target.velocity.y ) > 100 )
	 target.velocity.y = 100.0f * sgn ( target.velocity.y );

	 TVec2 Vel_offset = Vec2 ( 0.0f );
	 float target_dis = DIS ( Vec2 ( 0.0f ) , target.Position );
	 target_dis = sqrt ( target_dis * 5.0f * DIS ( Vec2 ( 0.0f ) , max_dec ) );

	 if ( target_dis < 100 )
	 {
	 target_dis = 100.0f - target_dis;
	 target_dis /= 100.0f;

	 Vel_offset.X = target_dis * target.velocity.x;
	 Vel_offset.Y = target_dis * target.velocity.y;
	 }*/

	TVec2 LocalVel;
	/*LocalVel.X = cos(trans_rad)*state.velocity.x - sin(trans_rad)*state.velocity.y;
	 LocalVel.Y = sin(trans_rad)*state.velocity.x + cos(trans_rad)*state.velocity.y;*/
	LocalVel = Vec2 ( state.velocity.x , state.velocity.y );

	LocalVel.X = ( LocalVel.X ) / 45.0f;
	LocalVel.Y = ( LocalVel.Y ) / 45.0f;

	/*if ( fabs ( oldAns[state.vision_id].X - LocalVel.X ) > 40.0f )
		oldAns[state.vision_id].X = ( oldAns[state.vision_id].X + LocalVel.X ) / 2.0f;
	if ( fabs ( oldAns[state.vision_id].Y - LocalVel.Y ) > 40.0f )
		oldAns[state.vision_id].Y = ( oldAns[state.vision_id].Y + LocalVel.Y ) / 2.0f;*/

	float KP = 0.10;
	float Pdis = 0;

	//if ( accurate )
	ans.X = speed;
	//else
	if (fabs( target.Position.X ) < Pdis) {
		ans.X = KP * max_dec.X * fabs( target.Position.X );
	}
	else {
		ans.X = pow ( 0.6f * max_dec.X * fabs( target.Position.X ) , 0.6f );
	}
	ans.X *= sgn ( target.Position.X );
	//ans.X += Vel_offset.X;
	if ( fabs( target.Position.X ) < 5 )
		ans.X = 0.0f;
	if ( ans.X * oldAns[state.vision_id].X <= 0 )
	{
		float tmp = oldAns[state.vision_id].X + max_dec.X * sgn ( ans.X );
		if ( ans.X == 0 )
			tmp = oldAns[state.vision_id].X - max_dec.X * sgn ( oldAns[state.vision_id].X );

		//float tmp = oldAns[state.vision_id].X + 20.0f * max_acc.X * sgn ( ans.X );

		if ( tmp * ans.X > 0 )
		{
			tmp = min(max_acc.X,fabs(tmp)) * sgn ( ans.X );
			if ( fabs ( tmp ) > fabs ( ans.X ) )
				tmp = ans.X;
		}

		ans.X = tmp;
	}
	else
	{
		/*if ( !accurate )
		 {
		 if ( 30.0f * oldAns[state.vision_id].X * oldAns[state.vision_id].X / ( 120.0f * max_dec.X ) > fabs(target.Position.X) )
		 {
		 ans.X = max ( 0 , ( fabs ( oldAns[state.vision_id].X ) - max_dec.X ) ) * sgn ( ans.X );
		 }
		 }*/
		if ( fabs ( ans.X ) > fabs ( oldAns[state.vision_id].X ) + max_acc.X )
		{
			ans.X = ( fabs ( oldAns[state.vision_id].X ) + max_acc.X ) * sgn ( ans.X );
		}
		else if ( fabs ( ans.X ) < fabs ( oldAns[state.vision_id].X ) - max_dec.X )
		{
			ans.X = ( fabs ( oldAns[state.vision_id].X ) - max_dec.X ) * sgn ( ans.X );
		}
		if ( fabs ( ans.X ) > tmp_max_speed.X )
		{
			if ( sgn(ans.X) == 0 )
				ans.X = max(fabs(oldAns[state.vision_id].X)-max_dec.X,fabs(tmp_max_speed.X))*sgn(oldAns[state.vision_id].X);
			else
				ans.X = max(fabs(oldAns[state.vision_id].X)-max_dec.X,fabs(tmp_max_speed.X))*sgn(ans.X);
			//ans.X = tmp_max_speed.X * sgn ( ans.X );
		}
	}


	//if ( accurate )
	ans.Y = speed;
	//else
	if (fabs( target.Position.Y ) < Pdis) {
		ans.Y = KP * max_dec.Y * fabs( target.Position.Y );
	}
	else {
		ans.Y = pow ( 0.6f * max_dec.Y * fabs( target.Position.Y ) , 0.6f );
	}
	//ans.Y = sqrt ( 2.0f * max_dec.Y * fabs( target.Position.Y ) );
	ans.Y *= sgn ( target.Position.Y );
	//ans.Y += Vel_offset.Y;
	if ( fabs( target.Position.Y ) < 5 )
	{
		ans.Y = 0;//max(0,fabs(oldAns[state.vision_id].Y)-max_dec.Y)*sgn(ans.Y);
	}
	if ( ans.Y * oldAns[state.vision_id].Y <= 0 )
	{
		//float tmp = oldAns[state.vision_id].Y + 20.0f * max_acc.Y * sgn ( ans.Y );
		float tmp = oldAns[state.vision_id].Y + max_dec.Y * sgn ( ans.Y );
		if ( ans.Y == 0 )
			tmp = oldAns[state.vision_id].Y - max_dec.Y * sgn ( oldAns[state.vision_id].Y );
		if ( tmp * ans.Y > 0 )
		{
			tmp = min(max_acc.Y,fabs(tmp)) * sgn ( ans.Y );
			if ( fabs ( tmp ) > fabs ( ans.Y ) )
				tmp = ans.Y;
		}

		ans.Y = tmp;
	}
	else
	{
		/*if ( !accurate )
		 {
		 if ( 30.0f * oldAns[state.vision_id].Y * oldAns[state.vision_id].Y / ( 120.0f * max_dec.Y ) > fabs(target.Position.Y) )
		 {
		 ans.Y = max ( 0 , ( fabs ( oldAns[state.vision_id].Y ) - max_dec.Y ) ) * sgn ( ans.Y );
		 }
		 }*/
		if ( fabs ( ans.Y ) > fabs ( oldAns[state.vision_id].Y ) + max_acc.Y )
		{
			ans.Y = ( fabs ( oldAns[state.vision_id].Y ) + max_acc.Y ) * sgn ( ans.Y );
		}
		else if ( fabs ( ans.Y ) < fabs ( oldAns[state.vision_id].Y ) - max_dec.Y )
		{
			ans.Y = ( fabs ( oldAns[state.vision_id].Y ) - max_dec.Y ) * sgn ( ans.Y );
		}
		if ( fabs ( ans.Y ) > tmp_max_speed.Y )
		{
			if ( sgn(ans.Y) == 0 )
				ans.Y = max(fabs(oldAns[state.vision_id].Y)-max_dec.Y,fabs(tmp_max_speed.Y))*sgn(oldAns[state.vision_id].Y);
			else
				ans.Y = max(fabs(oldAns[state.vision_id].Y)-max_dec.Y,fabs(tmp_max_speed.Y))*sgn(ans.Y);

			//ans.Y = tmp_max_speed.Y * sgn ( ans.Y );
		}


		if ( fabs(ans.Y) > fabs(oldAns[state.vision_id].Y) )
			if ( fabs(ans.Y-oldAns[state.vision_id].Y) > max_acc.Y*1.1 )
				cout << "	gaz nade	" << max_acc.Y << "		<	" << fabs(ans.Y-oldAns[state.vision_id].Y);

		if ( fabs(ans.Y) < fabs(oldAns[state.vision_id].Y) )
			if ( fabs(ans.Y-oldAns[state.vision_id].Y) > max_dec.Y*1.1 )
				cout << "	tormoz nakon	" << max_dec.Y << "		<	" << fabs(ans.Y-oldAns[state.vision_id].Y);

		cout << endl;

	}

	//ans.Y = target.Position.Y/20.0;

	//ans.Y = 0;

	//if ( state.vision_id == 8 )
	//	cout << (int)ans.Z << endl;

	/*float kh_mag = DIS ( Vec2 ( 0.0f ) , Vec2 ( ans.X,ans.Y ) );
	 ans.Z *= ( 3000.0f-kh_mag ) / 1500.0f;*/

	if (!accurate) {
		//ans = Vec3(vdes_ann.X,vdes_ann.Y, 0);
	}

	oldAns[state.vision_id] = ans;

	//if ( state.vision_id == 6 )
	//	cout << "	" << ans.X << "	";

	return ans;
}