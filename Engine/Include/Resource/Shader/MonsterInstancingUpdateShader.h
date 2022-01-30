#pragma once
#include "ComputeShader.h"

class CMonsterInstancingUpdateShader :
    public CComputeShader
{
    friend class CShaderManager;

protected:
    CMonsterInstancingUpdateShader();
    virtual ~CMonsterInstancingUpdateShader();

public:
    virtual bool Init();
};

