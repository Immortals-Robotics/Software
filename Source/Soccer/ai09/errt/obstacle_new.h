#pragma once
//#include "line.h"
#include <stdlib.h>

class BaseObstacle
{
	public:
		virtual bool IsInObstacle ( float _x , float _y ) = 0;
                virtual float NearestDistance ( float _x , float _y ) = 0;
};

class CircleObstacle : public BaseObstacle
{
	private:
		float x , y;
		float r;

	public:
		CircleObstacle ( float _x , float _y , float _r );
		bool IsInObstacle ( float _x , float _y );
                float NearestDistance ( float _x , float _y );
};

class RectangleObstacle : public BaseObstacle
{
	private:
		float x , y;
		float w , h;

	public:
		RectangleObstacle ( float _x , float _y , float _w , float _h );
		bool IsInObstacle ( float _x , float _y );
                float NearestDistance ( float _x , float _y );
};

class HalfPlaneObstacle : public BaseObstacle
{
	private:
		float w , h;

	public:
		HalfPlaneObstacle ( float _x , float _y , float _w , float _h );
		bool IsInObstacle ( float _x , float _y );
                float NearestDistance ( float _x , float _y );
};

class ObsMap
{
	private:
		unsigned int obsNum , maxObs;

		BaseObstacle ** obstacle;

	public:
		ObsMap ( unsigned int _maxObs );

		void AddCircle ( float _x , float _y , float _r );
		void AddRectangle ( float _x , float _y , float _w , float _h );
		//void AddHalfPlane ( float _x , float _y , float _w , float _h );
		void AddObstacle ( BaseObstacle * obs );

		bool IsInObstacle ( float _x , float _y );
                float NearestDistance ( float _x , float _y );


		void resetMap ( void );

		int getObsNum ( void );
};
