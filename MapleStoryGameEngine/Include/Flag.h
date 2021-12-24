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

enum class ConstantBuffer_Shader_Type
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
	Max
};

enum class Collision_Channel
{
	Object,
	Player,
	Monster,
	PlayerAttack,
	MonsterAttack,
	Custom1,
	Custom2,
	Custom3,
	Custom4,
	Custom5,
	Custom6,
	Custom7,
	Custom8,
	Custom9,
	Custom10,
	Custom11,
	Custom12,
	Custom13,
	Custom14,
	Custom15,
	Custom16,
	Max
};

enum class Collision_State
{
	Ignore,
	Collision
};

enum class Collider_Type
{
	Box2D
};