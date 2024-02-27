#include "obstacle.h"
#include "../../../Common/distance.h"

#define world2mapX(a) min(729,max(0,((a/10)+364)))
#define world2mapY(a) min(529,max(0,((a/10)+264)))

/*bool obs_lut[605][405];

void clear_map ( void )
{
        for ( int i = 0 ; i < 605 ; i ++ )
                for ( int j = 0 ; j < 405 ; j ++ )
                {
                        obs_lut[i][j] = false;
                }
}

bool IsInObstacle ( TVec2 p )
{
        return obs_lut[(int)p.X][(int)p.Y];
}

/*bool collisionDetect ( TVec2 p1 , TVec2 p2 )
{
        float coss , sinn;
        coss = ( p2.X - p1.X ) / DIS ( p1 , p2 );
        sinn = ( p2.Y - p1.Y ) / DIS ( p1 , p2 );

        TVec2 current = p1;

        while ( DIS ( current , p2 ) > 9 )
        {
                if ( IsInObstacle ( current ) )
                        return true;

                current.X += coss * 9;
                current.Y += sinn * 9;
        }

        return false;
}*/

/*void AddCircle ( int x , int y , int r )
{
	x = world2mapX(x);
	y = world2mapY(y);
        for ( int i = 0 ; i < 605 ; i ++ )
                for ( int j = 0 ; j < 405 ; j ++ )
                {
                        //obs_lut[i][j] = false;
                        //if ( (i-x) * (i-x) + (j-y) * (j-y) < r * r )	obs_lut[i][j] = true;
                }
}

void AddRectangle ( int x , int y , int w , int h )
{
	x = world2mapX(x);
	y = world2mapY(y);
	for ( int i = x ; i < x+w ; i ++ )
		for ( int j = y ; j < y+h ; j ++ )
		{
			//obs_lut[i][j] = true;
		}
}*/


ObsMap map ( 100 );

void clear_map ( void )
{
        map.resetMap();
}

bool IsInObstacle ( TVec2 p )
{
        return map.IsInObstacle ( p.X , p.Y );
}

bool collisionDetect ( TVec2 p1 , TVec2 p2 )
{
        float coss , sinn;
        coss = ( p2.X - p1.X ) / DIS ( p1 , p2 );
        sinn = ( p2.Y - p1.Y ) / DIS ( p1 , p2 );

        TVec2 current = p1;

        while ( DIS ( current , p2 ) > 10.0f )
        {
                if ( IsInObstacle ( current ) )
                        return true;

                current.X += coss * 10.0f;
                current.Y += sinn * 10.0f;
        }

        return false;
}

/*bool collisionDetect ( TVec2 p1 , TVec2 p2 )
{
        if ( map.getObsNum() == 0 )
            return false;
        float coss , sinn , dis;
        dis = DIS ( p1 , p2 );

        if ( dis < 0.1 )
                return false;

        coss = ( p2.X - p1.X ) / dis;
        sinn = ( p2.Y - p1.Y ) / dis;

        TVec2 current = p1;
        float advance_dis = 1;

        for ( int i = 0 ; DIS ( current , p2 ) > 2 ; i ++ )
        {
                if ( i > 10 )
                    return true;

                if ( IsInObstacle ( current ) )
                        return true;
                advance_dis = min ( max ( 1 , map.NearestDistance(current.X,current.Y) ) , DIS ( current , p2 ) );
                if ( advance_dis <= 0 )
                    return true;

                current.X += coss * advance_dis;
                current.Y += sinn * advance_dis;
        }

        return false;
}*/

void AddCircle ( float x , float y , float r )
{
    map.AddCircle ( x , y , r );
}

void AddRectangle ( float x , float y , float w , float h )
{
	map.AddRectangle(x,y,w,h);
}
