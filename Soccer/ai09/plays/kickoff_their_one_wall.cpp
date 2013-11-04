#include "ai09.h"

void ai09::kickoff_their_one_wall ( void )
{
	swap(dmf, lmf);
	GK_Ghuz ( gk , 1 );
	DefGhuz ( def );
	
	ERRTSetObstacles ( dmf , true , true , true , true );
	OwnRobot[dmf].face(ball.Position);
	ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*3025, 0), DIS(ball.Position, Vec2(side*3025, 0))/3.0f) ,0 , 40,&VELOCITY_PROFILE_MAMOOLI);
		
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
			ERRTSetObstacles ( rmf , true , true , true , false );
			OwnRobot[rmf].face(Vec2(-side*2995,0));
			ERRTNavigate2Point ( rmf , PointOnConnectingLine ( OppRobot[indexN].Position , Vec2(side*2995,0) , (fabs(OppRobot[indexN].Position.X)+10)*1.4 ) );
		}
		else
		{
			ERRTSetObstacles ( lmf , true , true , true , false );
			OwnRobot[lmf].face(Vec2(-side*2995,0));
			ERRTNavigate2Point ( lmf , PointOnConnectingLine ( OppRobot[indexN].Position , Vec2(side*2995,0) , (fabs(OppRobot[indexN].Position.X)+10)*1.4 ) );
		}
	}
	else
	{
		if ( side == -1 )
		{
			ERRTSetObstacles ( rmf , true , true , true , true );
			OwnRobot[rmf].face(Vec2(ball.Position.X,ball.Position.Y));
			ERRTNavigate2Point ( rmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*2995,0))),790) ,0 , 100);
		}
		else
		{
			ERRTSetObstacles ( lmf , true , true , true , true );
			OwnRobot[lmf].face(Vec2(ball.Position.X,ball.Position.Y));
			ERRTNavigate2Point ( lmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*2995,0))),790) ,0 , 100);
		}
		
	}
	
	if ( indexP != -1 )
	{
		if ( side == 1 )
		{
			ERRTSetObstacles ( rmf , true , true , true , false );
			OwnRobot[rmf].face(Vec2(-side*2995,0));
			ERRTNavigate2Point ( rmf , PointOnConnectingLine ( OppRobot[indexP].Position , Vec2(side*2995,0) , (fabs(OppRobot[indexP].Position.X)+10)*1.4 ) );
		}
		else
		{
			ERRTSetObstacles ( lmf , true , true , true , false );
			OwnRobot[lmf].face(Vec2(-side*2995,0));
			ERRTNavigate2Point ( lmf , PointOnConnectingLine ( OppRobot[indexP].Position , Vec2(side*2995,0) , (fabs(OppRobot[indexP].Position.X)+10)*1.4 ) );
		}
	}
	else
	{
		if ( side == 1 )
		{
			ERRTSetObstacles ( rmf , true , true , true , true );
			OwnRobot[rmf].face(Vec2(ball.Position.X,ball.Position.Y));
			ERRTNavigate2Point ( rmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*2995,0))),790) ,0 , 100);
		}
		else
		{
			ERRTSetObstacles ( lmf , true , true , true , true );
			OwnRobot[lmf].face(Vec2(ball.Position.X,ball.Position.Y));
			ERRTNavigate2Point ( lmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*2995,0))),790) ,0 , 100);
		}
		
	}
	
	DefenceWall(attack,true);
	
	swap(dmf, lmf);
}