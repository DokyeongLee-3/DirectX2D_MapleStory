#pragma once

#include "Vector4.h"

// SIMD 연산을 하려면 16바이트 정렬이 되어있어야 한다
union __declspec(align(16)) Matrix
{
	XMMATRIX m;

	struct
	{
		float	_11, _12, _13, _14;
		float	_21, _22, _23, _24;
		float	_31, _32, _33, _34;
		float	_41, _42, _43, _44;
	};

	Vector4 v[4];

	Matrix();
	Matrix(const Matrix& _m);
	Matrix(const XMMATRIX& _m);
	Matrix(Vector4 _v[4]);

	Vector4& operator [] (int Index);
	Matrix operator = (const Matrix& _m);
	Matrix operator = (const XMMATRIX& _m);
	Matrix operator = (Vector4 _v[4]);

	Matrix operator * (const Matrix& _m)	const;
	Matrix operator * (const XMMATRIX& _m)	const;

	const Matrix& operator *= (const Matrix& _m);
	const Matrix& operator *= (const XMMATRIX& _m);

	// void리턴타입 변환 함수들은 내가 원하는 변환을 해주는 변환 행렬을
	// 멤버로 갖고 있는 녀석이다(내가 인자로 뭘 주면 변환해서 
	// 변환 결과를 주는게 아니라)
	void Identity();
	void Transpose();
	void Inverse();
	void Scaling(const struct Vector3& _v);
	void Scaling(float x, float y, float z);
	void Scaling(const struct Vector2& _v);
	void Scaling(float x, float y);
	void Rotation(const struct Vector3& _v);
	void Rotation(float x, float y, float z);
	void RotationX(float x);
	void RotationY(float y);
	void RotationZ(float z);
	void RotationQuaternion(const Vector4& qut);
	// 임의의 축 회전
	void RotationAxis(const struct Vector3& Axis, float Angle);
	void Translation(const struct Vector3& _v);
	void Translation(float x, float y, float z);
	void Translation(const struct Vector2& _v);
	void Translation(float x, float y);

	static Matrix StaticIdentity();
	static Matrix StaticTranspose(const Matrix& _m);
	static Matrix StaticInverse(const Matrix& _m);
	static Matrix StaticScaling(const struct Vector3& _v);
	static Matrix StaticScaling(float x, float y, float z);
	static Matrix StaticScaling(const struct Vector2& _v);
	static Matrix StaticScaling(float x, float y);
	static Matrix StaticRotation(const struct Vector3& _v);
	static Matrix StaticRotation(float x, float y, float z);
	static Matrix StaticRotationX(float x);
	static Matrix StaticRotationY(float y);
	static Matrix StaticRotationZ(float z);
	static Matrix StaticRotationQuaternion(const Vector4& qut);
	// 임의의 축 회전
	static Matrix StaticRotationAxis(const struct Vector3& Axis, float Angle);
	static Matrix StaticTranslation(const struct Vector3& _v);
	static Matrix StaticTranslation(float x, float y, float z);
	static Matrix StaticTranslation(const struct Vector2& _v);
	static Matrix StaticTranslation(float x, float y);
};

