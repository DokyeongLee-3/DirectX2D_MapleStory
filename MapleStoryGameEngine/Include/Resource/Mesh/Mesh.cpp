
#include "Mesh.h"
#include "../../Device.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	size_t Size = m_vecContainer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecContainer[i]);
	}
}

bool CMesh::CreateMesh(void* VtxData, int Size, int Count, D3D11_USAGE Usage, D3D11_PRIMITIVE_TOPOLOGY Primitive, void* IdxData, int IdxSize, int IdxCount, D3D11_USAGE IdxUsage, DXGI_FORMAT Fmt)
{
	MeshContainer* Container = new MeshContainer;

	Container->VB.Size = Size;
	Container->VB.Count = Count;
	Container->Primitive = Primitive;

	if (!CreateBuffer(Buffer_Type::Vertex, VtxData, Size,
		Count, Usage, &Container->VB.Buffer))
		return false;

	if (IdxData != nullptr)
	{
		Container->vecIB.resize(1);

		Container->vecIB[0].Size = IdxSize;
		Container->vecIB[0].Count = IdxCount;
		Container->vecIB[0].Fmt = Fmt;

		if (!CreateBuffer(Buffer_Type::Index, IdxData, IdxSize,
			IdxCount, IdxUsage, &Container->vecIB[0].Buffer))
			return false;
	}

	m_vecContainer.push_back(Container);

	return true;
}

bool CMesh::Init()
{
	return true;
}

bool CMesh::CreateBuffer(Buffer_Type Type, void* Data,
	int Size, int Count, D3D11_USAGE Usage,
	ID3D11Buffer** Buffer)
{
	D3D11_BUFFER_DESC Desc = {};

	Desc.ByteWidth = Size * Count;
	Desc.Usage = Usage;

	if (Type == Buffer_Type::Vertex)
		Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	else
		Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// DYNAMIC은 CPU쓰기만 허용
	if (Usage == D3D11_USAGE_DYNAMIC)
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// STAGING은 CPU쓰기/읽기 모두 허용
	else if (Usage == D3D11_USAGE_STAGING)
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA BufferData = {};

	BufferData.pSysMem = Data;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc,
		&BufferData, Buffer)))
		return false;

	if (Type == Buffer_Type::Vertex)
	{
		char* VertexData = (char*)Data;

		for (int i = 0; i < Count; ++i)
		{

			Vector3* Pos = (Vector3*)VertexData;
			VertexData += Size;

			if (m_Min.x > Pos->x)
				m_Min.x = Pos->x;

			if (m_Min.y > Pos->y)
				m_Min.y = Pos->y;

			if (m_Min.z > Pos->z)
				m_Min.z = Pos->z;

			if (m_Max.x < Pos->x)
				m_Max.x = Pos->x;

			if (m_Max.y < Pos->y)
				m_Max.y = Pos->y;

			if (m_Max.z < Pos->z)
				m_Max.z = Pos->z;
		}
	}


	return true;
}

void CMesh::Render()
{
	size_t Size = m_vecContainer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		unsigned int Stride = m_vecContainer[i]->VB.Size;
		unsigned int Offset = 0;

		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecContainer[i]->Primitive);
		// 1번째 인자 StartSlot : 정점 버퍼를 pipeline의 한 입력슬롯에 묶을 수(bind) 있다 -> 몇번 슬롯에 bind할것인지
		// 2번째 인자 NumBuffers : bind할 buffer의 수
		// 3번째 인자 : bind할 VertexBuffer
		// 4번째 인자 Stride : Each stride is the size (in bytes) of the elements that are to be used from that vertex buffer.
		// 마지막인자 Offset : Each offset is the number of bytes between the first element of a vertex buffer and the first element that will be used.
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,
			&m_vecContainer[i]->VB.Buffer, &Stride, &Offset);

		size_t IdxCount = m_vecContainer[i]->vecIB.size();


		// Draw나 DrawIndex 함수는 모든 파이프라인 구성및 설정이 끝났을때 호출하고
		// 파이프라인을 동작시켜서 렌더링 결과를 렌더타겟뷰와 DepthStencilBuffer에
		// 결과를 출력하도록 한다
		if (IdxCount > 0)
		{
			for (size_t j = 0; j < IdxCount; ++j)
			{
				CDevice::GetInst()->GetContext()->IASetIndexBuffer(
					m_vecContainer[i]->vecIB[j].Buffer,
					m_vecContainer[i]->vecIB[j].Fmt, 0);
				// 2번째 인자 StartIndexLocation : The location of the first index read by the GPU from the index buffer.
				// 3번째 인자 BaseVertexLocation : A value added to each index before reading a vertex from the vertex buffer.
				CDevice::GetInst()->GetContext()->DrawIndexed(
					m_vecContainer[i]->vecIB[j].Count, 0, 0);
			}
		}

		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->Draw(
				m_vecContainer[i]->VB.Count, 0);
		}
	}

}

void CMesh::RenderInstancing(int Count)
{
	size_t	Size = m_vecContainer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		unsigned int	Stride = (unsigned int)m_vecContainer[i]->VB.Size;
		unsigned int	Offset = 0;

		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecContainer[i]->Primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,
			&m_vecContainer[i]->VB.Buffer, &Stride, &Offset);

		size_t	IdxCount = m_vecContainer[i]->vecIB.size();

		if (IdxCount > 0)
		{
			for (size_t j = 0; j < IdxCount; ++j)
			{
				CDevice::GetInst()->GetContext()->IASetIndexBuffer(
					m_vecContainer[i]->vecIB[j].Buffer,
					m_vecContainer[i]->vecIB[j].Fmt, 0);
				CDevice::GetInst()->GetContext()->DrawIndexedInstanced(
					m_vecContainer[i]->vecIB[j].Count, Count, 0, 0, 0);
			}
		}

		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->DrawInstanced(
				m_vecContainer[i]->VB.Count, Count, 0, 0);
		}
	}
}

void CMesh::RenderInstancing(ID3D11Buffer* InstancingBuffer, unsigned int InstanceSize, int Count)
{
	size_t	Size = m_vecContainer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		unsigned int	Stride[2] = { (unsigned int)m_vecContainer[i]->VB.Size, InstanceSize };
		unsigned int	Offset[2] = {};

		ID3D11Buffer* Buffer[2] = { m_vecContainer[i]->VB.Buffer , InstancingBuffer };

		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecContainer[i]->Primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 2,
			Buffer, Stride, Offset);

		size_t	IdxCount = m_vecContainer[i]->vecIB.size();

		if (IdxCount > 0)
		{
			for (size_t j = 0; j < IdxCount; ++j)
			{
				CDevice::GetInst()->GetContext()->IASetIndexBuffer(
					m_vecContainer[i]->vecIB[j].Buffer,
					m_vecContainer[i]->vecIB[j].Fmt, 0);
				CDevice::GetInst()->GetContext()->DrawIndexedInstanced(
					m_vecContainer[i]->vecIB[j].Count, Count, 0, 0, 0);
			}
		}

		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->DrawInstanced(
				m_vecContainer[i]->VB.Count, Count, 0, 0);
		}
	}
}
