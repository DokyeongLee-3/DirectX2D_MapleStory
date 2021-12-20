#pragma once

#include "EngineMath.h"

struct Vector2
{
	float x, y;

	Vector2();
	Vector2(float _x, float _y);
	Vector2(const Vector2& v);
	// SIMD레지스터를 이용해서 빠르게 병렬적으로 연산하는
	// XMVERTOR를 이용하면서 동시에 사용하기 쉬운
	// XMFLOAT2(or 3, 4)를 써서 연산하려면
	// 연산할때는 XMVERTOR로 변환해서 연산하되 
	// 연산이 끝나고 결과는 XMFLOAT으로 변환해서 사용하자
	Vector2(const XMVECTOR& v);

	// operator =
	Vector2& operator = (const Vector2 & v);
	Vector2& operator = (const XMVECTOR& v);
	Vector2& operator = (float f);

	// operator ==, !=
	bool operator ==(const Vector2& v) const;
	bool operator ==(const XMVECTOR& v) const;
	bool operator !=(const Vector2& v) const;
	bool operator !=(const XMVECTOR& v) const;

	float& operator [] (int Index);

	// operator +
	Vector2 operator + (const Vector2& v)	const;
	Vector2 operator + (const XMVECTOR& v)	const;
	Vector2 operator + (const float f)	const;

	// operator +=
	void operator += (const Vector2& v);
	void operator += (const XMVECTOR& v);
	void operator += (const float f);

	// operator -
	Vector2 operator - (const Vector2& v)	const;
	Vector2 operator - (const XMVECTOR& v)	const;
	Vector2 operator - (const float f)	const;

	// operator -=
	void operator -= (const Vector2& v);
	void operator -= (const XMVECTOR& v);
	void operator -= (const float f);

	// operator *
	Vector2 operator * (const Vector2& v)	const;
	Vector2 operator * (const XMVECTOR& v)	const;
	Vector2 operator * (const float f)	const;

	// operator *=
	void operator *= (const Vector2& v);
	void operator *= (const XMVECTOR& v);
	void operator *= (const float f);

	// operator /
	Vector2 operator / (const Vector2& v)	const;
	Vector2 operator / (const XMVECTOR& v)	const;
	Vector2 operator / (const float f)	const;

	// operator /=
	void operator /= (const Vector2& v);
	void operator /= (const XMVECTOR& v);
	void operator /= (const float f);

	// operator ++
	const Vector2& operator ++ ();
	const Vector2& operator ++ (int);

	// operator --
	const Vector2& operator -- ();
	const Vector2& operator -- (int);

	float Length()	const;
	float Distance(const Vector2& v)	const;
	void Normalize();
	// 내적
	float Dot(const Vector2& v) const;
	float Angle(const Vector2& v)	const;

	XMVECTOR Convert() const;
	void Convert(const XMVECTOR& v);
};

