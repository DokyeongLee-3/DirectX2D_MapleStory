#pragma once

enum class Engine_Space
{
	Space2D,
	Space3D
};

enum AXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX
};

enum class Buffer_Type
{
	Vertex,
	Index
};

enum class Shader_Type
{
	Graphic,
	Compute
};

enum class Buffer_Shader_Type
{
	Vertex = 0x1,
	Pixel = 0x2,
	Domain = 0x4,
	Hull = 0x8,
	Geometry = 0x10,
	Compute = 0x20,
	Graphic = Vertex | Pixel | Domain | Hull | Geometry,
	All = Vertex | Pixel | Domain | Hull | Geometry | Compute
};

enum class Component_Type
{
	SceneComponent,
	ObjectComponent
};

enum class Input_Type
{
	Direct,
	Window
};

enum Key_State
{
	KeyState_Down,
	KeyState_Push,
	KeyState_Up,
	KeyState_Max
};

enum class Image_Type
{
	Atlas,
	Frame,
	Array
};

enum class Sampler_Type
{
	Point,
	Linear,
	Anisotropic
};

enum class RenderState_Type
{
	Blend,
	Rasterizer,
	DepthStencil,
	Max
};

enum class SceneComponent_Type
{
	Sprite,
	StaticMesh,
	ColliderBox2D,
	ColliderCircle,
	DragCollider,
	Max
};

enum class Collision_Channel
{
	Object,
	Player,
	Monster,
	PlayerAttack,
	MonsterAttack,
	MapCollider,
	Custom2,
	Custom3,
	Custom4,
	Custom5,
	Custom6,
	Custom7,
	Custom8,
	Custom9,
	Max
};

enum class Collision_Interaction
{
	Ignore,
	Collision
};

enum class Collision_State
{
	// 이제 막 처음으로 충돌이 시작됐을때
	Begin,
	// 충돌이 끝나고 두 충돌체가 서로 떨어질때
	End,
	Max
};

enum class Collider_Type
{
	Box2D,
	Circle,
	Pixel
};

enum class Camera_Type
{
	Camera2D,
	Camera3D,
	CameraUI
};

enum class PixelCollision_Type
{
	// 해당 컬러만 제외하고 나머지 컬러에 대해 모두 충돌
	Color_Ignore,
	// 해당 컬러만 충돌
	Color_Confirm,
	// 해당 알파값만 제외하고 나머지 알파값에 대해 충돌
	Alpha_Ignore,
	// 해당 알파값만 충돌
	Alpha_Confirm
};

enum class Button_State
{
	Normal,
	MouseOn,
	Click,
	Disable,
	Max
};

enum class Button_Sound_State
{
	MouseOn,
	Click,
	Max
};

enum class WidgetComponent_Space
{
	Screen,
	World
};

enum class ProgressBar_Dir
{
	RightToLeft,
	LeftToRight,
	TopToBottom,
	BottomToTop
};

enum class Mouse_State
{
	Normal,
	Click,
	State1,
	State2,
	State3,
	State4,
	State5,
	Max
};

// Thread가 Loading하게 될 Scene을 CLoadingThread에 설정해준다
enum class ThreadLoadingScene
{
	Lobby,
	Onion,
	Main
};


enum class Tile_Shape
{
	Rect,
	Rhombus
};

enum class Tile_Type
{
	Normal,
	Wall
};
