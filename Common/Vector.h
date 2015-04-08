#pragma once

struct TVec2
{
	float X;
	float Y;
};

struct TVec3
{
	float X;
	float Y;
	float Z;
};

TVec2 Vec2 ( float X , float Y );
TVec2 Vec2 ( float a = 0.0f );
TVec3 Vec3 ( float X , float Y , float Z );
TVec3 Vec3 ( float a = 0.0f );

TVec2 operator + ( const TVec2 & a , const TVec2 & b );
TVec2 operator - ( const TVec2 & a , const TVec2 & b );
TVec2 operator * ( const TVec2 & a , const TVec2 & b );
TVec2 operator / ( const TVec2 & a , const TVec2 & b );
TVec2 operator * ( const TVec2 & a , const float b );
TVec2 operator / ( const TVec2 & a , const float b );
TVec2 operator += ( TVec2 & a , const TVec2 & b );
TVec2 operator -= ( TVec2 & a , const TVec2 & b );
TVec2 operator *= ( TVec2 & a , const TVec2 & b );
TVec2 operator /= ( TVec2 & a , const TVec2 & b );
TVec2 operator *= ( TVec2 & a , const float b );
TVec2 operator /= ( TVec2 & a , const float b );

TVec3 operator + ( const TVec3 & a , const TVec3 & b );
TVec3 operator - ( const TVec3 & a , const TVec3 & b );
TVec3 operator * ( const TVec3 & a , const TVec3 & b );
TVec3 operator / ( const TVec3 & a , const TVec3 & b );
TVec3 operator * ( const TVec3 & a , const float b );
TVec3 operator / ( const TVec3 & a , const float b );
TVec3 operator += ( TVec3 & a , const TVec3 & b );
TVec3 operator -= ( TVec3 & a , const TVec3 & b );
TVec3 operator *= ( TVec3 & a , const TVec3 & b );
TVec3 operator /= ( TVec3 & a , const TVec3 & b );
TVec3 operator *= ( TVec3 & a , const float b );
TVec3 operator /= ( TVec3 & a , const float b );

TVec2 Normalize ( const TVec2& a );
TVec3 Normalize ( const TVec3& a );

float Magnitude ( const TVec2& a );
float Magnitude ( const TVec3& a );


float Dot ( const TVec2& a , const TVec2& b );
float Dot ( const TVec3& a , const TVec3& b );