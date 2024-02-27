#include "../ai09.h"

//vector<TVec2> wayp;
TVec2 wayp[6] = { 
	Vec2(2700,0) ,
	Vec2(2700,1700) ,
	Vec2(2700,0) ,
	Vec2(2700,-1700) ,
	Vec2(-1400,1100) ,
	Vec2(-1800,0) 
};

int curr[6] = {0,1,2,3,0,1};

void ai09::tech_challenge ( void )
{
	/*ERRTSetObstacles ( 0 , true , true , true , true );
	 OwnRobot[0].face(Vec2(ball.Position.X,ball.Position.Y));
	 ERRTNavigate2Point ( 0 , Vec2 ( -200 , -1800 ) ,0 , 60);
	 
	 ERRTSetObstacles ( 1 , true , true , true , true );
	 OwnRobot[1].face(Vec2(ball.Position.X,ball.Position.Y));
	 ERRTNavigate2Point ( 1 , Vec2 ( 200 , -1800 ) ,0 , 60);
	 
	 ERRTSetObstacles ( 2 , true , true , true , true );
	 OwnRobot[2].face(Vec2(ball.Position.X,ball.Position.Y));
	 ERRTNavigate2Point ( 2 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*3025,0))),590) ,0 , 60);
	 
	 ERRTSetObstacles ( 3 , true , true , true , true );
	 OwnRobot[3].face(Vec2(ball.Position.X,ball.Position.Y));
	 ERRTNavigate2Point ( 3 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*3025,0))),590) ,0 , 60);*/
	
	/*int rn =0;
	 if ( dis ( wayp[curr].X , wayp[curr].Y , OwnRobot[rn].State.Position.X , OwnRobot[rn].State.Position.Y ) < 30 )
	 curr ++;
	 curr = curr % 6;
	 ERRTSetObstacles ( rn , false , false , true , true );
	 OwnRobot[rn].target.Angle = 0.0f;
	 //OwnRobot[rn].face ( wayp[curr] );
	 //OwnRobot[rn].face(Vec2(ball.Position.X,ball.Position.Y));
	 //OwnRobot[rn].target.velocity.x = wayp[(curr+1)%4].X-wayp[curr].X;
	 //OwnRobot[rn].target.velocity.y = wayp[(curr+1)%4].Y-wayp[curr].Y;
	 ERRTNavigate2Point ( rn , wayp[curr] ,0 , 100);*/
	
	bool adv = true;
	for ( int i = 0 ; i < 4 ; i ++ )
	{
		if ( dis ( wayp[curr[i]].X , wayp[curr[i]].Y , OwnRobot[i].State.Position.X , OwnRobot[i].State.Position.Y ) > 50 )
			adv	= false;
	}
	
	for ( int i = 0 ; i < 6 ; i ++ )
	{
		if ( adv )
		{
			curr[i] ++;
			curr[i] = curr[i] % 4;
		}

		ERRTSetObstacles ( i , false , false , true , true );
		OwnRobot[i].target.Angle = 90.0f - 90*sgn(wayp[curr[i]].Y);
		//OwnRobot[i].face ( wayp[curr[i]] );
		//OwnRobot[rn].face(Vec2(ball.Position.X,ball.Position.Y));
		//OwnRobot[rn].target.velocity.x = wayp[(curr+1)%4].X-wayp[curr].X;
		//OwnRobot[rn].target.velocity.y = wayp[(curr+1)%4].Y-wayp[curr].Y;
		ERRTNavigate2Point ( i , wayp[curr[i]] ,0 , 100,&VELOCITY_PROFILE_MAMOOLI);
	}
	
	/*ERRTSetObstacles ( rn+1 , false , false , true , true );
	 OwnRobot[rn+1].target.Angle = 0.0f;
	 ERRTNavigate2Point ( rn+1 , wayp[(curr+1)%4] ,0 , 100);
	 
	 ERRTSetObstacles ( rn+2 , false , false , true , true );
	 OwnRobot[rn+2].target.Angle = 0.0f;
	 ERRTNavigate2Point ( rn+2 , wayp[(curr+2)%4] ,0 , 100);
	 
	 ERRTSetObstacles ( rn+3 , false , false , true , true );
	 OwnRobot[rn+3].target.Angle = 0.0f;
	 ERRTNavigate2Point ( rn+3 , wayp[(curr+3)%4] ,0 , 100);*/
}