//
// Created by dot_blue on 6/10/18.
//

#include "ai09.h"

#define DEF_VEL_PROFILE (stop ? &VELOCITY_PROFILE_AROOM : &VELOCITY_PROFILE_MAMOOLI)

void ai09::runningDef(int robot_num,TVec2 target,TVec2 * defendTarget ,bool stop) {


    const float max_def_move_to_intercept = 400.0f;
    const float max_near_asshole_dis = 750.0f;
    const float max_ball_handler_asshole_dis = 400.0f;
    const float max_shoot_blocker_dis = 1500.0f;
    const float shoot_blocker_r = 200.0f;
    const float max_fast_ball_spd = 5000.0f;
    const float max_own_attacker_dis = 400.0f;

    bool ballAriving = false;
    bool oneTouchNear = false;
    bool interceptNear = false;
    bool assholeNear = false;
    bool assholeHasBall = false;
    bool oppGoalOpen = false;
    bool ballMovingFast = false;
    bool ownAttackHasBall = false;
    bool ballIsToGoal = false;

    ballAriving = oneTouchDetector[robot_num].IsArriving(40,80);

    TVec2 oneTouchPos = CalculatePassPos(robot_num,Vec2(-side*field_width, 0),OwnRobot[robot_num].State.Position);

    if (DIS(oneTouchPos, target) < max_def_move_to_intercept ) {
        oneTouchNear = true;
    }

    if (DIS(ball.Position, target) < max_def_move_to_intercept ) {
        interceptNear = true;
    }

    int nearestAsshole = findNearestAsshole(target, -1, true);
    assholeNear = DIS ( OppRobot[nearestAsshole].Position , target ) < max_near_asshole_dis;

    int ballHandlerAsshole = findNearestAsshole(ball.Position, -1, true);
    assholeHasBall = DIS ( OppRobot[ballHandlerAsshole].Position , ball.Position ) < max_ball_handler_asshole_dis;


    ballMovingFast = ball.velocity.magnitude > max_fast_ball_spd;

    ownAttackHasBall = DIS ( OwnRobot[attack].State.Position , ball.Position ) < max_own_attacker_dis;
    if (OwnRobot[attack].State.seenState == CompletelyOut) {
        ownAttackHasBall = false;
    }

    ballIsToGoal = ballIsGoaling();
    ballIsToGoal = false;

    /*cout << "interceptNear:	"<<interceptNear <<endl;
    cout << "assholeHasBall: " << assholeHasBall<<endl;
    cout << "ballMovingFast: " << ballMovingFast <<endl;
    cout << "ownAttackHasBall: " << ownAttackHasBall<<endl;
    cout << "ballMovingFast: " << ballMovingFast <<endl;
    cout << "ballIsToGoal: " << ballIsToGoal << endl;
    cout << "gkIntercepting: "<< gkIntercepting << endl;*/

    if (
            ( interceptNear ) &&
            ( !assholeHasBall ) &&
            ( !ballMovingFast ) &&
            ( !ownAttackHasBall ) &&
            ( !ballIsToGoal ) &&
            ( !gkIntercepting ) &&
            ( !stop )
            )
    {
        ERRTSetObstacles ( robot_num , 0 , 1 , 1 , 0 , 0 );
        //tech_circle(robot_num,sgn(ball.Position.Y)*side*60 ,0,15,false);
        tech_circle(robot_num,AngleWith ( ball.Position , Vec2 ( side * (field_width+110) , 0 ) ) ,0,80,true,0,0,0);
    }
    else {
        ERRTSetObstacles ( robot_num , stop , true , true , false );
        OwnRobot[robot_num].face ( Vec2 ( (*defendTarget).X , (*defendTarget).Y ) );
        ERRTNavigate2Point(robot_num, target, 0, 100, DEF_VEL_PROFILE);
    }
}

void ai09::DefBy1(int thelastdef_num, TVec2 *defendTarget, bool stop){
    if (thelastdef_num != -1)
    {
        double alpha = NormalizeAngle(AngleWith(Vec2(side * field_width, 0), ball.Position) + (90 + side * 90));
        alpha = min(90, max(-90, alpha));
        int alphaSgn = sgn(alpha);

        if (fabs(alpha) < 45.0)
        {
            Line ball_line = Line::makeLineFromTwoPoints(VecPosition(ball.Position.X, ball.Position.Y),
                                                         VecPosition(side * field_width, 0.0));
            Line Front_line = Line::makeLineFromPositionAndAngle(
                    VecPosition(side * (field_width - penalty_area_r - 100), 0), 90.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 target = Vec2(ans.getX(), ans.getY());

            runningDef(thelastdef_num, target, defendTarget, stop);
        } else if (alpha > 85.0)
        {
            Line ball_line = Line::makeLineFromPositionAndAngle(VecPosition(side * field_width, 0.0), 85.0);
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, -side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 fans = Vec2(ans.getX(), ans.getY());

            OwnRobot[thelastdef_num].target.Angle = 85.0 + 90 + side * 90;
            ERRTSetObstacles(thelastdef_num, stop, 1, 1, 0, 0, 0);
            ERRTNavigate2Point(thelastdef_num, fans, 1, 100, DEF_VEL_PROFILE);
        } else if (alpha >= 45.0)
        {
            Line ball_line = Line::makeLineFromTwoPoints(VecPosition(ball.Position.X, ball.Position.Y),
                                                         VecPosition(side * field_width, 0.0));
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, -side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 target = Vec2(ans.getX(), ans.getY());

            runningDef(thelastdef_num, target, defendTarget, stop);
        } else if (alpha < -85.0)
        {
            Line ball_line = Line::makeLineFromPositionAndAngle(VecPosition(side * field_width, 0.0), -85.0);
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 fans = Vec2(ans.getX(), ans.getY());

            OwnRobot[thelastdef_num].target.Angle = -85.0 + 90 + side * 90;
            ERRTSetObstacles(thelastdef_num, stop, 1, 1, 0, 0, 0);
            ERRTNavigate2Point(thelastdef_num, fans, 1, 100, DEF_VEL_PROFILE);
        } else if (alpha <= -45.0)
        {
            Line ball_line = Line::makeLineFromTwoPoints(VecPosition(ball.Position.X, ball.Position.Y),
                                                         VecPosition(side * field_width, 0.0));
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 target = Vec2(ans.getX(), ans.getY());

            runningDef(thelastdef_num, target, defendTarget, stop);
        }
    }
}

void ai09::DefBy2(int rightdef_num ,int leftdef_num, TVec2 * defendTarget , bool stop){
    double alpha = NormalizeAngle(AngleWith(Vec2(side * field_width, 0), ball.Position) + (90 +side*90));
    alpha = min(90, max(-90, alpha));
    int alphaSgn = sgn(alpha);

    if (rightdef_num != -1)
    {
        //rightdef_num
        if (alpha < -85.0)
        {
            Line ball_line = Line::makeLineFromPositionAndAngle(VecPosition(side * field_width, 0.0), -85.0);
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 fans = Vec2(ans.getX(), ans.getY());

            OwnRobot[rightdef_num].target.Angle = -85.0 + 90 + side * 90;
            ERRTSetObstacles(rightdef_num, stop, 1, 1, 0, 0, 0);
            ERRTNavigate2Point(rightdef_num, fans, 1, 100, DEF_VEL_PROFILE);
        } else if (alpha < -48.0)
        {
            Line ball_line = Line::makeLineFromTwoPoints(VecPosition(ball.Position.X, ball.Position.Y),
                                                         VecPosition(side * field_width, 0.0));
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 target = Vec2(ans.getX(), ans.getY());

            runningDef(rightdef_num, target, defendTarget, stop);
        } else if (alpha < -3.5)
        {
            Line ball_line = Line::makeLineFromTwoPoints(VecPosition(ball.Position.X, ball.Position.Y),
                                                         VecPosition(side * field_width, 0.0));
            Line Front_line = Line::makeLineFromPositionAndAngle(
                    VecPosition(side * (field_width - penalty_area_r - 100), 0), 90.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 target = Vec2(ans.getX(), ans.getY());

            runningDef(rightdef_num, target, defendTarget, stop);
        } else
        {
            Line ball_line = Line::makeLineFromPositionAndAngle(VecPosition(side * field_width, 0.0), -3.5);
            Line Front_line = Line::makeLineFromPositionAndAngle(
                    VecPosition(side * (field_width - penalty_area_r - 100), 0), 90.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 fans = Vec2(ans.getX(), ans.getY());

            OwnRobot[rightdef_num].target.Angle = 90 + side * 90;
            ERRTSetObstacles(rightdef_num, stop, 1, 1, 0, 0, 0);
            ERRTNavigate2Point(rightdef_num, fans, 1, 100, DEF_VEL_PROFILE);
        }
    }

    if (leftdef_num != -1)
    {
        //leftdef_num
        if (alpha > 85.0)
        {
            Line ball_line = Line::makeLineFromPositionAndAngle(VecPosition(side * field_width, 0.0), 85.0);
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, -side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 fans = Vec2(ans.getX(), ans.getY());

            OwnRobot[leftdef_num].target.Angle = 85.0 + 90 + side * 90;
            ERRTSetObstacles(leftdef_num, stop, 1, 1, 0, 0, 0);
            ERRTNavigate2Point(leftdef_num, fans, 1, 100, DEF_VEL_PROFILE);
        } else if (alpha > 48.0)
        {
            Line ball_line = Line::makeLineFromTwoPoints(VecPosition(ball.Position.X, ball.Position.Y),
                                                         VecPosition(side * field_width, 0.0));
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, -side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 target = Vec2(ans.getX(), ans.getY());

            runningDef(leftdef_num, target, defendTarget, stop);
        } else if (alpha > 3.5)
        {
            Line ball_line = Line::makeLineFromTwoPoints(VecPosition(ball.Position.X, ball.Position.Y),
                                                         VecPosition(side * field_width, 0.0));
            Line Front_line = Line::makeLineFromPositionAndAngle(
                    VecPosition(side * (field_width - penalty_area_r - 100), 0), 90.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 target = Vec2(ans.getX(), ans.getY());

            runningDef(leftdef_num, target, defendTarget, stop);
        } else
        {
            Line ball_line = Line::makeLineFromPositionAndAngle(VecPosition(side * field_width, 0.0), 3.5);
            Line Front_line = Line::makeLineFromPositionAndAngle(
                    VecPosition(side * (field_width - penalty_area_r - 100), 0), 90.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 fans = Vec2(ans.getX(), ans.getY());

            OwnRobot[leftdef_num].target.Angle = 90 + side * 90;
            ERRTSetObstacles(leftdef_num, stop, 1, 1, 0, 0, 0);
            ERRTNavigate2Point(leftdef_num, fans, 1, 100, DEF_VEL_PROFILE);
        }
    }
}

void ai09::DefBy3 ( int middef_num ,int rightdef_num ,int leftdef_num , TVec2 * defendTarget , bool stop ) {
    double alpha = NormalizeAngle(AngleWith(Vec2(side * field_width, 0), ball.Position) + (90 +side*90));
    alpha = min(90, max(-90, alpha));
    int alphaSgn = sgn(alpha);


    if (middef_num != -1)
    {
        if (fabs(alpha) < 43.0)
        {
            Line Front_line = Line::makeLineFromPositionAndAngle(
                    VecPosition(side * (field_width - penalty_area_r - 100), 0), 90.0);
            Line ball_line = Line::makeLineFromTwoPoints(VecPosition(ball.Position.X, ball.Position.Y),
                                                         VecPosition(side * field_width, 0.0));
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 target = Vec2(ans.getX(), ans.getY());

            runningDef(middef_num, target, defendTarget, stop);
        } else
        {
            Line ball_line = Line::makeLineFromPositionAndAngle(VecPosition(side * field_width, 0.0), alphaSgn * 43.0);
            Line Front_line = Line::makeLineFromPositionAndAngle(
                    VecPosition(side * (field_width - penalty_area_r - 100), 0), 90.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 fans = Vec2(ans.getX(), ans.getY());

            OwnRobot[middef_num].target.Angle = alphaSgn * 43.0 + 90 + side * 90;
            ERRTSetObstacles(middef_num, stop, 1, 1, 0, 0, 0);
            ERRTNavigate2Point(middef_num, fans, 1, 100, DEF_VEL_PROFILE);
        }
    }

    if (rightdef_num != -1)
    {
        //rightdef_num
        if (alpha < -85.0)
        {
            Line ball_line = Line::makeLineFromPositionAndAngle(VecPosition(side * field_width, 0.0), -85.0);
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 fans = Vec2(ans.getX(), ans.getY());

            OwnRobot[rightdef_num].target.Angle = -85.0 + 90 + side * 90;
            ERRTSetObstacles(rightdef_num, stop, 1, 1, 0, 0, 0);
            ERRTNavigate2Point(rightdef_num, fans, 1, 100, DEF_VEL_PROFILE);
        } else if (alpha < -48.0)
        {
            Line ball_line = Line::makeLineFromTwoPoints(VecPosition(ball.Position.X, ball.Position.Y),
                                                         VecPosition(side * field_width, 0.0));
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 target = Vec2(ans.getX(), ans.getY());

            runningDef(rightdef_num, target, defendTarget, stop);
        } else
        {
            Line ball_line = Line::makeLineFromPositionAndAngle(VecPosition(side * field_width, 0.0), -48.0);
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 fans = Vec2(ans.getX(), ans.getY());

            OwnRobot[rightdef_num].target.Angle = -48.0 + 90 + side * 90;
            ERRTSetObstacles(rightdef_num, stop, 1, 1, 0, 0, 0);
            ERRTNavigate2Point(rightdef_num, fans, 1, 100, DEF_VEL_PROFILE);
        }
    }

    if (leftdef_num != -1)
    {
        //leftdef_num
        if (alpha > 85.0)
        {
            Line ball_line = Line::makeLineFromPositionAndAngle(VecPosition(side * field_width, 0.0), 85.0);
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, -side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 fans = Vec2(ans.getX(), ans.getY());

            OwnRobot[leftdef_num].target.Angle = 85.0 + 90 + side * 90;
            ERRTSetObstacles(leftdef_num, stop, 1, 1, 0, 0, 0);
            ERRTNavigate2Point(leftdef_num, fans, 1, 100, DEF_VEL_PROFILE);
        } else if (alpha > 48.0)
        {
            Line ball_line = Line::makeLineFromTwoPoints(VecPosition(ball.Position.X, ball.Position.Y),
                                                         VecPosition(side * field_width, 0.0));
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, -side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 target = Vec2(ans.getX(), ans.getY());

            runningDef(leftdef_num, target, defendTarget, stop);
        } else
        {
            Line ball_line = Line::makeLineFromPositionAndAngle(VecPosition(side * field_width, 0.0), 48.0);
            Line Front_line = Line::makeLineFromPositionAndAngle(VecPosition(0, -side * (penalty_area_r + 100)), 0.0);
            VecPosition ans = ball_line.getIntersection(Front_line);
            TVec2 fans = Vec2(ans.getX(), ans.getY());

            OwnRobot[leftdef_num].target.Angle = 48.0 + 90 + side * 90;
            ERRTSetObstacles(leftdef_num, stop, 1, 1, 0, 0, 0);
            ERRTNavigate2Point(leftdef_num, fans, 1, 100, DEF_VEL_PROFILE);
        }
    }
}

void ai09::DefMid ( int &middef_num ,int &rightdef_num ,int &leftdef_num , TVec2 * defendTarget , bool stop , bool replace ) {

    double alpha = AngleWith(Vec2(side * field_width, 0), ball.Position);
    alpha = min(90, max(-90, alpha));
    int alphaSgn = sgn(alpha);
    cout<<"ALPHA: "<<alpha<<endl;

    if (!defendTarget)
        defendTarget = &(ball.Position);

    //make sure the def is present:
    if(OwnRobot[middef_num].State.seenState == CompletelyOut && replace){
        if(OwnRobot[rightdef_num].State.seenState != CompletelyOut){
            swap(middef_num,rightdef_num);
        }else if(OwnRobot[leftdef_num].State.seenState != CompletelyOut){
            swap(middef_num,leftdef_num);
        }else{//We're busted...

        }
    }

    bool leftdef_available = OwnRobot[leftdef_num].State.seenState != CompletelyOut && markMap[&leftdef_num] == -1;
    bool rightdef_available = OwnRobot[rightdef_num].State.seenState != CompletelyOut && markMap[&leftdef_num] == -1;

    DefHi(middef_num, defendTarget, stop);

    if(!leftdef_available && !rightdef_available)
    {
        DefBy1(-1,defendTarget,stop);
    }
    else if(!rightdef_available)
    {
        DefBy2(-1,leftdef_num,defendTarget,stop);
    }
    else if(!leftdef_available)
    {
        DefBy2(rightdef_num,-1,defendTarget,stop);
    }
    else
    {
        DefBy3(-1,rightdef_num,leftdef_num,defendTarget,stop);
    }

//    cout<<"the alpha: "<<alpha<<endl;

}
