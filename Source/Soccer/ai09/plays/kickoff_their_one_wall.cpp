#include "../ai09.h"

void ai09::kickoff_their_one_wall ( void )
{
	//swap(dmf, lmf);
	GKHi ( gk , 1 );
	DefHi ( def );
	
	ERRTSetObstacles ( dmf , true , true , true , true );
	OwnRobot[dmf].face(ball.Position);
	ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/2.0f) ,0 , 40,&VELOCITY_PROFILE_MAMOOLI);
		
	int indexP = -1;
	int indexN = -1;
	
	for ( int i = 0 ; i < 12 ; i ++ )
	{
		if ( ( fabs ( OppRobot[i].Position.X ) < 600 ) && ( fabs ( OppRobot[i].Position.Y ) > 600 ) && ( OppRobot[i].seenState != CompletelyOut ) )
		{
			if ( OppRobot[i].Position.Y > 0 )
				indexP = i;
			if ( OppRobot[i].Position.Y < 0 )
				indexN = i;
		}
	}
	
	cout << indexN << "	" << indexP << endl;
	
	if ( indexN != -1 )
	{
		if ( side == -1 )
		{
			ERRTSetObstacles ( mid1 , true , true , true , false );
			OwnRobot[mid1].face(Vec2(-side*field_width,0));
			ERRTNavigate2Point ( mid1 , PointOnConnectingLine ( OppRobot[indexN].Position , Vec2(side*field_width,0) , (fabs(OppRobot[indexN].Position.X)+14)*1.5 ) );
			markMap[&mid1] = indexN;
		}
		else
		{
			ERRTSetObstacles ( mid2 , true , true , true , false );
			OwnRobot[mid2].face(Vec2(-side*field_width,0));
			ERRTNavigate2Point ( mid2 , PointOnConnectingLine ( OppRobot[indexN].Position , Vec2(side*field_width,0) , (fabs(OppRobot[indexN].Position.X)+14)*1.5 ) );
			markMap[&mid2] = indexN;
		}
	}
	else
	{
		if ( side == -1 )
		{
			ERRTSetObstacles ( mid1 , true , true , true , true );
			OwnRobot[mid1].face(Vec2(ball.Position.X,ball.Position.Y));
			ERRTNavigate2Point ( mid1 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*field_width,0))),790) ,0 , 100);
			markMap[&mid1] = -1;
		}
		else
		{
			ERRTSetObstacles ( mid2 , true , true , true , true );
			OwnRobot[mid2].face(Vec2(ball.Position.X,ball.Position.Y));
			ERRTNavigate2Point ( mid2 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*field_width,0))),790) ,0 , 100);
			markMap[&mid2] = -1;
		}
		
	}
	
	if ( indexP != -1 )
	{
		if ( side == 1 )
		{
			ERRTSetObstacles ( mid1 , true , true , true , false );
			OwnRobot[mid1].face(Vec2(-side*field_width,0));
			ERRTNavigate2Point ( mid1 , PointOnConnectingLine ( OppRobot[indexP].Position , Vec2(side*field_width,0) , (fabs(OppRobot[indexP].Position.X)+14)*1.5 ) );
			markMap[&mid1] = indexP;
		}
		else
		{
			ERRTSetObstacles ( mid2 , true , true , true , false );
			OwnRobot[mid2].face(Vec2(-side*field_width,0));
			ERRTNavigate2Point ( mid2 , PointOnConnectingLine ( OppRobot[indexP].Position , Vec2(side*2995,0) , (fabs(OppRobot[indexP].Position.X)+14)*1.5 ) );
			markMap[&mid2] = indexP;
		}
	}
	else
	{
		if ( side == 1 )
		{
			ERRTSetObstacles ( mid1 , true , true , true , true );
			OwnRobot[mid1].face(Vec2(ball.Position.X,ball.Position.Y));
			ERRTNavigate2Point ( mid1 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*field_width,0))),790) ,0 , 100);
			markMap[&mid1] = -1;
		}
		else
		{
			ERRTSetObstacles ( mid2 , true , true , true , true );
			OwnRobot[mid2].face(Vec2(ball.Position.X,ball.Position.Y));
			ERRTNavigate2Point ( mid2 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*field_width,0))),790) ,0 , 100);
			markMap[&mid2] = -1;
		}
		
	}
	
	DefenceWall(attack,true);
	
	//swap(dmf, lmf);
}