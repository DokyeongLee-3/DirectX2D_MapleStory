
#include "ColorMesh.h"

CColorMesh::CColorMesh()
{
	SetTypeID<CColorMesh>();
}

CColorMesh::~CColorMesh()
{
}

bool CColorMesh::Init()
{
	MeshContainer* Container = new MeshContainer;

	Container->VB.Size = sizeof(VertexColor);
	Container->VB.Count = 4;
	Container->Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VertexColor	Vtx[4] =
	{
		VertexColor(Vector3(0.f, 1.f, 0.f), Vector4::Red),
		VertexColor(Vector3(1.f, 1.f, 0.f), Vector4::Red),
		VertexColor(Vector3(0.f, 0.f, 0.f), Vector4::Red),
		VertexColor(Vector3(1.f, 0.f, 0.f), Vector4::Red)
	};

	if (!CreateBuffer(Buffer_Type::Vertex, Vtx, sizeof(VertexColor),
		4, D3D11_USAGE_IMMUTABLE, &Container->VB.Buffer))
		return false;

	Container->vecIB.resize(1);

	Container->vecIB[0].Size = 2;
	Container->vecIB[0].Count = 6;
	Container->vecIB[0].Fmt = DXGI_FORMAT_R16_UINT;

	unsigned short Idx[6] = { 0, 1, 3, 0, 3, 2 };

	if (!CreateBuffer(Buffer_Type::Index, Idx, sizeof(unsigned short),
		6, D3D11_USAGE_IMMUTABLE, &Container->vecIB[0].Buffer))
		return false;

	m_vecContainer.push_back(Container);

	return true;
}
