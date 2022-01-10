
#include "MeshManager.h"
#include "SpriteMesh.h"
#include "StaticMesh.h"
#include "ColorMesh.h"

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
}

bool CMeshManager::Init()
{
	CMesh* SpriteMesh = new CSpriteMesh;

	if (!SpriteMesh->Init())
	{
		SpriteMesh->Release();
		return false;
	}

	SpriteMesh->SetName("SpriteMesh");
	m_mapMesh.insert(std::make_pair("SpriteMesh", SpriteMesh));

	CMesh* FrameRectMesh = new CStaticMesh;

	Vector3 FrameRectPos[5] =
	{
		Vector3(0.f, 1.f, 0.f),
		Vector3(1.f, 1.f, 0.f),
		Vector3(1.f, 0.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		Vector3(0.f, 1.f, 0.f)
	};

	FrameRectMesh->SetName("FrameRect");
	FrameRectMesh->CreateMesh(FrameRectPos, sizeof(Vector3), 5,
		D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	m_mapMesh.insert(std::make_pair("FrameRect", FrameRectMesh));



	CMesh* Box2DMesh = new CStaticMesh;

	Vector3	Box2DPos[5] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f),
		Vector3(-0.5f, 0.5f, 0.f)
	};

	Box2DMesh->SetName("Box2D");
	Box2DMesh->CreateMesh(Box2DPos, sizeof(Vector3), 5, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	m_mapMesh.insert(std::make_pair("Box2D", Box2DMesh));


	// ��������Ʈ������ �߾��� ����ִ� ������ �뵵 Mesh
	CMesh* ColorMesh = new CColorMesh;

	if (!ColorMesh->Init())
	{
		ColorMesh->Release();
		return false;
	}

	m_mapMesh.insert(std::make_pair("ColorMesh", ColorMesh));


	// CirCleColliderComponent�� ���� ������ �޽�
	CMesh* CircleMesh = new CStaticMesh;

	Vector3	CirclrPos[31];

	CirclrPos[0] = Vector3(0.5f, 0.f, 0.f);

	for (int i = 1; i < 31; ++i)
	{
		CirclrPos[i].x = cosf(DegreeToRadian(12.f * i)) * 0.5f;
		CirclrPos[i].y = sinf(DegreeToRadian(12.f * i)) * 0.5f;
	}

	CircleMesh->SetName("Circle");
	CircleMesh->CreateMesh(CirclrPos, sizeof(Vector3), 31, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	m_mapMesh.insert(std::make_pair("Circle", CircleMesh));

	CMesh* WidgetMesh = new CStaticMesh;

	VertexUV	WidgetVtx[4] =
	{
		VertexUV(Vector3(0.f, 1.f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(1.f, 1.f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(0.f, 0.f, 0.f), Vector2(0.f, 1.f)),
		VertexUV(Vector3(1.f, 0.f, 0.f), Vector2(1.f, 1.f))
	};

	int	WidgetIdx[6] = { 0, 1, 3, 0, 3, 2 };

	WidgetMesh->SetName("WidgetMesh");
	WidgetMesh->CreateMesh(WidgetVtx, sizeof(VertexUV), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		WidgetIdx, 4, 6, D3D11_USAGE_IMMUTABLE,
		DXGI_FORMAT_R32_UINT);

	m_mapMesh.insert(std::make_pair("WidgetMesh", WidgetMesh));


	return true;
}

CMesh* CMeshManager::FindMesh(const std::string& Name)
{
	auto	iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
		return nullptr;

	return iter->second;
}

void CMeshManager::ReleaseMesh(const std::string& Name)
{
	auto	iter = m_mapMesh.find(Name);

	if (iter != m_mapMesh.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapMesh.erase(iter);
	}
}