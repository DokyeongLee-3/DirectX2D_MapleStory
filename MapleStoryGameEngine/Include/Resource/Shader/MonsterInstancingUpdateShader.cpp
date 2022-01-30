#include "MonsterInstancingUpdateShader.h"

CMonsterInstancingUpdateShader::CMonsterInstancingUpdateShader()
{
	SetTypeID<CMonsterInstancingUpdateShader>();
}

CMonsterInstancingUpdateShader::~CMonsterInstancingUpdateShader()
{
}

bool CMonsterInstancingUpdateShader::Init()
{
	if (!LoadComputeShader("MonsterInfoUpdate", TEXT("Mesh2D.fx"), SHADER_PATH))
		return false;

	return true;
}