#pragma once

#define	DIRECTINPUT_VERSION	0x0800

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <crtdbg.h>
#include <functional>
#include <string>
#include <math.h>
#include <algorithm>
#include <d3d11.h>
// 쉐이더를 위해 HLSL이라는 스크립트 언어를 컴파일해야하는데
// 그 기능을 포함한 헤더 
#include <d3dcompiler.h>
#include <dinput.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "SharedPtr.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


#define ROOT_PATH		"Root"
#define SHADER_PATH		"Shader"
#define	TEXTURE_PATH	"Texture"
#define	FONT_PATH		"Font"
#define	ANIMATION_PATH	"Animation"
#define	SCENE_PATH		"Scene"

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr;}
#define SAFE_RELEASE(p) if(p) { p->Release(); p = nullptr; }

#define DECLARE_SINGLE(Type)	\
private:\
	static Type* m_Inst;\
public:\
	static Type* GetInst()\
	{\
		if (!m_Inst)\
			m_Inst = new Type; \
		return m_Inst;\
	}\
	static void DestroyInst()\
	{\
		SAFE_DELETE(m_Inst);\
	}\
private:\
	Type(); \
	~Type();

#define DEFINITION_SINGLE(Type) Type* Type::m_Inst = nullptr;

struct Resolution
{
	unsigned int Width;
	unsigned int Height;
};

struct VertexColor
{
	Vector3 Pos;
	Vector4 Color;

	VertexColor()
	{

	}

	VertexColor(const Vector3& _Pos, const Vector4& _Color) :
		Pos(_Pos),
		Color(_Color)
	{
	}
};

struct VertexBuffer
{
	ID3D11Buffer* Buffer;
	// Vertex하나의 데이터 사이즈
	int Size;
	int Count;

	VertexBuffer() :
		Buffer(nullptr),
		Size(0),
		Count(0)
	{
	}

	~VertexBuffer()
	{
		SAFE_RELEASE(Buffer);
	}
};

struct IndexBuffer
{
	ID3D11Buffer* Buffer;
	int Size;
	int Count;
	DXGI_FORMAT Fmt;

	IndexBuffer() :
		Buffer(nullptr),
		Size(0),
		Count(0),
		Fmt(DXGI_FORMAT_UNKNOWN)
	{
	}

	~IndexBuffer()
	{
		SAFE_RELEASE(Buffer);
	}
};

struct MeshContainer
{
	VertexBuffer VB;
	std::vector<IndexBuffer> vecIB;
	D3D11_PRIMITIVE_TOPOLOGY Primitive;
};

//ShaderInfo.fx에 cbuffer Transform와 대응되는 구조체
struct TransformCBuffer
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
	// World랑 View 곱한것
	Matrix	matWV;
	// World, View, Proj 곱한것
	Matrix	matWVP;
	// View랑 Proj 곱한것
	Matrix	matVP;
	Vector3	Pivot;
	// 16바이트 정렬을 위해 끼워넣은것
	float	Empty1;
	Vector3	MeshSize;
	// 16바이트 정렬을 위해 끼워넣은것
	float	Empty;
};

struct VertexUV
{
	Vector3	Pos;
	Vector2 UV;

	VertexUV()
	{
	}

	VertexUV(const Vector3& _Pos, const Vector2& _UV) :
		Pos(_Pos),
		UV(_UV)
	{
	}
};

struct MaterialCBuffer
{
	Vector4	BaseColor;
	float Opacity;
	Vector3 Empty;
};

struct AnimationFrameData
{
	Vector2 Start;
	Vector2 Size;
};

struct Animation2DCBuffer
{
	Vector2 Animation2DStartUV;
	Vector2 Animation2DEndUV;
	int Animation2DType;
	Vector3 Animation2DEmpty;
};

struct Standard2DCBuffer
{
	int		AnimationEnable;
	Vector3	Empty;
};

struct FindComponentName
{
	std::string	Name;
	std::string	ParentName;
};
