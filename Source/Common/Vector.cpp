
#include "Vector.h"
#include <math.h>

TVec2 Vec2 ( float X , float Y )
{
	TVec2 ans;
	ans.X = X;
	ans.Y = Y;
	return ans;
}

TVec2 Vec2 ( float a )
{
	TVec2 ans;
	ans.X = a;
	ans.Y = a;
	return ans;
}

TVec3 Vec3 ( float X , float Y , float Z )
{
	TVec3 ans;
	ans.X = X;
	ans.Y = Y;
	ans.Z = Z;
	return ans;
}

TVec3 Vec3 ( float a )
{
	TVec3 ans;
	ans.X = a;
	ans.Y = a;
	ans.Z = a;
	return ans;
}

TVec2 operator + ( const TVec2 & a , const TVec2 & b )
{
	return Vec2 ( a.X + b.X , a.Y + b.Y );
}

TVec2 operator - ( const TVec2 & a , const TVec2 & b )
{
	return Vec2 ( a.X - b.X , a.Y - b.Y );
}

TVec2 operator * ( const TVec2 & a , const TVec2 & b )
{
	return Vec2 ( a.X * b.X , a.Y * b.Y );
}

TVec2 operator / ( const TVec2 & a , const TVec2 & b )
{
	return Vec2 ( a.X / b.X , a.Y / b.Y );
}

TVec2 operator * ( const TVec2 & a , const float b )
{
	return Vec2 ( a.X * b , a.Y * b );
}

TVec2 operator / ( const TVec2 & a , const float b )
{
	return Vec2 ( a.X / b , a.Y / b );
}

TVec2 operator += ( TVec2 & a , const TVec2 & b )
{
	a.X += b.X;
	a.Y += b.Y;

	return a;
}

TVec2 operator -= ( TVec2 & a , const TVec2 & b )
{
	a.X -= b.X;
	a.Y -= b.Y;

	return a;
}

TVec2 operator *= ( TVec2 & a , const TVec2 & b )
{
	a.X *= b.X;
	a.Y *= b.Y;

	return a;
}

TVec2 operator /= ( TVec2 & a , const TVec2 & b )
{
	a.X /= b.X;
	a.Y /= b.Y;

	return a;
}

TVec2 operator *= ( TVec2 & a , const float b )
{
	a.X *= b;
	a.Y *= b;
	
	return a;
}

TVec2 operator /= ( TVec2 & a , const float b )
{
	a.X /= b;
	a.Y /= b;
	
	return a;
}

TVec3 operator + ( const TVec3 & a , const TVec3 & b )
{
	return Vec3 ( a.X + b.X , a.Y + b.Y , a.Z + b.Z );
}

TVec3 operator - ( const TVec3 & a , const TVec3 & b )
{
	return Vec3 ( a.X - b.X , a.Y - b.Y , a.Z - b.Z );
}

TVec3 operator * ( const TVec3 & a , const TVec3 & b )
{
	return Vec3 ( a.X * b.X , a.Y * b.Y , a.Z * b.Z );
}

TVec3 operator / ( const TVec3 & a , const TVec3 & b )
{
	return Vec3 ( a.X / b.X , a.Y / b.Y , a.Z / b.Z );
}

TVec3 operator * ( const TVec3 & a , const float b )
{
	return Vec3 ( a.X * b , a.Y * b , a.Z * b );
}

TVec3 operator / ( const TVec3 & a , const float b )
{
	return Vec3 ( a.X / b , a.Y / b , a.Z / b );
}


TVec3 operator += ( TVec3 & a , const TVec3 & b )
{
	a.X += b.X;
	a.Y += b.Y;
	a.Z += b.Z;

	return a;
}

TVec3 operator -= ( TVec3 & a , const TVec3 & b )
{
	a.X -= b.X;
	a.Y -= b.Y;
	a.Z -= b.Z;

	return a;
}

TVec3 operator *= ( TVec3 & a , const TVec3 & b )
{
	a.X *= b.X;
	a.Y *= b.Y;
	a.Z *= b.Z;

	return a;
}

TVec3 operator /= ( TVec3 & a , const TVec3 & b )
{
	a.X /= b.X;
	a.Y /= b.Y;
	a.Z /= b.Z;

	return a;
}

TVec3 operator *= ( TVec3 & a , const float b )
{
	a.X *= b;
	a.Y *= b;
	a.Z *= b;
	
	return a;
}

TVec3 operator /= ( TVec3 & a , const float b )
{
	a.X /= b;
	a.Y /= b;
	a.Z /= b;
	
	return a;
}

TVec2 Normalize ( const TVec2& a )
{
	if (Magnitude(a)==0) {
		return Vec2(0, 0);
	}
	return a/Magnitude(a);
}

TVec3 Normalize ( const TVec3& a )
{
	if (Magnitude(a)==0) {
		return Vec3(0, 0, 0);
	}
	return a/Magnitude(a);
}

float Magnitude ( const TVec2& a )
{
	return sqrt(a.X*a.X+a.Y*a.Y);
}

float Magnitude ( const TVec3& a )
{
	return sqrt(a.X*a.X+a.Y*a.Y+a.Z*a.Z);
}

float Dot ( const TVec2& a , const TVec2& b )
{
    return a.X*b.X + a.Y*b.Y;
}

float Dot ( const TVec3& a , const TVec3& b )
{
    return a.X*b.X + a.Y*b.Y + a.Z*b.Z;
}