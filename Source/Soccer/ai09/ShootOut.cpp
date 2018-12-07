//
// Created by dot_blue on 12/6/18.
//
#include "ai09.h"

void ai09::penalty_our_Shoot_Out(){//For the 2018 Field size

    cout<<"penalty_our_Shoot_Out is running"<<endl;
    if( side*ball.Position.X > -3500){
        OwnRobot[attack].face(Vec2(-side*field_width,0));
        ERRTSetObstacles(attack,0,1,0,1,0,0);//test
        ERRTNavigate2Point(attack, ball.Position, 1, 100, &VELOCITY_PROFILE_AROOM);
        OwnRobot[attack].Shoot(0);
        OwnRobot[attack].Chip(0);
    } else {
        TVec2 shootPoint;

        float shootDelta = (goal_width/2.0) - 50.0f;

        if ( randomParam < 0.5 )
        {
            shootPoint = Vec2 ( -side*field_width,-shootDelta );
        }
        else {
            shootPoint = Vec2 ( -side*field_width,shootDelta );
        }

        VecPosition intPoint[2];
        side = -side;
        int oppGK = findCruncherOpp(-1, -1,true);
        side = -side;
        cout << "	GK e harif: " << oppGK << endl;
        if ( oppGK != -1 )
        {
            Line shootLine = Line::makeLineFromTwoPoints ( VecPosition ( shootPoint.X,shootPoint.Y ) , VecPosition ( ball.Position.X,ball.Position.Y ) );
            if (shootLine.getCircleIntersectionPoints(Circle(VecPosition(OppRobot[oppGK].Position.X,OppRobot[oppGK].Position.Y),130),intPoint,intPoint+1)>0) {
                shootPoint.Y = -shootPoint.Y;
            }
        }
//        penaltyAngle+=180;
        penaltyAngle = AngleWith ( shootPoint , ball.Position );

        PenaltyUs(attack, NormalizeAngle( penaltyAngle), 28 );//TODO Fix this (it goes in the goal field)
//        PenaltyUs(attack,)
    }
}





