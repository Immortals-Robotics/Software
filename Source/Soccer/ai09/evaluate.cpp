#include "ai09.h"
VecPosition obj[20];
VecPosition* ptr = new VecPosition ( );

VecPosition t1 ( 0 , 167 );               //tire yek
VecPosition t2 ( 0 , 237 );			   //tire Do
VecPosition center (0,202);
VecPosition tmp,ans,bal,tans;
bool s;

float dteta ;
float dr [20];
float di [10];

int obj_num = 0;

void ai09::SetEvaulateOBJs ( int robot_num1 , int robot_num2 )
{
	obj_num = 0;
	for ( int i = 0 ; i < 5 ; i ++ )
	{
		if ( ( OwnRobot[i].State.seenState != CompletelyOut ) && ( i != robot_num1 ) && ( i != robot_num2 ) && ( OwnRobot[i].State.vision_id != OwnRobot[robot_num1].State.vision_id ) && ( OwnRobot[i].State.vision_id != OwnRobot[robot_num2].State.vision_id ) )
		{
			obj[obj_num] = VecPosition ( 302 + OwnRobot[i].State.Position.X / 10.0f , 202 + OwnRobot[i].State.Position.Y / 10.0f );
			obj_num ++;
		}
	}
	for ( int i = 0 ; i < OppRobotNum ; i ++ )
	{
		obj[obj_num] = VecPosition ( 302 + OppRobot[i].Position.X / 10 , 202 + OppRobot[i].Position.Y / 10 );
		obj_num ++;
	}
	bal = VecPosition ( 302.0f + ball.Position.X / 10.0f , 202.0f + ball.Position.Y / 10.0f );
	center.setX ( ( side + 1 ) * 302 );
	t1.setX ( ( side + 1 ) * 302 );
	t2.setX ( ( side + 1 ) * 302 );
	s = side + 1;
}

void ai09::PassEvaluation ( void )
{
	if( side )
	{
		for ( int i = 0 ; i < obj_num ; i++ )
			obj[i].setX(605-obj[i].getX());
		center.setX(605-center.getX());
		t1.setX(605-t1.getX());
		t2.setX(605-t2.getX());
		bal.setX(605-bal.getX());
	}
	float maxScore = 0;
			
	for(int x=0;x<605;x+=10)
	{
		for(int y=0;y<405;y+=10)
		{
			tmp.setVecPosition( x , y);

			
			//ZAVIYE TOOP TA NOGHTE VA DISTANCE

			float dis2 = 
				//dis_lut[(int)fabs(bal.getX()-x)][(int)fabs(bal.getY()-y)];
				tmp.getDistanceTo(bal);
			float def2 = atanDeg(20/dis2);
		   	def2 = fabs(def2)*2;
				

			for(int i=0;i<obj_num;i++)
			{   
				//c.setCircle( obj[i] , 15 );
				//l.makeLineFromTwoPoints ( tmp , bal );

				if ( def2 > 0.01 )
				{
					if ( Line::makeLineFromTwoPoints ( bal , obj[i] ).isInBetween ( obj[i] , bal , tmp ) )
						if ( Line::makeLineFromTwoPoints ( tmp , bal ).getCircleIntersectionPoints ( Circle( obj[i] , 15 ) , ptr , ptr) )
						{
							def2 = 0;
						}
					}
				}

				// ZAVIYE TA DARVAZE VA ROBOT HA
					
					
				float d2 =(t2-tmp).getDirection();
				if (d2 < 0 ) d2+=360;

				float d1 = (t1-tmp).getDirection();
				if (d1 < 0 ) d1+=360;

				float def = fabs(VecPosition::normalizeAngle( d1-d2));
    
				for(int i=0;i<obj_num;i++)
				{   
					dr [i] = (obj[i]-tmp).getDirection();
					if(dr[i]<0) dr[i]+=360; 
					di [i] = 
						//dis_lut[(int)fabs(obj[i].getX()-x)][(int)fabs(obj[i].getY()-y)];
						(tmp.getDistanceTo(obj[i]));
					dteta = asinDeg(9/di[i]);
					if(dteta<0) dteta+=360;
					    
					if((dr[i] - dteta) >d2  && (dr[i] + dteta) < d1)						 //Robot tooye zaviye
					{
						def = max (fabs(dr[i] - dteta - d2) , fabs(dr[i] + dteta - d1));
					}
					else if(dr[i] + dteta > d2 && dr[i] + dteta < d1)                    //bakhshi az Robot birune zaviye
					{
						def = fabs(dr[i] + dteta - d1);
					}
					else if (dr[i] - dteta < d1 &&  dr[i] - dteta > d2)					//bakhshi az Robot birune zaviye
					{
						def = fabs(dr[i] - dteta - d2);
					}
					else if(dr[i] - dteta < d2 && dr[i] + dteta > d1 )                  // robot zaviye ro baste bashe
					{
						def=0;
					}
				}


				// ZAVIYE HAR NOGHTE AZ TOOP VA VASATE DARVAZE

				float d3 = (center-tmp).getDirection();
				if (d3<0) d3+=360;
				float d4 = (bal-tmp).getDirection();
				if (d4<0) d4+=360;
				float def3 = fabs(VecPosition::normalizeAngle(d4-d3));
				if(def3>90)
					def3=0;
				else if ( def3 > 45 )
					def3 = 1;
				else
					def3 = def3 / 45.0;

					
				float score = min ( fabs ( VecPosition::normalizeAngle ( def ) ) , fabs ( VecPosition::normalizeAngle ( def2 ) ) ) * def3;

				if ( score > maxScore )
				{
					maxScore = score;
					tans = tmp;
				}
		}
	}
	if( side )
	{
		for ( int i = 0 ; i < obj_num ; i++ )
			obj[i].setX(605-obj[i].getX());
		center.setX(605-center.getX());
		t1.setX(605-t1.getX());
		t2.setX(605-t2.getX());
		bal.setX(605-bal.getX());

		tans.setX(605-tans.getX());
	}
	tans -= VecPosition ( 302 , 202 );
	ans = VecPosition ( 10 * tans.getX ( ) , 10 * tans.getY ( ) ) ;
}