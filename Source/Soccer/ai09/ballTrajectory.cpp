#include "ai09.h"

using namespace std;

void ai09::CalculateBallTrajectory ( void )
{
	if ( ball.velocity.magnitude == 0 )
		return;
	
	if (ballHist.size()<5) {
		return;
	}
	
	static MedianFilter<float> visionM;
	if ( ball.velocity.x == 0 )
		visionM.AddData(100.0f);
	else
		visionM.AddData(fabs(ball.velocity.y/ball.velocity.x));
	
	bool isVertical = visionM.GetCurrent() > 1.0f;
	
	vector<float> ball_x;
	vector<float> ball_y;
	MedianFilter<float> dMedian(5);
	
	int i;
	for (i = ballHist.size()-1; i>=0; i--) {
		if (ball_x.size()>1)
		{
			float d;
			if (isVertical) {
				ballLine.calculate(ball_x.size(), &ball_y[0], &ball_x[0]);
				d = ballLine.getDisToPoint(Vec2(ballHist[i].Position.Y,ballHist[i].Position.X));
			}
			else {
				ballLine.calculate(ball_x.size(), &ball_x[0], &ball_y[0]);
				d = ballLine.getDisToPoint(ballHist[i].Position);
			}

			dMedian.AddData(d);
			if ( ( dMedian.GetCurrent()> 50)&&(ballHist.size()-i>5)) {
				break;
			}
		}
		ball_x.push_back(ballHist[i].Position.X);
		ball_y.push_back(ballHist[i].Position.Y);
	}
	
	i++;
	
	if ( isVertical )
		ballLine.chapeKon();
	
	//ball_line.calculate(ball_x.size(), &ball_x[0], &ball_y[0]);
	if (ballLine.isAmoodi()) {
		AddDebugLine(Vec2(ballLine.getXIntercept(), -2000),Vec2(ballLine.getXIntercept(), 2000),Purple);
	}
	AddDebugLine(Vec2(ball.Position.X, ballLine.getValue(ball.Position.X)),Vec2(ballHist[i].Position.X, ballLine.getValue(ballHist[i].Position.X)));
	
	if (!ballLine.isAmoodi())
	{
		Line new_line ( 1.0 , -ballLine.getSlope() , -ballLine.getIntercept() );
		VecPosition ballN,lastN;
		ballN = new_line.getPointOnLineClosestTo(VecPosition(ball.Position.X,ball.Position.Y));
		lastN = new_line.getPointOnLineClosestTo(VecPosition(ballHist[i].Position.X,ballHist[i].Position.Y));
		AddDebugLine(Vec2(ballN.getX(),ballN.getY()),Vec2(lastN.getX(),lastN.getY()),isVertical?Yellow:Red);
	}
	
	for (i = max ( ballHist.size() - 60 , 0 ) ; i < ballHist.size(); i +=10) {
		//AddDebugCircle(ballHist[i].Position,10+10.0*(float(i)/float(ballHist.size())),Red);
	}
	
	//AddDebugCircle(Vec2(0, 0),1000,Blue);
	
	//AddDebugLine(ballHist.front().Position,ballHist.back().Position,Blue);
}