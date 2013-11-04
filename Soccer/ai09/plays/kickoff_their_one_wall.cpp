#include "ai09.h"

void ai09::kickoff_their_one_wall ( void )
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
		if ( side == -1 )
		{
			ERRTSetObstacles ( passgir , true , true , true , false );
			OwnRobot[passgir].face(Vec2(-side*2995,0));
			ERRTNavigate2Point ( passgir , PointOnConnectingLine ( OppRobot[indexN].Position , Vec2(side*2995,0) , (fabs(OppRobot[indexN].Position.X)+10)*1.4 ) );
		}
		else
		{
			ERRTSetObstacles ( def2 , true , true , true , false );
			OwnRobot[def2].face(Vec2(-side*2995,0));
			ERRTNavigate2Point ( def2 , PointOnConnectingLine ( OppRobot[indexN].Position , Vec2(side*2995,0) , (fabs(OppRobot[indexN].Position.X)+10)*1.4 ) );
		}
	}
	else
	{
		if ( side == -1 )
		{
			ERRTSetObstacles ( passgir , true , true , true , true );
			OwnRobot[passgir].face(Vec2(ball.Position.X,ball.Position.Y));
			ERRTNavigate2Point ( passgir , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*2995,0))),990) ,0 , 100);
		}
		else
		{
			ERRTSetObstacles ( def2 , true , true , true , true );
			OwnRobot[def2].face(Vec2(ball.Position.X,ball.Position.Y));
			ERRTNavigate2Point ( def2 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*2995,0))),990) ,0 , 100);
		}
		
	}
	
	if ( indexP != -1 )
	{
		if ( side == 1 )
		{
			ERRTSetObstacles ( passgir , true , true , true , false );
			OwnRobot[passgir].face(Vec2(-side*2995,0));
			ERRTNavigate2Point ( passgir , PointOnConnectingLine ( OppRobot[indexP].Position , Vec2(side*2995,0) , (fabs(OppRobot[indexP].Position.X)+10)*1.4 ) );
		}
		else
		{
			ERRTSetObstacles ( def2 , true , true , true , false );
			OwnRobot[def2].face(Vec2(-side*2995,0));
			ERRTNavigate2Point ( def2 , PointOnConnectingLine ( OppRobot[indexP].Position , Vec2(side*2995,0) , (fabs(OppRobot[indexP].Position.X)+10)*1.4 ) );
		}
	}
	else
	{
		if ( side == 1 )
		{
			ERRTSetObstacles ( passgir , true , true , true , true );
			OwnRobot[passgir].face(Vec2(ball.Position.X,ball.Position.Y));
			ERRTNavigate2Point ( passgir , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*2995,0))),990) ,0 , 100);
		}
		else
		{
			ERRTSetObstacles ( def2 , true , true , true , true );
			OwnRobot[def2].face(Vec2(ball.Position.X,ball.Position.Y));
			ERRTNavigate2Point ( def2 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*2995,0))),990) ,0 , 100);
		}
		
	}
	
	DefenceWall(attack, 10);
}