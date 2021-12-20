#pragma once

#include "../../Ref.h"

// shader�� �� ������ ���� ����
// 1. Graphic Shader 2. Compute Shader
// Graphic Shader�� ������ ���� Shader
// Compute�� �������� ���� ��� �뵵 Shader
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
    // Shader�� ������ ���������ο� �������ִ� �Լ�
    virtual void SetShader() = 0;
};

