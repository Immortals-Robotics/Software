#include "../ai09.h"

void ai09::kickoff_their_back_def ( void )
{
	GKHi ( gk , 1 );
	DefHi ( def );
	
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
		
		ERRTSetObstacles ( lmf , true , true , true , false );
		OwnRobot[lmf].face(OppRobot[indexN].Position);
		ERRTNavigate2Point ( lmf , PointOnConnectingLine ( Vec2(side*2995,-150) , OppRobot[indexN].Position , 900 ) );
	}
	else
	{
		
		ERRTSetObstacles ( lmf , true , true , true , false );
		OwnRobot[lmf].face(Vec2(-side*200, -2000));
		ERRTNavigate2Point ( lmf , PointOnConnectingLine ( Vec2(side*2995,-150) , Vec2(-side*200, -2000) , 900 ) );
	}
	
	if ( indexP != -1 )
	{
		ERRTSetObstacles ( rmf , true , true , true , false );
		OwnRobot[rmf].face(OppRobot[indexP].Position);
		ERRTNavigate2Point ( rmf , PointOnConnectingLine ( Vec2(side*2995,150) , OppRobot[indexP].Position , 900 ) );
	}
	else
	{
		ERRTSetObstacles ( rmf , true , true , true , false );
		OwnRobot[rmf].face(Vec2(-side*200, 2000));
		ERRTNavigate2Point ( rmf , PointOnConnectingLine ( Vec2(side*2995,150) , Vec2(-side*200, 2000) , 900 ) );
	}
	
	DefenceWall(attack,true);
}