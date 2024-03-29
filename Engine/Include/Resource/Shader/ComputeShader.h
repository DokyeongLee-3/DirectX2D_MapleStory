#pragma once

#include "Shader.h"

class CComputeShader :
    public CShader
{
    friend class CShaderManager;

protected:
    CComputeShader();
    CComputeShader(const CComputeShader& shader);
    virtual ~CComputeShader() = 0;

private:
    ID3D11ComputeShader* m_CS;
    ID3DBlob* m_CSBlob;
    char    m_EntryName[64];
    TCHAR   m_FileName[MAX_PATH];
    std::string m_PathName;

public:
    virtual bool Init() = 0;
    // 컴파일해서 만든 Shader를 렌더링 파이프라인에 세팅해주는 함수
    virtual void SetShader();
    virtual CComputeShader* Clone();

public:
    bool LoadComputeShader(const char* EntryName, const TCHAR* FileName,
        const std::string& PathName);
    void Excute(unsigned int x, unsigned int y, unsigned int z);
};

