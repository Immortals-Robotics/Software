#include "helpers.h"
#include <math.h>

float max(float a, float b)
{
	return (a > b) ? a : b;
}

float min(float a, float b)
{
	return (a < b) ? a : b;
}

int sgn(float num)
{
	if (num > 0) return(1);
	else if (num < 0) return(-1);
	else return(0);
}

bool sgnBool(float num)
{
	if (num >= 0) return(false);
	else return(true);
}

float AngleWith(TVec2 a,TVec2 b){
		float ans;
		ans=atan((b.Y-a.Y)/(b.X-a.X));
		ans*=(float)180.0/(float)3.1415;
		if(b.X-a.X<0)
			ans+=180;
		while(ans>180)
			ans-=360;
		while(ans<-180)
			ans+=360;
		return ans;
	}

float dis(float x1,float y1,float x2,float y2){
		return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
	}

float NormalizeAngle(float teta){
	if(teta>180)
		teta-=360;
	if(teta<-180)
		teta+=360;
	return teta;
}

TVec2 PointOnConnectingLine(TVec2 FirstPoint,TVec2 SecondPoint,float distance){
	float m = (SecondPoint.Y-FirstPoint.Y)/(SecondPoint.X-FirstPoint.X);
	TVec2 ans;
	if(SecondPoint.X-FirstPoint.X>0)
		ans.X = FirstPoint.X + distance/sqrt(pow(m,2)+1);
	else
		ans.X = FirstPoint.X - distance/sqrt(pow(m,2)+1);
	ans.Y = FirstPoint.Y + m*(ans.X-FirstPoint.X);
	return ans;
}

TVec2 CircleAroundPoint(TVec2 point,float angle,float radius){
	angle = NormalizeAngle(angle) * 3.1415 / 180.0;
	TVec2 ans = Vec2(point.X + radius * cos(angle),point.Y + radius * sin(angle));
	return ans;
}