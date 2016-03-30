#include "../ai09.h"

void ai09::throwin_us_outgir ( void )
{
    GKHi(gk, 1, 0);
	DefHi(def,NULL, 0);

	ERRTSetObstacles(dmf, 1, 1, 1, 1, 0);
	ERRTNavigate2Point(dmf, PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), 2000), 0, 100, &VELOCITY_PROFILE_MAMOOLI);
    
    if ( timer.time() < 2.0 )
	{
		float passAngle = 90-side*90;
		circle_ball(attack, passAngle, 0, 0, 1.0f);
	}
	else if ( timer.time() < 4.0 )
    {
        float passAngle = sgn(ball.Position.Y) * 90.0f;
		circle_ball(attack, passAngle, 0, 0, 1.0f);
    }
    
    else if ( timer.time() < 5.0 )
    {
        float passAngle = AngleWith(Vec2(-side*field_width,0),ball.Position);
        PenaltyUs(attack, passAngle, 0 );
    }
    
    /*else if ( timer.time() < 5.2 )
    {
        float passAngle = AngleWith(Vec2(-side*field_width,0),ball.Position);
        PenaltyUs(attack, passAngle, 1 );

    }*/
    else //if ( timer.time() < 5.3 )
    {
        float passAngle = 90-side*90;
		//circle_ball(attack, passAngle, 100, 0, 1.0f);
        //tech_circle ( attack , passAngle , 100 , 0 , false , false , false , true , false);
        PenaltyUs(attack, passAngle, 100 );
        
    }
    
    if ( timer.time() < 1.0 )
    {
        Halt(lmf);
        Halt(rmf);
    }
    else
    {
        ERRTSetObstacles ( lmf , true , true , true, true, false, true);
        ERRTNavigate2Point ( lmf , Vec2(-side*(field_width-800),-sgn(ball.Position.Y)*1800) , true , 100 , &VELOCITY_PROFILE_MAMOOLI );
        
        ERRTSetObstacles ( rmf , true , true , true, true, false, true);
        ERRTNavigate2Point ( rmf , Vec2(-side*100,-sgn(ball.Position.Y)*1000) , true , 100 , &VELOCITY_PROFILE_MAMOOLI );
    }
}