//
// Created by dot_blue on 6/6/18.
//

#include "ai09.h"

void ai09::their_place_ball ( void ){
    GKHi( gk , 1 , true );
    DefHi(def,NULL, true);


    ERRTSetObstacles ( dmf , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[dmf].face(ball.Position);
    ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( rw , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[rw].face(ball.Position);
    ERRTNavigate2Point ( rw ,Vec2(0,-100) + PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( lw , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[lw].face(ball.Position);
    ERRTNavigate2Point ( lw ,Vec2(0,100) + PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);


    ERRTSetObstacles ( lmf , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[lmf].face(Vec2(ball.Position.X,ball.Position.Y));
    ERRTNavigate2Point ( lmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*field_width,0))),1090) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( rmf , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[rmf].face(Vec2(ball.Position.X,ball.Position.Y));
    ERRTNavigate2Point ( rmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*field_width,0))),1090) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( cmf , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[cmf].face(Vec2(ball.Position.X,ball.Position.Y));
    ERRTNavigate2Point ( cmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(AngleWith(ball.Position , Vec2(side*field_width,0))),1090) ,0 , 100,&VELOCITY_PROFILE_AROOM);
    //side=-side;
}

