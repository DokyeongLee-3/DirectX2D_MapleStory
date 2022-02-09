#pragma once

#include "ComputeShader.h"

// Particle ���� Update�� Shader
// Particle�� Render�ϴ� �뵵(VS, GS, PS���� ������������ �����ϴ� ���̴�)�� Shader�� �����ȴ�
class CParticleUpdateShader :
    public CComputeShader
{
    friend class CShaderManager;

protected:
    CParticleUpdateShader();
    CParticleUpdateShader(const CParticleUpdateShader& shader);
    virtual ~CParticleUpdateShader();

public:
    virtual bool Init();
    virtual CParticleUpdateShader* Clone();
};

