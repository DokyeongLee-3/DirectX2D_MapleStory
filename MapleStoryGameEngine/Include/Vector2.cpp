#include "Vector2.h"

Vector2::Vector2()	:
	x(0.f),
	y(0.f)
{
}

Vector2::Vector2(float _x, float _y)	:
	x(_x),
	y(_y)
{
}

Vector2::Vector2(const Vector2& v)	:
	x(v.x),
	y(v.y)
{
}

Vector2::Vector2(const XMVECTOR& v)
{
	// XMVECTOR->XMFLOAT2로 변환해줌(3,4도 있음)
	// Vector2->XMFLOAT2는 멤버가 완전히 일치하기 때문에
	// 바로 아래처럼 명시적 형변환 가능
	XMStoreFloat2((XMFLOAT2*)this, v);

	// XMLoadFloat2는 XMFLOAT2->XMVECTOR로 변환해줌(3,4도 있음)
}

Vector2& Vector2::operator=(const Vector2& v)
{
	x = v.x;
	y = v.y;

	return *this;
}

Vector2& Vector2::operator=(const XMVECTOR& v)
{
	XMStoreFloat2((XMFLOAT2*)this, v);

	return *this;
}

Vector2& Vector2::operator=(float f)
{
	x = f;
	y = f;
	
	return *this;
}

bool Vector2::operator==(const Vector2& v) const
{
	return x == v.x && y == v.y;
}

bool Vector2::operator==(const XMVECTOR& v) const
{
	// XMVECTOR1 == XMVECTOR2 이런식의 직접 비교는 불가능
	// 따라서 아래처럼 각각 비교해주고 AND연산을 해줘야한다
	Vector2 v1;
	XMStoreFloat2((XMFLOAT2*)&v1, v);

	return x == v1.x && y == v1.y;
}

bool Vector2::operator!=(const Vector2& v) const
{
	return x != v.x || y != v.y;
}

bool Vector2::operator!=(const XMVECTOR& v) const
{
	Vector2 v1;
	XMStoreFloat2((XMFLOAT2*)&v1, v);

	return x != v1.x || y != v1.y;
}

float& Vector2::operator[](int Index)
{
	if (Index < 0 || Index > 1)
	{
		assert(false);
	}

	if (Index == 0)
		return x;

	return y;
}

// +
Vector2 Vector2::operator+(const Vector2& v) const
{
	XMVECTOR v1 = Convert();
	XMVECTOR v2 = v.Convert();

	return Vector2(v1 + v2);
}

Vector2 Vector2::operator+(const XMVECTOR& v) const
{
	return Vector2(Convert() + v);
}

Vector2 Vector2::operator+(const float f) const
{
	return Vector2(x + f, y + f);
}

// +=
void Vector2::operator+=(const Vector2& v)
{
	XMVECTOR v1 = Convert();
	XMVECTOR v2 = v.Convert();

	// 더한값을 Convert한 후 자기 자신의 값으로 할당
	Convert(v1 + v2);
}

void Vector2::operator+=(const XMVECTOR& v)
{
	XMVECTOR v1 = Convert();

	// 더한값을 Convert한 후 자기 자신의 값으로 할당
	Convert(v1 + v);
}

void Vector2::operator+=(const float f)
{
	x += f;
	y += f;
}

// -
Vector2 Vector2::operator-(const Vector2& v) const
{
	XMVECTOR v1 = Convert();
	XMVECTOR v2 = v.Convert();

	return Vector2(v1 - v2);
}

Vector2 Vector2::operator-(const XMVECTOR& v) const
{
	return Vector2(Convert() - v);
}

Vector2 Vector2::operator-(const float f) const
{
	return Vector2(x - f, y - f);
}

// -=
void Vector2::operator-=(const Vector2& v)
{
	XMVECTOR v1 = Convert();
	XMVECTOR v2 = v.Convert();

	// 뺀값을 Convert한 후 자기 자신의 값으로 할당
	Convert(v1 - v2);
}

void Vector2::operator-=(const XMVECTOR& v)
{
	XMVECTOR v1 = Convert();

	// 뺀값을 Convert한 후 자기 자신의 값으로 할당
	Convert(v1 - v);
}

void Vector2::operator-=(const float f)
{
	x -= f;
	y -= f;
}

Vector2 Vector2::operator*(const Vector2& v) const
{
	XMVECTOR v1 = Convert();
	XMVECTOR v2 = v.Convert();

	return Vector2(v1 * v2);
}

Vector2 Vector2::operator*(const XMVECTOR& v) const
{
	return Vector2(Convert() * v);
}

Vector2 Vector2::operator*(const float f) const
{
	return Vector2(x * f, y * f);
}

void Vector2::operator*=(const Vector2& v)
{
	XMVECTOR v1 = Convert();
	XMVECTOR v2 = v.Convert();

	// 곱한값을 Convert한 후 자기 자신의 값으로 할당
	Convert(v1 * v2);
}

void Vector2::operator*=(const XMVECTOR& v)
{
	XMVECTOR v1 = Convert();

	// 곱한값을 Convert한 후 자기 자신의 값으로 할당
	Convert(v1 * v);
}

void Vector2::operator*=(const float f)
{
	x *= f;
	y *= f;
}

// 여기부터 /
Vector2 Vector2::operator/(const Vector2& v) const
{
	XMVECTOR v1 = Convert();
	XMVECTOR v2 = v.Convert();

	return Vector2(v1 / v2);
}

Vector2 Vector2::operator/(const XMVECTOR& v) const
{
	return Vector2(Convert() / v);
}

Vector2 Vector2::operator/(const float f) const
{
	return Vector2(x / f, y / f);
}

void Vector2::operator/=(const Vector2& v)
{
	XMVECTOR v1 = Convert();
	XMVECTOR v2 = v.Convert();

	// 곱한값을 Convert한 후 자기 자신의 값으로 할당
	Convert(v1 / v2);
}

void Vector2::operator/=(const XMVECTOR& v)
{
	XMVECTOR v1 = Convert();

	// 곱한값을 Convert한 후 자기 자신의 값으로 할당
	Convert(v1 / v);
}

void Vector2::operator/=(const float f)
{
	x /= f;
	y /= f;
}

const Vector2& Vector2::operator++()
{
	x += 1.f;
	y += 1.f;

	return *this;
}

const Vector2& Vector2::operator++(int)
{
	x += 1.f;
	y += 1.f;

	return *this;
}

const Vector2& Vector2::operator--()
{
	x -= 1.f;
	y -= 1.f;

	return *this;
}

const Vector2& Vector2::operator--(int)
{
	x -= 1.f;
	y -= 1.f;

	return *this;
}

float Vector2::Length() const
{
	// XMVector2Length함수는 인자로 들어온 2D벡터의 길이를
	// x, y 성분 둘다에 갖고 있는 XMVector를 리턴한다
	// 따라서 X값만 얻으면 XMVector2Length의 인자로 준
	// 벡터의 길이를 얻을 수 있다
	return XMVectorGetX(XMVector2Length(Convert()));
}

float Vector2::Distance(const Vector2& v) const
{
	Vector2 Dist = v - *this;

	return Dist.Length();
}

void Vector2::Normalize()
{
	*this = XMVector2Normalize(Convert());
}

float Vector2::Dot(const Vector2& v) const
{
	return XMVectorGetX(XMVector2Dot(Convert(), v.Convert()));

}

float Vector2::Angle(const Vector2& v) const
{
	return 0.0f;
}


XMVECTOR Vector2::Convert() const
{
	// XMFLOAT2의 값을 XMVECTOR로 변환해서 리턴
	// 자기 자신을 바꾸는게 아니라 바꿔준걸 리턴하는거니까
	// const를 붙여줌
	return XMLoadFloat2((XMFLOAT2*)this);
}

void Vector2::Convert(const XMVECTOR& v)
{
	// XMVECTOR를 XMFLOAT2에 저장한다
	// v를 변환해서 자기자신에 저장하는거니까
	// const를 붙이면 절대 안된다
	XMStoreFloat2((XMFLOAT2*)this, v);
}
