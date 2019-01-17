#pragma once

// from http://www.codersnotes.com/notes/maths-lib-2016/
// with some modifications

#include <cstdint>
#include <xmmintrin.h>
#include <math.h>

// Shuffle helpers.
// Examples: SHUFFLE3(v, 0,1,2) leaves the vector unchanged.
//           SHUFFLE3(v, 0,0,0) splats the X coord out.
#define SHUFFLE3(V, X,Y,Z) SHUFFLE4(V, X, Y, Z, Z)
#define SHUFFLE4(V, X,Y,Z,W) SVec4(_mm_shuffle_ps((V).m, (V).m, _MM_SHUFFLE(W,Z,Y,X)))

struct SVec4
{
	// Constructors.
	__forceinline SVec4() {}
	__forceinline explicit SVec4(const float *const p) : m(_mm_loadu_ps(p)) {}
	__forceinline explicit SVec4(const float x, const float y, const float z) : SVec4(x, y, z, z) {}
	__forceinline explicit SVec4(const float x, const float y, const float z, const float w) : m(_mm_set_ps(w, z, y, x)) {}
	__forceinline explicit SVec4(const __m128 v) : m(v) {}

	__forceinline float X() const { return _mm_cvtss_f32(m); }
	__forceinline float Y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
	__forceinline float Z() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
	__forceinline float W() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3))); }

	__forceinline SVec4 Yzx() const { return SHUFFLE3(*this, 1, 2, 0); }
	__forceinline SVec4 Zxy() const { return SHUFFLE3(*this, 2, 0, 1); }

	__forceinline void Store(float *const dst) const { _mm_store_ps(dst, m); }

	__forceinline static SVec4 Zero() { return SVec4{ 0.f, 0.f, 0.f, 0.f }; }

	__m128 m;
};

typedef SVec4 SBool4;

struct vconstu
{
	union { uint32_t u[4]; __m128 v; };
	__forceinline operator __m128() const { return v; }
};
extern const __declspec(selectany) vconstu vsignbits = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
extern const __declspec(selectany) vconstu allone = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

__forceinline SVec4 operator+ (SVec4 a, const SVec4 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
__forceinline SVec4 operator- (SVec4 a, const SVec4 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
__forceinline SVec4 operator* (SVec4 a, const SVec4 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
__forceinline SVec4 operator/ (SVec4 a, const SVec4 b) { a.m = _mm_div_ps(a.m, b.m); return a; }
__forceinline SVec4 operator* (SVec4 a, const float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
__forceinline SVec4 operator/ (SVec4 a, const float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }
__forceinline SVec4 operator* (const float a, SVec4 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }
__forceinline SVec4 operator/ (const float a, SVec4 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }
__forceinline SVec4& operator+= (SVec4 &a, const SVec4 b) { a = a + b; return a; }
__forceinline SVec4& operator-= (SVec4 &a, const SVec4 b) { a = a - b; return a; }
__forceinline SVec4& operator*= (SVec4 &a, const SVec4 b) { a = a * b; return a; }
__forceinline SVec4& operator/= (SVec4 &a, const SVec4 b) { a = a / b; return a; }
__forceinline SVec4& operator*= (SVec4 &a, const float b) { a = a * b; return a; }
__forceinline SVec4& operator/= (SVec4 &a, const float b) { a = a / b; return a; }
__forceinline SBool4 operator==(SVec4 a, const SVec4 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
__forceinline SBool4 operator!=(SVec4 a, const SVec4 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
__forceinline SBool4 operator< (SVec4 a, const SVec4 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
__forceinline SBool4 operator> (SVec4 a, const SVec4 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }
__forceinline SBool4 operator<=(SVec4 a, const SVec4 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
__forceinline SBool4 operator>=(SVec4 a, const SVec4 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
__forceinline SVec4 Min(SVec4 a, const SVec4 b) { a.m = _mm_min_ps(a.m, b.m); return a; }
__forceinline SVec4 Max(SVec4 a, const SVec4 b) { a.m = _mm_max_ps(a.m, b.m); return a; }


// bitwise operations
__forceinline SBool4 operator&(SBool4 a, const SBool4 b) { a.m = _mm_and_ps(a.m, b.m); return a; }
__forceinline SBool4 operator|(SBool4 a, const SBool4 b) { a.m = _mm_or_ps(a.m, b.m); return a; }
__forceinline SBool4 operator^(SBool4 a, const SBool4 b) { a.m = _mm_xor_ps(a.m, b.m); return a; }
__forceinline SBool4 operator~(SBool4 a) { a.m = _mm_andnot_ps(a.m, allone); return a; }

__forceinline SVec4 operator- (SVec4 v) { v.m = _mm_xor_ps(v.m, _mm_set1_ps(-0.0f)); return v; }
__forceinline SVec4 Abs(SVec4 v) { v.m = _mm_andnot_ps(vsignbits, v.m); return v; }
__forceinline SVec4 SignFlip(SVec4 v) { v.m = _mm_and_ps(vsignbits, v.m); return v; }

__forceinline float Hmin(SVec4 v) {
	v = Min(v, SHUFFLE3(v, 1, 0, 2));
	return Min(v, SHUFFLE3(v, 2, 0, 1)).X();
}

__forceinline float Hmax(SVec4 v) {
	v = Max(v, SHUFFLE3(v, 1, 0, 2));
	return Max(v, SHUFFLE3(v, 2, 0, 1)).X();
}

__forceinline SVec4 Cross(const SVec4 a, const SVec4 b) {
	// x  <-  a.y*b.z - a.z*b.y
	// y  <-  a.z*b.x - a.x*b.z
	// z  <-  a.x*b.y - a.y*b.x
	// We can save a shuffle by grouping it in this wacky order:
	return (a.Zxy()*b - a * b.Zxy()).Zxy();
}

// Returns a 4-bit code where bit0..bit3 is X..W
__forceinline unsigned Mask(const SVec4 v) { return _mm_movemask_ps(v.m); }

__forceinline int HminElement(const SVec4 v) {
	const unsigned m0 = Mask(v < v.Yzx());
	const unsigned m1 = Mask(v < v.Zxy());
	const unsigned m2 = (m0 & m1) >> 1;
	return ((m2 >> 1) ^ (m2 & 1)) | (m2 & 2);
}

__forceinline int HmaxElement(const SVec4 v)
{
	const unsigned m0 = Mask(v > v.Yzx());
	const unsigned m1 = Mask(v > v.Zxy());
	const unsigned m2 = (m0 & m1) >> 1;
	return ((m2 >> 1) ^ (m2 & 1)) | (m2 & 2);
}

// Once we have a comparison, we can branch based on its results:
__forceinline bool Any(const SBool4 v) { return Mask(v) != 0; }
__forceinline bool All3(const SBool4 v) { return Mask(v) == 7; }
__forceinline bool All4(const SBool4 v) { return Mask(v) == 15; }

__forceinline SVec4 Clamp(const SVec4 t, const SVec4 a, const SVec4 b) { return Min(Max(t, a), b); }
__forceinline float Sum(const SVec4 v) { return v.X() + v.Y() + v.Z(); }
__forceinline float Dot(const SVec4 a, const SVec4 b) { return Sum(a*b); }
__forceinline float Length(const SVec4 v) { return sqrtf(Dot(v, v)); }
__forceinline float LengthSq(const SVec4 v) { return Dot(v, v); }
__forceinline SVec4 Sqrt(SVec4 v) { v.m = _mm_sqrt_ps(v.m); return v; }
__forceinline SVec4 Normalize(const SVec4 v) { return v * (1.0f / Length(v)); }
__forceinline SVec4 Lerp(const SVec4 a, const SVec4 b, const float t) { return a + (b - a)*t; }
__forceinline SVec4 Select(const SVec4 a, const SVec4 b, const SBool4 mask) { return (b&mask) | (a&(~mask)); }