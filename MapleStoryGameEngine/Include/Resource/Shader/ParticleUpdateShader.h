#pragma once

#include "ComputeShader.h"

// Particle 정보 Update용 Shader
// Particle을 Render하는 용도(VS, GS, PS등의 파이프라인을 실행하는 셰이더)의 Shader와 구별된다
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

