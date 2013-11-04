#pragma once

class Line
{
	private:
		float x , y;
		float m;

	public:
		void Construct ( float _x , float _y , float _m );
		void Construct ( float _x1 , float _y1 , float _x2 , float _y2 );
};