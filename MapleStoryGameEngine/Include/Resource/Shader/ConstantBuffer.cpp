
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

	// �ý��۸޸𸮿� �ִ� ���۸� CPU�� �����ؾ� �ϴϱ� Dynamic
	// Dynamic�� ���� CConstantBuffer::UpdateBuffer���� �� �� �ֵ���, Map, UnMap����
	// ������ۿ� �����͸� ���ش�
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
	// �츮�� CConstantBuffer::Init���� CreateBuffer�� ���� ������
	// m_Buffer�� ������ �������̽��̴�. �� �������̽��� ���� ����
	// ������ �а� �� �� �ִ� �޸� �ּҸ� �� �� �ִµ�, �̶���
	// �ش� �޸𸮴� �ٸ� �ڿ����� ������ �� ���� atomic�ϰ� �ؾ��ϹǷ�
	// lock,unlock�� �̿��ؾ��ϴµ� �� lock/unlock�� DX11������
	// map,unmap�̴�.
	// Map/UnMap�� �ι�° ���ڴ� ���긮�ҽ� �ε����ε�,
	// ������ ��� ������ü�� ������ ���긮�ҽ��̹Ƿ� 0�� �ִ´�
	D3D11_MAPPED_SUBRESOURCE Map = {};

	CDevice::GetInst()->GetContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD,
		0, &Map);

	memcpy(Map.pData, Data, m_Size);

	CDevice::GetInst()->GetContext()->Unmap(m_Buffer, 0);

	// ���� �츮�� C++�� ����ü�� ������ ������ۿ��ٰ� World, View, Proj ���� �����ϰ� �ִµ�
	// ���̴����� ShaderInfo.fx�� ������ ������ۿ� �����ؼ� World, View, Proj��ĵ���
	// �������� �� ���̴�. ���� C++�� ������ ����ü�� �����͸� ������ Map/UnMap�� ����
	// �� ���̴� �ܰ迡�� ShaderInfo.fx�� ������ۿ� ��������� �Ѵ�
	if (m_ConstantBufferShaderType & (int)ConstantBuffer_Shader_Type::Vertex)
		CDevice::GetInst()->GetContext()->VSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & (int)ConstantBuffer_Shader_Type::Pixel)
		CDevice::GetInst()->GetContext()->PSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & (int)ConstantBuffer_Shader_Type::Domain)
		CDevice::GetInst()->GetContext()->DSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & (int)ConstantBuffer_Shader_Type::Hull)
		CDevice::GetInst()->GetContext()->HSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & (int)ConstantBuffer_Shader_Type::Geometry)
		CDevice::GetInst()->GetContext()->GSSetConstantBuffers(m_Register, 1, &m_Buffer);

	if (m_ConstantBufferShaderType & (int)ConstantBuffer_Shader_Type::Compute)
		CDevice::GetInst()->GetContext()->CSSetConstantBuffers(m_Register, 1, &m_Buffer);
}