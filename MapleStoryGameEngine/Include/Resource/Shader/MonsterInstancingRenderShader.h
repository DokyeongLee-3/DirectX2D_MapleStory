#pragma once

#include "GraphicShader.h"

class CMonsterInstancingRenderShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CMonsterInstancingRenderShader();
    virtual ~CMonsterInstancingRenderShader();

public:
    virtual bool Init();
};


