#include "ai09.h"

#include "ai09.h"

void ai09::kickoff_their_back_def ( void )
{
	GK ( gk , 1 );
	OneDef ( def1 );
	
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
		
		ERRTSetObstacles ( def2 , true , true , true , false );
		OwnRobot[def2].face(OppRobot[indexN].Position);
		ERRTNavigate2Point ( def2 , PointOnConnectingLine ( Vec2(side*2995,-150) , OppRobot[indexN].Position , 900 ) );
	}
	else
	{
		
		ERRTSetObstacles ( def2 , true , true , true , false );
		OwnRobot[def2].face(Vec2(-side*200, -2000));
		ERRTNavigate2Point ( def2 , PointOnConnectingLine ( Vec2(side*2995,-150) , Vec2(-side*200, -2000) , 900 ) );
	}
	
	if ( indexP != -1 )
	{
		ERRTSetObstacles ( passgir , true , true , true , false );
		OwnRobot[passgir].face(OppRobot[indexP].Position);
		ERRTNavigate2Point ( passgir , PointOnConnectingLine ( Vec2(side*2995,150) , OppRobot[indexP].Position , 900 ) );
	}
	else
	{
		ERRTSetObstacles ( passgir , true , true , true , false );
		OwnRobot[passgir].face(Vec2(-side*200, 2000));
		ERRTNavigate2Point ( passgir , PointOnConnectingLine ( Vec2(side*2995,150) , Vec2(-side*200, 2000) , 900 ) );
	}
	
	DefenceWall(attack, 10);
}