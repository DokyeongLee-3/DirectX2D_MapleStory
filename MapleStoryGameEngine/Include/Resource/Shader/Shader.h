#pragma once

#include "../../Ref.h"

// shader의 두 종류로 나눌 것임
// 1. Graphic Shader 2. Compute Shader
// Graphic Shader는 렌더링 관련 Shader
// Compute는 렌더링을 위한 계산 용도 Shader
class CShader :
    public CRef
{
    friend class CShaderManager;

protected:
    CShader();
    virtual ~CShader() = 0;

protected:
    Shader_Type m_Type;

public:
    Shader_Type GetShaderType() const
    {
        return m_Type;
    }

public:
    virtual bool Init() = 0;
    // Shader를 렌더링 파이프라인에 세팅해주는 함수
    virtual void SetShader() = 0;
};

