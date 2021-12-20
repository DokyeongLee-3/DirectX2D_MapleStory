#pragma once

#include "../../Ref.h"

// 상수버퍼도 레지스터 번호(m_Register에 담고 있음)로 구별한다
class CConstantBuffer :
    public CRef
{

    friend class CShaderManager;

private:
    CConstantBuffer();
    ~CConstantBuffer();

private:
    ID3D11Buffer*   m_Buffer;
    int m_Size;
    int m_Register;
    // 어떤 셰이더에 넘길 것인지
    int m_ConstantBufferShaderType;

public:
    bool Init(int Size, int Register, int ConstantBufferShaderType);
    void UpdateBuffer(void* Data);
};

