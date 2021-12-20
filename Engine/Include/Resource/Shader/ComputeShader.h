#pragma once

#include "Shader.h"

class CComputeShader :
    public CShader
{
    friend class CShaderManager;

protected:
    CComputeShader();
    virtual ~CComputeShader() = 0;

public:
    virtual bool Init() = 0;
    // Shader를 렌더링 파이프라인에 세팅해주는 함수
    virtual void SetShader();
};

