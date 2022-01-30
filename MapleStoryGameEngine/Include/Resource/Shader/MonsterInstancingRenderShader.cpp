
#include "MonsterInstancingRenderShader.h"

CMonsterInstancingRenderShader::CMonsterInstancingRenderShader()
{
	SetTypeID<CMonsterInstancingRenderShader>();
}

CMonsterInstancingRenderShader::~CMonsterInstancingRenderShader()
{
}

bool CMonsterInstancingRenderShader::Init()
{
	if (!LoadVertexShader("MonsterInstancedVS", TEXT("Mesh2D.fx"), SHADER_PATH))
		return false;

	if (!LoadGeometryShader("MonsterInstancedGS", TEXT("Mesh2D.fx"), SHADER_PATH))
		return false;

	if (!LoadPixelShader("MonsterInstancedPS", TEXT("Mesh2D.fx"), SHADER_PATH))
		return false;

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
		return false;

	return true;
}