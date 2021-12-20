
#include "ColorMeshShader.h"

CColorMeshShader::CColorMeshShader()
{
	SetTypeID<CColorMeshShader>();
}

CColorMeshShader::~CColorMeshShader()
{
}

bool CColorMeshShader::Init()
{
	// LoadVertexShader로 셰이더 오브젝트(m_VS)와 셰이더 바이트코드(m_VSBlob)를
	// 얻어낸 후에 그걸 AddInputDesc에 쓰기 때문에 LoadVertexShader를
	// 먼저 호출하고 그 다음 Input Layout을 만들어낸다
	if (!LoadVertexShader("ColorMeshVS", TEXT("ColorMesh.fx"),
		SHADER_PATH))
		return false;

	if (!LoadPixelShader("ColorMeshPS", TEXT("ColorMesh.fx"),
		SHADER_PATH))
		return false;

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
		return false;

	return true;
}
