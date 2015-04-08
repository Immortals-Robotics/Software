#include "ai09.h"

float normalizeAngleR( float angle )//radian
{
	while( angle > 3.1415  ) angle -= 2*3.1415;
	while( angle < -3.1415 ) angle += 2*3.1415;
	
	return ( angle );
}

TVec2 ai09::calculateOpenAngleToGoal(TVec2 p1,int robot_num)
{
	
	int counter=0;
    bool oops=0;
	int obsCount=0;
	int obs[50][2];    
	TVec2 t2 = Vec2 ( -field_width*side , goal_width/2.0 );
	TVec2 t1 = Vec2 ( -field_width*side , -goal_width/2.0 );
    TVec2 MidGoal = Vec2(-field_width*side , 0);
    float midGoalAngel;    
    float t1Angel;
    float t2Angel;
    midGoalAngel = atan2((MidGoal.Y - p1.Y), (MidGoal.X - p1.X));
    t1Angel = atan2((t1.Y - p1.Y), (t1.X - p1.X));
    t2Angel = atan2((t2.Y - p1.Y), (t2.X - p1.X));
	
	if(fabs(t1Angel - t2Angel)>3.1415)
        oops=1;
	
    if(t1Angel > t2Angel)
    {
        float tmp = t1Angel;
        t1Angel = t2Angel;
        t2Angel = tmp;
    }
    if(oops)
    {
        float tmp = t1Angel;
        t1Angel = t2Angel;
        t2Angel = tmp;
    }
    
    double d[100][2];
    double dtmp[2] , ttmp[2];
	
	// Set Obstacles //////////////////////////////////////////////
	
	for(int i=0;i<12;i++)
	{
		if((OwnRobot[i].State.seenState!=CompletelyOut)&&(i!=robot_num))
		{
			obs[obsCount][0]=OwnRobot[i].State.Position.X;
			obs[obsCount][1]=OwnRobot[i].State.Position.Y;
			obsCount++;
		}
	}
	for(int i=0;i<12;i++)
	{
		if(OppRobot[i].seenState!=CompletelyOut)
		{
			obs[obsCount][0]=OppRobot[i].Position.X;
			obs[obsCount][1]=OppRobot[i].Position.Y;
			obsCount++;
		}
	}
	
	///////////////////////////////////////////////////////////////
	
    for(int i=0;i<obsCount;i++)
    {
		
		d[counter][0] = atan2((obs[i][1] - p1.Y) ,obs[i][0] - p1.X);
		
		double dd =atan(90/DIS(p1.X,p1.Y,obs[i][0],obs[i][1]));
		if(dd<0)
			dd=-dd;
		d[counter][1] = d[counter][0] + dd;
		d[counter][0] = d[counter][0] - dd;
		
		d[counter][0] =  normalizeAngleR(d[counter][0]);
		d[counter][1] =  normalizeAngleR(d[counter][1]);
				
		dtmp[0]=d[counter][0];
		dtmp[1]=d[counter][1];
		ttmp[0]=t1Angel;
		ttmp[1]=t2Angel;
		
		if(oops)
		{
			if(dtmp[0]<0)
				dtmp[0]+=2*3.1415;
			if(dtmp[1]<0)
				dtmp[1]+=2*3.1415;
			if(ttmp[0]<0)
				ttmp[0]+=2*3.1415;
			if(ttmp[1]<0)
				ttmp[1]+=2*3.1415;
		}
		
		// No Free Angel
		if(dtmp[1] > ttmp[1] && dtmp[0]<ttmp[0])
			return Vec2(midGoalAngel*180.0f/3.1415f , 0 );
		
		if(dtmp[0] > ttmp[1] || dtmp[1]<ttmp[0])
		{
			d[counter][0]=0;
			d[counter][1]=0;
			continue;
		}
		
		
		if(dtmp[0]<ttmp[0])
			d[counter][0]=t1Angel;
		
		if(dtmp[1]>ttmp[1])
			d[counter][1]=t2Angel;
		
		counter++;
    }
	
    // Completely Free
    if(counter==0)
    {
		return Vec2(midGoalAngel*180.0f/3.1415f , fabs(t2Angel - t1Angel)*180.0f/3.1415f);
    }
	
    float step = (t2Angel - t1Angel)/100.0f;
    if(oops)
    {
        step +=2*3.1415/100.0;
    }
    bool freeAngel [100];
    for(int i=0 ; i<100 ; i++)
    {
        float ang = (t1Angel+i*step);
        for(int j=0;j<counter;j++)
        {
            dtmp[0]=d[j][0];
            dtmp[1]=d[j][1];
            if(oops)
            {
                if(dtmp[0]<0)
                    dtmp[0]+=2*3.1415;
                if(dtmp[1]<0)
                    dtmp[1]+=2*3.1415;
                if(ang<0)
                    ang+=2*3.1415;
            }
			
            if(ang >=dtmp[0] && ang <dtmp[1])
            {
                freeAngel[i]=1;
                //i=(d[j][1]-t1Angel)/step;
                break;
            }
            else
                freeAngel[i]=0;
			
        }
		
    }
    int maxFree=0;
    int freeCount=0;
    float max=0;
    for(int i=0;i<100;i++)
    {
        if(freeAngel[i]==1)
        {            
            if(freeCount>maxFree)
            {
                maxFree=freeCount;
                max = i - maxFree/2;
            }
            freeCount=0;
        }
        else
        {
            freeCount++;
		}
    }
	
    if(freeCount>maxFree)
    {
        maxFree=freeCount;
        max = 100 - maxFree/2;;
    }
    max = t1Angel + max*step;
    max = normalizeAngleR(max);
	
	//cout << "	Salam Olaghe aziz :	" << maxFree << endl;
	
	if ( maxFree == 0 )
		return Vec2(midGoalAngel*180.0f/3.1415f , 0 );
	return Vec2( max*180.0f/3.1415f , (maxFree*180.0f/3.1415f)*step );
	
	//freeAngleFilter[robot_num].AddData(finalAns.X);
	//finalAns.X = freeAngleFilter[robot_num].GetCurrent();
	
	//return finalAns;
    
}