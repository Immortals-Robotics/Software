#include "../ai09.h"

float tetta = 0;

void ai09::tech_mexico ( void )
{
	int rn = 0;
	ERRTSetObstacles ( rn , false , false , true , true );
	
	float hhh = sinDeg(tetta);
	float ggg = cosDeg(tetta);

	if ( ggg < 0 )
		ggg = -1;
	else
		ggg = 1;
	
	for (int i = 0; i<6; i++) {
		//OwnRobot[i].face(Vec2(0, 0));
		ERRTSetObstacles ( i , false , false , true , true );
		/*if ( ggg < -0.5 )
		{
			OwnRobot[i].target.Angle = 90.0;
			ERRTNavigate2Point ( i   , Vec2 ( (2500.0f+0*1000.0f) , -1.0*(1300.0f+300.0f) ) , 1 , 100 );
		}
		else
		{
			//OwnRobot[i].target.Angle += 1.0;
			ERRTNavigate2Point ( i   , Vec2 ( (1500.0f+ggg*1000.0f) , ggg*(1300.0f+300.0f) ) , 0 , 100 );
		}*/
		OwnRobot[i].target.Angle = 90.0;
		//ERRTNavigate2Point ( i   , Vec2 ( (100.0f+1000.0f) , ggg*(1300.0f+300.0f) ) , 1 , 100 );
		ERRTNavigate2Point ( i   , Vec2 (-3000+400.0f*i , ggg *1700.0f) , 1 , 100,&VELOCITY_PROFILE_MAMOOLI );
		/*if (i==cmf) {
			debugDraw=true;
			AddDebugCircle(OwnRobot[i].State.Position,90,Blue_Violet);
			AddDebugCircle(OwnRobot[i].target.Position,90,Gold);
			debugDraw=false;
		}*/

	}
	
	/*OwnRobot[rn].target.Angle = 90.0f+hhh*0.0f;
	
	//Navigate2Point ( rn   , Vec2 (  -1000 + hhh*00.0f , 00*hhh+1400.0 ) , 0 , 100 );
	//OwnRobot[rn].Shoot(10);
	OwnRobot[rn].face(Vec2(0, 0));
	ERRTNavigate2Point ( rn   , Vec2 ( -1600.0f * hhh , ggg*1600.0f ) , 0 , 100 );
	//OwnRobot[attack].MoveByMotion(Vec3(0, 30.0*sin(tetta*3.14/180.0), 0));
	
	//dribble(rn, Vec2(0, 0));
	
	ERRTSetObstacles ( rn+1 , true , false , true , true );
	OwnRobot[rn+1].target.Angle = 90.0f;
	ERRTNavigate2Point ( rn+1 , Vec2 ( -500.0f , 1800.0f*sin ( ( tetta + 30 ) * 3.1415 / 180.0 ) ) , 0 , 100 );
	//Navigate2Point ( rn+1   , Vec2 (  -700+hhh*0.0f , 00+hhh*1400.0 ) , 0 , 100 );


	ERRTSetObstacles ( rn+2 , true , false , true , true );
	OwnRobot[rn+2].target.Angle = 90.0f;
	//ERRTNavigate2Point ( rn+2 , Vec2 ( 0.0f , 1800.0f*sin ( ( tetta + 60 ) * 3.1415 / 180.0 ) ) , 0 , 100 );
	//ERRTNavigate2Point ( rn+2   , Vec2 (  hhh*2700.0f , -1300.0f ) , 0 , 100 );
	Navigate2Point ( rn+2   , Vec2 (  -400+hhh*0.0f , 00+hhh*1400.0 ) , 0 , 100 );



	ERRTSetObstacles ( rn+3 , true , false , true , true );
	OwnRobot[rn+3].target.Angle = 90.0f;
	//ERRTNavigate2Point ( rn+3 , Vec2 ( 500.0f , 1800.0f*sin ( ( tetta + 90 ) * 3.1415 / 180.0 ) ) , 0 , 100 );
	Navigate2Point ( rn+3   , Vec2 (  -100+hhh*0.0f , 00+hhh*1400.0 ) , 0 , 100 );


	ERRTSetObstacles ( rn+4 , true , false , true , true );
	OwnRobot[rn+4].target.Angle = 90.0f;
	//ERRTNavigate2Point ( rn+4 , Vec2 ( 1000.0f , 1800.0f*sin ( ( tetta + 120 ) * 3.1415 / 180.0 ) ) , 0 , 100 );
	Navigate2Point ( rn+4   , Vec2 (  200+hhh*0.0f , 00+hhh*1400.0 ) , 0 , 100 );
	
	ERRTSetObstacles ( rn+5 , true , false , true , true );
	OwnRobot[rn+5].target.Angle = 90.0f;
	//ERRTNavigate2Point ( rn+5 , Vec2 ( 1000.0f , 1800.0f*sin ( ( tetta + 120 ) * 3.1415 / 180.0 ) ) , 0 , 100 );
	Navigate2Point ( rn+5   , Vec2 (  500+hhh*0.0f , 00+hhh*1400.0 ) , 0 , 100 );*/



	tetta +=0.8f;

	if ( tetta >= 360 )
		tetta = 0;
	//tetta = 135;
}