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
#include <dwrite.h>
#include <dwrite_3.h>
#include <d2d1.h>
#include <stack>
#include <process.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "SharedPtr.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Resource/Texture/DirectXTex.h"
#include "fmod.hpp"

#ifdef _DEBUG

#pragma comment (lib, "DirectXTex_Debug.lib")

#else

#pragma comment(lib, "DirectXTex.lib")

#endif	// _DEBUG

#pragma comment(lib, "fmod64_vc.lib")


#define ROOT_PATH				"Root"
#define SHADER_PATH				"Shader"
#define	TEXTURE_PATH			"Texture"
#define	FONT_PATH				"Font"
#define	ANIMATION_PATH			"Animation"
#define	SCENE_PATH				"Scene"
#define	SOUND_PATH				"Sound"
#define	PLAYER_TEXTURE_PATH				"PlayerTexture"

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
	float	Opacity;
	int		PaperBurnEnable;
	Vector2	Empty;
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
	float	FadeAmount;
	Vector2	Empty;
};

struct FindComponentName
{
	std::string	Name;
	std::string	ParentName;
};

struct CollisionProfile
{
	std::string			Name;
	Collision_Channel	Channel;
	bool				CollisionEnable;

	std::vector<Collision_Interaction>	vecInteraction;
};

struct CollisionResult
{
	class CColliderComponent* Src;
	class CColliderComponent* Dest;
	Vector3	HitPoint;

	CollisionResult() :
		Src(nullptr),
		Dest(nullptr)
	{
	}
};

struct Box2DInfo
{
	// ColliderBox2D의 WorldPos로 부터 상대적인 위치로 센터를 잡는다
	// -> ColliderComponent들이 Offset을 갖고 있어서 WorldPos로부터
	// 센터가 얼마나 떨어져 있는지 Offset으로 설정해둔다
	Vector2	Center;
	Vector2	Axis[2];
	// 가로축, 너비축의 절반 길이
	Vector2	Length;
	Vector2	Min;
	Vector2	Max;
};

struct CircleInfo
{
	Vector2	Center;
	float	Radius;
	Vector2	Min;
	Vector2	Max;
};

struct PixelInfo
{
	// Pixel이라는 동적배열은 ColliderPixel에서 
	// 얕은 복사가 되게하고 얕은 복사가 될때마다
	// RefCount를 증가시켜주고 RefCount가 0이면
	// 그때서야 Delete를 해준다
	unsigned char* Pixel;
	unsigned int	Width;
	unsigned int	Height;
	PixelCollision_Type	Type;
	// 어떤 색이랑만 충돌할지(혹은 이 색이랑만 충돌을 무시하고 나머지 색 모두랑 충돌할지 정해준다
	unsigned char	Color[4];
	// 픽셀 충돌체 자체가 하나의 이미지(아래 SRV)를 갖고 있는 상황이고
	// 그 이미지의 테두리를 감싸는 Box충돌체가 있다
	Box2DInfo		Box;
	Vector2	Min;
	Vector2	Max;
	ID3D11ShaderResourceView* SRV;
	int		RefCount;

	PixelInfo() :
		RefCount(1),
		Pixel(nullptr),
		SRV(nullptr),
		Width(0),
		Height(0),
		Box{},
		Color{},
		Type(PixelCollision_Type::Color_Confirm)
	{
	}
};

struct ColliderCBuffer
{
	Vector4	Color;
	Matrix	matWVP;
};

struct WidgetCBuffer
{
	Vector4	Tint;
	Matrix	matWP;
	int		UseTexture;
	int		WidgetAnimType;
	Vector2	WidgetAnimStartUV;
	Vector2	WidgetAnimEndUV;
	int		WidgetAnimEnable;
	float	Opacity;
};

struct ProgressBarCBuffer
{
	float	Percent;
	int		Dir;
	Vector2	Empty;
};

struct	ParticleCBuffer
{
	unsigned int	SpawnEnable;	// 현재 파티클 생성 여부
	Vector3	StartMin;				// 파티클이 생성될 영역의 좌하단 좌표
	Vector3	StartMax;				// 파티클이 생성될 영역의 우상단 좌표
	unsigned int	SpawnCountMax;	// 생성될 파티클의 최대
	Vector3	ScaleMin;				// 생성될 파티클 크기의 Min
	float	LifeTimeMin;			// 생성될 파티클이 살아있을 최소시간
	Vector3	ScaleMax;				// 새성될 파티클 크기의 Max
	float	LifeTimeMax;			// 생성될 파티클이 살아있을 최대시간
	Vector4	ColorMin;				// 생성될 파티클의 색상 Min
	Vector4	ColorMax;				// 생성될 파티클의 색상 Max
	float	SpeedMin;				// 파티클의 최소 이동속도
	float	SpeedMax;				// 파티클의 최대 이동속도
	int		Move;					// 이동을 하는지 안하는지
	int		Gravity;				// 중력 적용을 받는지 안받는지
	Vector3	MoveDir;				// 이동을 한다면 기준이 될 이동 방향
	int		Is2D;					// 2D용 파티클인지
	Vector3	MoveAngle;				// 이동을 한다면 기준이 될 방향으로부터 x, y, z 에 저장된 각도만큼 틀어진 랜덤한 방향을 구한다.
	float	Empty;
};

struct ParticleInfo
{
	Vector3	WorldPos;
	Vector3	Dir;
	float	Speed;
	float	LifeTime;
	float	LifeTimeMax;
	int		Alive;
	float	FallTime;
	float	FallStartY;
};

struct ParticleInfoShared
{
	unsigned int	SpawnEnable;
	Vector3	ScaleMin;
	Vector3	ScaleMax;
	Vector4	ColorMin;
	Vector4	ColorMax;
	int		GravityEnable;
};

struct GlobalCBuffer
{
	float	DeltaTime;
	float	AccTime;
	Vector2	Resolution;
	Vector2	NoiseResolution;
	Vector2	Empty;
};

struct PaperBurnCBuffer
{
	Vector4	InLineColor;	// 안쪽 라인 색깔
	Vector4	OutLineColor;	// 바깥쪽 라인 색깔
	Vector4	CenterLineColor;// 중간 라인 색깔
	float	Filter;		// 흐른 시간값
	int		Inverse;		// 역실행 여부
	float	InFilter;		// 안쪽 두깨
	float	OutFilter;		// 바깥쪽 두께
	float	CenterFilter;	// 중간 라인 두께
	Vector3	Empty;
};


struct MonsterInfo
{
	Vector3	WorldPos;
	int		Alive;
};

struct MonsterInfoShared
{
	unsigned int	SpawnEnable;
	unsigned int	CurrentSpawnCount;
	unsigned int	SpawnCountMax;
	float			Empty;
};

struct MonsterCBuffer
{
	unsigned int	MonsterSpawnEnable;
	unsigned int	MonsterCurrentSpawnCount;
	unsigned int	MonsterSpawnCountMax;
	float			MonsterEmpty;
};

struct TileMapCBuffer
{
	Vector2 ImageSize;
	Vector2 Size;
	Vector2 Start;
	Vector2 End;
	Matrix  matWVP;
};

struct TileInfo
{
	Matrix matWVP;
	Vector2 TileStart;
	Vector2 TileEnd;
	Vector4 TileColor;
	float Opacity;
	Vector3 Empty;
};

struct MovingTileMapCBuffer
{
	Vector2 ImageSize;
	float Speed;
	float Empty;
};

struct MovingTileInfo
{
	Matrix matWVP;
	Vector2 Start;
	Vector2 End;
	float Opacity;
	Vector3 Empty;
};