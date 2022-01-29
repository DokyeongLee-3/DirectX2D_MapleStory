
#include "ConstantBuffer.h"
#include "../../Device.h"

CConstantBuffer::CConstantBuffer()	:
	m_Buffer(nullptr),
	m_Size(0),
	m_Register(0),
	m_ConstantBufferShaderType(0)
{
}

CConstantBuffer::~CConstantBuffer()
{
	SAFE_RELEASE(m_Buffer);
}

bool CConstantBuffer::Init(int Size, int Register, int ConstantBufferShaderType)
{
	m_Size = Size;
	m_Register = Register;
	m_ConstantBufferShaderType = ConstantBufferShaderType;

	D3D11_BUFFER_DESC Desc = {};

	// 시스템메모리에 있는 버퍼를 CPU가 갱신해야 하니까 Dynamic
	// Dynamic을 쓰면 CConstantBuffer::UpdateBuffer에서 볼 수 있듯이, Map, UnMap으로
	// 상수버퍼에 데이터를 써준다
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.ByteWidth = Size;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if(FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, nullptr,
		&m_Buffer)))
		return false;


	return true;
}

void CConstantBuffer::UpdateBuffer(void* Data)
{
	// 우리가 CConstantBuffer::Init에서 CreateBuffer로 만든 버퍼인
	// m_Buffer는 버퍼의 인터페이스이다. 이 인터페이스를 통해 실제
	// 정점을 읽고 쓸 수 있는 메모리 주소를 얻어낼 수 있는데, 이때는
	// 해당 메모리는 다른 자원에서 접근할 수 없게 atomic하게 해야하므로
	// lock,unlock을 이용해야하는데 이 lock/unlock의 DX11버전이
	// map,unmap이다.
	// Map/UnMap의 두번째 인자는 서브리소스 인덱스인데,
	// 버퍼의 경우 버퍼자체가 유일한 서브리소스이므로 0을 넣는다
	D3D11_MAPPED_SUBRESOURCE Map = {};

	CDevice::GetInst()->GetContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD,
		0, &Map);

	memcpy(Map.pData, Data, m_Size);

	CDevice::GetInst()->GetContext()->Unmap(m_Buffer, 0);

	// 실제 우리는 C++의 구조체로 정의한 상수버퍼에다가 World, View, Proj 등을 갱신하고 있는데
	// 셰이더에선 ShaderInfo.fx에 정의한 상수버퍼에 접근해서 World, View, Proj행렬들을
	// 얻으려고 할 것이다. 따라서 C++로 정의한 구조체의 데이터를 위에서 Map/UnMap을 통해
	// 각 셰이더 단계에게 ShaderInfo.fx의 상수버퍼에 매핑해줘야 한다
	if (m_ConstantBufferShaderType & (int)Buffer_Shader_Type::Vertex)
		CDevice::GetInst()->GetContext()->VSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & (int)Buffer_Shader_Type::Pixel)
		CDevice::GetInst()->GetContext()->PSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & (int)Buffer_Shader_Type::Domain)
		CDevice::GetInst()->GetContext()->DSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & (int)Buffer_Shader_Type::Hull)
		CDevice::GetInst()->GetContext()->HSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & (int)Buffer_Shader_Type::Geometry)
		CDevice::GetInst()->GetContext()->GSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & (int)Buffer_Shader_Type::Compute)
		CDevice::GetInst()->GetContext()->CSSetConstantBuffers(m_Register, 1, &m_Buffer);
}
