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
	// XMVECTOR->XMFLOAT2�� ��ȯ����(3,4�� ����)
	// Vector2->XMFLOAT2�� ����� ������ ��ġ�ϱ� ������
	// �ٷ� �Ʒ�ó�� ����� ����ȯ ����
	XMStoreFloat2((XMFLOAT2*)this, v);

	// XMLoadFloat2�� XMFLOAT2->XMVECTOR�� ��ȯ����(3,4�� ����)
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
	// XMVECTOR1 == XMVECTOR2 �̷����� ���� �񱳴� �Ұ���
	// ���� �Ʒ�ó�� ���� �����ְ� AND������ ������Ѵ�
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

	// ���Ѱ��� Convert�� �� �ڱ� �ڽ��� ������ �Ҵ�
	Convert(v1 + v2);
}

void Vector2::operator+=(const XMVECTOR& v)
{
	XMVECTOR v1 = Convert();

	// ���Ѱ��� Convert�� �� �ڱ� �ڽ��� ������ �Ҵ�
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

	// ������ Convert�� �� �ڱ� �ڽ��� ������ �Ҵ�
	Convert(v1 - v2);
}

void Vector2::operator-=(const XMVECTOR& v)
{
	XMVECTOR v1 = Convert();

	// ������ Convert�� �� �ڱ� �ڽ��� ������ �Ҵ�
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

	// ���Ѱ��� Convert�� �� �ڱ� �ڽ��� ������ �Ҵ�
	Convert(v1 * v2);
}

void Vector2::operator*=(const XMVECTOR& v)
{
	XMVECTOR v1 = Convert();

	// ���Ѱ��� Convert�� �� �ڱ� �ڽ��� ������ �Ҵ�
	Convert(v1 * v);
}

void Vector2::operator*=(const float f)
{
	x *= f;
	y *= f;
}

// ������� /
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

	// ���Ѱ��� Convert�� �� �ڱ� �ڽ��� ������ �Ҵ�
	Convert(v1 / v2);
}

void Vector2::operator/=(const XMVECTOR& v)
{
	XMVECTOR v1 = Convert();

	// ���Ѱ��� Convert�� �� �ڱ� �ڽ��� ������ �Ҵ�
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
	// XMVector2Length�Լ��� ���ڷ� ���� 2D������ ���̸�
	// x, y ���� �Ѵٿ� ���� �ִ� XMVector�� �����Ѵ�
	// ���� X���� ������ XMVector2Length�� ���ڷ� ��
	// ������ ���̸� ���� �� �ִ�
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
	// XMFLOAT2�� ���� XMVECTOR�� ��ȯ�ؼ� ����
	// �ڱ� �ڽ��� �ٲٴ°� �ƴ϶� �ٲ��ذ� �����ϴ°Ŵϱ�
	// const�� �ٿ���
	return XMLoadFloat2((XMFLOAT2*)this);
}

void Vector2::Convert(const XMVECTOR& v)
{
	// XMVECTOR�� XMFLOAT2�� �����Ѵ�
	// v�� ��ȯ�ؼ� �ڱ��ڽſ� �����ϴ°Ŵϱ�
	// const�� ���̸� ���� �ȵȴ�
	XMStoreFloat2((XMFLOAT2*)this, v);
}
