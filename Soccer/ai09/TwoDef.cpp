#include "ai09.h"

int lastAttackingDef;
int attackingDefHys = 0;

void ai09::TwoDef ( int robot_num1 , int robot_num2 , TVec2 * defendTarget )
{

	if ( !defendTarget )
		defendTarget = &(ball.Position);
	VecPosition* p1 = new VecPosition ( );
	VecPosition* p2 = new VecPosition ( );

	OwnRobot[robot_num1].face ( (*defendTarget));
	OwnRobot[robot_num2].face ( (*defendTarget));

	TVec2 num1Target , num2Target ;

	if ( ( Line::makeLineFromTwoPoints(VecPosition(side*field_width,-175.0f),VecPosition((*defendTarget).X , (*defendTarget).Y)).getCircleIntersectionPoints(Circle (VecPosition(side*field_width,-175.0f),750.0f),p1,p2) ) &&
		( (*defendTarget).Y < -175.0f ) )
	{
		if ( p1->getY() <= -175.0f )
		{
			num1Target = Vec2 ( p1->getX() , p1->getY() );
		}
		else
		{
			num1Target = Vec2 ( p2->getX() , p2->getY() );
		}
	}
	else if ( ( Line::makeLineFromTwoPoints(VecPosition(side*field_width,-175.0f),VecPosition((*defendTarget).X , (*defendTarget).Y)).getCircleIntersectionPoints(Circle (VecPosition(side*field_width,175.0f),750.0f),p1,p2) ) &&
		( ( p1->getY ( ) > 175.0f ) && ( fabs ( p1->getX() ) < field_width ) || ( p2->getY ( ) > 175.0f ) && ( fabs ( p2->getX() ) < field_width ) ) )
	{
		if ( p1->getY() >= 175.0f )
		{
			num1Target = Vec2 ( p1->getX() , p1->getY() );
		}
		else
		{
			num1Target = Vec2 ( p2->getX() , p2->getY() );
		}
	}
	else
	{
		*p1 = Line::makeLineFromTwoPoints(VecPosition(side*field_width,-175.0f),VecPosition((*defendTarget).X , (*defendTarget).Y)).getIntersection ( Line::makeLineFromTwoPoints ( VecPosition ( side*2275.0f , 175.0f ) , VecPosition ( side*2275.0f , -175.0f ) ) );
		num1Target = Vec2 ( p1->getX() , p1->getY() );
		
	}
	
	ERRTSetObstacles ( robot_num2 , 0 , true , true , false );
	if ( ( Line::makeLineFromTwoPoints(VecPosition(side*3025.0f,175.0f),VecPosition((*defendTarget).X , (*defendTarget).Y)).getCircleIntersectionPoints(Circle (VecPosition(side*3025.0f,175.0f),750.0f),p1,p2) ) &&
		( (*defendTarget).Y > 175.0f ) )
	{
		if ( p1->getY() >= 175.0f )
		{
			num2Target = Vec2 ( p1->getX() , p1->getY() );
		}
		else
		{
			num2Target = Vec2 ( p2->getX() , p2->getY() );
		}
	}
	else if ( ( Line::makeLineFromTwoPoints(VecPosition(side*3025.0f,175.0f),VecPosition((*defendTarget).X , (*defendTarget).Y)).getCircleIntersectionPoints(Circle (VecPosition(side*3025.0f,-175.0f),750.0f),p1,p2) ) &&
		( ( p1->getY ( ) < -175.0f ) && ( fabs ( p1->getX() ) < 3025.0f ) || ( p2->getY ( ) < -175.0f ) && ( fabs ( p2->getX() ) < 3025.0f ) ) )
	{
		if ( p1->getY() <= -175.0f )
		{
			num2Target = Vec2 ( p1->getX() , p1->getY() );
		}
		else
		{
			num2Target = Vec2 ( p2->getX() , p2->getY() );
		}
	}
	else
	{
		*p1 = Line::makeLineFromTwoPoints(VecPosition(side*3025.0f,175.0f),VecPosition((*defendTarget).X , (*defendTarget).Y)).getIntersection ( Line::makeLineFromTwoPoints ( VecPosition ( side*2275.0f , 175.0f ) , VecPosition ( side*2275.0f , -175.0f ) ) );
		num2Target = Vec2 ( p1->getX() , p1->getY() );
	}

	float ballDis1 = DIS ( num1Target , (*defendTarget) );
	float ballDis2 = DIS ( num2Target , (*defendTarget) );
	if ( 0 )//( min ( ballDis1 , ballDis2 ) < 250 ) && ball.velocity.magnitude < 50 )
	{
		if ( ( ballDis1 < ballDis2 ) || ((attackingDefHys>0)&&(lastAttackingDef==1)) )
		{
			if ( ballDis1 < ballDis2 )
			{
				attackingDefHys = 10;
				lastAttackingDef = 1;
			}
			else
			{
				lastAttackingDef = 1;
				attackingDefHys --;
			}

			ERRTSetObstacles ( robot_num1 , 0 , 1 , 1 , 0 , 0 );
			tech_circle(robot_num1,sgn((*defendTarget).Y)*side*120.0f ,0,15);
			
			ERRTSetObstacles ( robot_num2 , 0 , true , true , false );
			ERRTNavigate2Point ( robot_num2 , num2Target );
		}
		else
		{
			if ( ballDis2 < ballDis1 )
			{
				attackingDefHys = 10;
				lastAttackingDef = 2;
			}
			else
			{
				lastAttackingDef = 2;
				attackingDefHys --;
			}

			ERRTSetObstacles ( robot_num2 , 0 , 1 , 1 , 0 , 0 );
			tech_circle(robot_num2,sgn((*defendTarget).Y)*side*120.0f ,0,15);
			
			ERRTSetObstacles ( robot_num1 , 0 , true , true , false );
			ERRTNavigate2Point ( robot_num1 , num1Target );
		}
	}
	else
	{
		ERRTSetObstacles ( robot_num1 , 0 , true , true , false );
		ERRTNavigate2Point ( robot_num1 , num1Target );
		
		ERRTSetObstacles ( robot_num2 , 0 , true , true , false );
		ERRTNavigate2Point ( robot_num2 , num2Target );
	}

	//OwnRobot[robot_num1].Chip ( 15 );
	//OwnRobot[robot_num2].Chip ( 15 );

	free ( p1 );
	free ( p2 );
}