//
// Created by dot_blue on 6/6/18.
//

#include "ai09.h"

void ai09::their_place_ball ( void ){
    GKHi( gk , true );
    DefHi(def,NULL, true);


    ERRTSetObstacles ( dmf , true , true , true , true, false);
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    AddCircle ( targetBallPlacement->X , targetBallPlacement->Y , 1010.0f );
    for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
    {
        if ( OppRobot[i].seenState != CompletelyOut )
        {
            AddCircle ( OppRobot[i].Position.X , OppRobot[i].Position.Y , 300.0 );
        }
    }
    OwnRobot[dmf].face(ball.Position);
    ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( rw , true, true , true , true, false);
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    AddCircle ( targetBallPlacement->X , targetBallPlacement->Y , 1010.0f );
    for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
    {
        if ( OppRobot[i].seenState != CompletelyOut )
        {
            AddCircle ( OppRobot[i].Position.X , OppRobot[i].Position.Y , 300.0 );
        }
    }
    OwnRobot[rw].face(ball.Position);
    ERRTNavigate2Point ( rw ,Vec2(0,-100) + PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( lw , true, true , true , true, false);
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    AddCircle ( targetBallPlacement->X , targetBallPlacement->Y , 1010.0f );
    for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
    {
        if ( OppRobot[i].seenState != CompletelyOut )
        {
            AddCircle ( OppRobot[i].Position.X , OppRobot[i].Position.Y , 300.0 );
        }
    }
    OwnRobot[lw].face(ball.Position);
    ERRTNavigate2Point ( lw ,Vec2(0,100) + PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);


    ERRTSetObstacles ( lmf , true, true , true , true, false);
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    AddCircle ( targetBallPlacement->X , targetBallPlacement->Y , 1010.0f );
    for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
    {
        if ( OppRobot[i].seenState != CompletelyOut )
        {
            AddCircle ( OppRobot[i].Position.X , OppRobot[i].Position.Y , 300.0 );
        }
    }
    OwnRobot[lmf].face(Vec2(ball.Position.X,ball.Position.Y));
    ERRTNavigate2Point ( lmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*field_width,0))),1090) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( rmf , true, true , true , true, false);
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    AddCircle ( targetBallPlacement->X , targetBallPlacement->Y , 1010.0f );
    for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
    {
        if ( OppRobot[i].seenState != CompletelyOut )
        {
            AddCircle ( OppRobot[i].Position.X , OppRobot[i].Position.Y , 300.0 );
        }
    }
    OwnRobot[rmf].face(Vec2(ball.Position.X,ball.Position.Y));
    ERRTNavigate2Point ( rmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*field_width,0))),1090) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( cmf , true, true , true , true, false);
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    AddCircle ( targetBallPlacement->X , targetBallPlacement->Y , 1010.0f );
    for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
    {
        if ( OppRobot[i].seenState != CompletelyOut )
        {
            AddCircle ( OppRobot[i].Position.X , OppRobot[i].Position.Y , 300.0 );
        }
    }
    OwnRobot[cmf].face(Vec2(ball.Position.X,ball.Position.Y));
    ERRTNavigate2Point ( cmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(AngleWith(ball.Position , Vec2(side*field_width,0))),1090) ,0 , 100,&VELOCITY_PROFILE_AROOM);
    //side=-side;
}

