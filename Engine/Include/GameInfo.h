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
// ���̴��� ���� HLSL�̶�� ��ũ��Ʈ �� �������ؾ��ϴµ�
// �� ����� ������ ��� 
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
	// Vertex�ϳ��� ������ ������
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

//ShaderInfo.fx�� cbuffer Transform�� �����Ǵ� ����ü
struct TransformCBuffer
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
	// World�� View ���Ѱ�
	Matrix	matWV;
	// World, View, Proj ���Ѱ�
	Matrix	matWVP;
	// View�� Proj ���Ѱ�
	Matrix	matVP;
	Vector3	Pivot;
	// 16����Ʈ ������ ���� ����������
	float	Empty1;
	Vector3	MeshSize;
	// 16����Ʈ ������ ���� ����������
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
	// ColliderBox2D�� WorldPos�� ���� ������� ��ġ�� ���͸� ��´�
	// -> ColliderComponent���� Offset�� ���� �־ WorldPos�κ���
	// ���Ͱ� �󸶳� ������ �ִ��� Offset���� �����صд�
	Vector2	Center;
	Vector2	Axis[2];
	// ������, �ʺ����� ���� ����
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
	// Pixel�̶�� �����迭�� ColliderPixel���� 
	// ���� ���簡 �ǰ��ϰ� ���� ���簡 �ɶ�����
	// RefCount�� ���������ְ� RefCount�� 0�̸�
	// �׶����� Delete�� ���ش�
	unsigned char* Pixel;
	unsigned int	Width;
	unsigned int	Height;
	PixelCollision_Type	Type;
	// � ���̶��� �浹����(Ȥ�� �� ���̶��� �浹�� �����ϰ� ������ �� ��ζ� �浹���� �����ش�
	unsigned char	Color[4];
	// �ȼ� �浹ü ��ü�� �ϳ��� �̹���(�Ʒ� SRV)�� ���� �ִ� ��Ȳ�̰�
	// �� �̹����� �׵θ��� ���δ� Box�浹ü�� �ִ�
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
	unsigned int	SpawnEnable;	// ���� ��ƼŬ ���� ����
	Vector3	StartMin;				// ��ƼŬ�� ������ ������ ���ϴ� ��ǥ
	Vector3	StartMax;				// ��ƼŬ�� ������ ������ ���� ��ǥ
	unsigned int	SpawnCountMax;	// ������ ��ƼŬ�� �ִ�
	Vector3	ScaleMin;				// ������ ��ƼŬ ũ���� Min
	float	LifeTimeMin;			// ������ ��ƼŬ�� ������� �ּҽð�
	Vector3	ScaleMax;				// ������ ��ƼŬ ũ���� Max
	float	LifeTimeMax;			// ������ ��ƼŬ�� ������� �ִ�ð�
	Vector4	ColorMin;				// ������ ��ƼŬ�� ���� Min
	Vector4	ColorMax;				// ������ ��ƼŬ�� ���� Max
	float	SpeedMin;				// ��ƼŬ�� �ּ� �̵��ӵ�
	float	SpeedMax;				// ��ƼŬ�� �ִ� �̵��ӵ�
	int		Move;					// �̵��� �ϴ��� ���ϴ���
	int		Gravity;				// �߷� ������ �޴��� �ȹ޴���
	Vector3	MoveDir;				// �̵��� �Ѵٸ� ������ �� �̵� ����
	int		Is2D;					// 2D�� ��ƼŬ����
	Vector3	MoveAngle;				// �̵��� �Ѵٸ� ������ �� �������κ��� x, y, z �� ����� ������ŭ Ʋ���� ������ ������ ���Ѵ�.
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
	Vector4	InLineColor;	// ���� ���� ����
	Vector4	OutLineColor;	// �ٱ��� ���� ����
	Vector4	CenterLineColor;// �߰� ���� ����
	float	Filter;		// �帥 �ð���
	int		Inverse;		// ������ ����
	float	InFilter;		// ���� �α�
	float	OutFilter;		// �ٱ��� �β�
	float	CenterFilter;	// �߰� ���� �β�
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