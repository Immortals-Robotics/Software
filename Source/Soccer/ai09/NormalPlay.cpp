#include "ai09.h"
#include <iostream>
using namespace std;

static float lastBackPassT = 0.0f;

void ai09::NormalPlay ( void )
{
	/*if ( ball.Position.X * side > 0 )
		this->isDefending = true;
	else
		this->isDefending = false;*/
	swap(dmf, lmf);
	if ( 1 )
	{
		OwnRobot[attack].target.Angle = 90;OwnRobot[attack].State.Angle + 160;
		ERRTSetObstacles(attack, 0, 0, 1, 1, 0);
		ERRTNavigate2Point ( attack , Vec2 ( 1500 , 1700 ) , 0 , 100 );
		OwnRobot[rmf].target.Angle = 90;OwnRobot[rmf].State.Angle + 160;
		Navigate2Point ( rmf , Vec2 ( -1600 , 1600 ) , 0 , 100 );
		//Navigate2Point(rmf, Vec2(OwnRobot[attack].State.Position.X+OwnRobot[attack].State.velocity.x/2.0f,OwnRobot[attack].State.Position.Y+OwnRobot[attack].State.velocity.y/2.0f), 0, 100);
		OwnRobot[lmf].target.Angle = 0;OwnRobot[dmf].State.Angle + 160;
		Navigate2Point ( lmf , Vec2 ( 2000 , 200 ) , 0 , 100 );
		//backPass(attack,AngleWith ( Vec2 ( -side * 3033 , 0 ) , ball.Position ) );
		//HaltAll();
	}
	else if ( ( ball.Position.X * side < -1500 ) && ( fabs ( ball.Position.Y ) > 1400 ) )
	{
		reached = true;
		//side=-side;
		//GK ( gk , 1 );
		//OneDef ( def );
		GKHi(gk);
		DefHi(def);
		//Halt(lmf);
		//side=-side;
		
		if (( ( DIS ( ball.Position , OwnRobot[attack].State.Position ) < 250 ) && ( OwnRobot[attack].State.velocity.magnitude < 100 ) )|| ( reached ) )
		{
			tech_circle(attack,AngleWith ( Vec2 ( -side*2000 , -sgn ( ball.Position.Y ) * 1000 ) , ball.Position ) ,0,5);
			reached = true;
		}
		else
			tech_circle(attack,AngleWith ( Vec2 ( -side*1000 , -sgn ( ball.Position.Y ) * 1000 ) , ball.Position ) ,0,0);
		//backPass(attack,AngleWith ( Vec2 ( -side*2000 , -sgn ( ball.Position.Y ) * 1000 ) , ball.Position ) ,lastBackPassT);

		
		if (oneTouchDetector[lmf].IsArriving(70)) {
			WaitForPass ( lmf );
		}
		else {
			OwnRobot[lmf].face ( Vec2 ( -side*3033 , 0 ) );
			ERRTSetObstacles ( lmf );
			ERRTNavigate2Point ( lmf , Vec2 ( -side*500 , sgn(ball.Position.Y )* 1700 ) );
		}
		
		if (oneTouchDetector[rmf].IsArriving(70)) {
			WaitForPass ( rmf );
		}
		else {
			OwnRobot[rmf].face ( Vec2 ( -side*3033 , 0 ) );
			ERRTSetObstacles ( rmf );
			ERRTNavigate2Point ( rmf , Vec2 ( -side*2200 , -sgn(ball.Position.Y )* 1700 ) );
		}
	}
	else
	{
		lastBackPassT = timer.time();
		reached = false;
		//side=-side;
		//GK ( gk , 1 );
		//OneDef ( def );
		GKHi(gk);
		DefHi(def);
		Halt(dmf);
		//Halt(rmf);
		//side=-side;
		//OwnRobot[lmf].Shoot( 15 );
		/*if ( ( fabs ( NormalizeAngle ( ball.velocity.direction - AngleWith ( ball.Position , Vec2 ( OwnRobot[lmf].State.Position.X + BAR * cosDeg ( OwnRobot[lmf].State.Angle ) , OwnRobot[lmf].State.Position.Y + BAR * sinDeg ( OwnRobot[lmf].State.Angle ) ) ) ) ) < 65 ) && ( fabs(NormalizeAngle(180+ball.velocity.direction-OwnRobot[lmf].State.Angle)) < 150 ) && ( ball.velocity.magnitude > 700 ) )//&&(abs(ball.vel_angle-90)>0.01)&&(abs(ball.vel_angle+90)>0.01)&&(abs(ball.vel_angle-180)>0.01)&&(abs(ball.vel_angle+180)>0.01))
		{
			WaitForPass ( lmf );
			hys = 40;
		}
		else if (( hys > 0 )&& ( ball.velocity.magnitude > 50 ))// &&(abs(ball.vel_angle-90)>0.01)&&(abs(ball.vel_angle+90)>0.01)&&(abs(ball.vel_angle-180)>0.01)&&(abs(ball.vel_angle+180)>0.01))
		{
			WaitForPass ( lmf );
			hys --;
		}
		
		else
		{
			hys = 0;
			OwnRobot[lmf].face ( Vec2 ( -side*3033 , 0 ) );
			ERRTSetObstacles ( lmf );
			if ( !isDefending )
				ERRTNavigate2Point ( lmf , Vec2 ( -side*500 , -sgn(ball.Position.Y )* 1700 ) );
			else
			{
				int index = findCruncherOpp(-1);
				if ( index == -1 )
				{
					ERRTNavigate2Point ( lmf , Vec2 ( side*500,0));//CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*2995,0))),590) ,0 , 100);
				}
				else
				{
					ERRTNavigate2Point ( lmf , PointOnConnectingLine ( OppRobot[index].Position , Vec2 ( side*2995 , 0 ) , 220.0f ) );
				}
			}
		}*/
		
		if (oneTouchDetector[lmf].IsArriving(70)) {
			WaitForPass ( lmf );
		}
		else {
			OwnRobot[lmf].face ( Vec2 ( -side*3033 , 0 ) );
			ERRTSetObstacles ( lmf );
			ERRTNavigate2Point ( lmf , Vec2 ( -side*500 , sgn(ball.Position.Y )* 1700 ) );
		}
		
		if (oneTouchDetector[rmf].IsArriving(70)) {
			WaitForPass ( rmf );
		}
		else {
			OwnRobot[rmf].face ( Vec2 ( -side*3033 , 0 ) );
			ERRTSetObstacles ( rmf );
			ERRTNavigate2Point ( rmf , Vec2 ( -side*2500 , -sgn(ball.Position.Y )* 1700 ) );
		}

		ERRTSetObstacles ( attack );
		//tech_circle(attack,AngleWith ( Vec2 ( -side * 3033 , 0 ) , ball.Position ) ,15);
		tech_circle(attack, NormalizeAngle( 180+calculateOpenAngleToGoal(ball.Position, attack).X), 15 ,0,1,0,0,1);
		//backPass(attack, NormalizeAngle( 180+calculateOpenAngleToGoal(ball.Position, attack).X));
		//Halt(attack);
	}
	swap(dmf, lmf);
}