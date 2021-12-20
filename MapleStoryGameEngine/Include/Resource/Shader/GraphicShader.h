#pragma once

#include "Shader.h"

class CGraphicShader :
    public CShader
{
    friend class CShaderManager;

protected:
    CGraphicShader();
    virtual ~CGraphicShader() = 0;

private:
    // GraphicShader에서 InputLayer을 갖고 있는 이유는
    // Load*Shader로 fx파일과 entry point를 넘겨줘서 셰이더 코드를 로드한 후
    // AddInputDesc, CreateInputLayout를 해줘야 하기 때문이다 이까지 하고 나면
    // 최종적으로 Shader를 Render할때 SetShader로 완성된 VS,PS,HS,DS,GS들을
    // Set해주기 때문이다
    ID3D11InputLayout* m_InputLayout;
    // 우리는 지금 정의한 Vertex가 Pos, Color 이렇게 2개의
    // 멤버로 이루어져있어서 2개의 Description이 필요하지만
    // 다르게 정점을 정의하면 더 많을수도,
    // 더 적을수도 있으므로 vector로 입력 레이아웃 Description을
    // 모아놓는다. Description을 이용해서 InputLayout을 만든다
    std::vector<D3D11_INPUT_ELEMENT_DESC> m_vecInputDesc;
    unsigned int m_InputSize;

private:
    // Vertex Shader
    ID3D11VertexShader* m_VS;
    // 쉐이더 코드를 컴파일 한 후 생기는 바이트코드를 관리하는 인터페이스
    ID3DBlob* m_VSBlob;

    ID3D11PixelShader* m_PS;
    ID3DBlob* m_PSBlob;

    ID3D11HullShader* m_HS;
    ID3DBlob* m_HSBlob;

    ID3D11DomainShader* m_DS;
    ID3DBlob* m_DSBlob;

    ID3D11GeometryShader* m_GS;
    ID3DBlob* m_GSBlob;

public:
    /*
    <D3D11_INPUT_ELEMENT_DESC 구조체 멤버들>
    LPCSTR SemanticName : HLSL코드에서 쓴 레지스터 이름
    UINT SemanticIndex : 
    DXGI_FORMAT Format : Pos의 경우 12바이트이니까 R32G32B32,
    Color의 경우 16바이트니까 DXGI_FORMAT_R32G32B32A32_FLOAT
    UINT InputSlot : 나중에 Instancing할때 쓰이므로 패스
    UINT AlignedByteOffset : Pos는 VertexColor 에서 0바이트부터 시작이니까
    Offset이 0, Color는 12바이트부터 시작이니까 Offset이 12
    D3D11_INPUT_CLASSIFICATION InputSlotClass : Vertex 데이터냐,
    Instance 데이터냐
    UINT InstanceDataStepRate :
    */

    // 인자 Size에 Pos의 경우 12바이트, Color의 경우 16바이트
    // 이런 Size를 넣어주면 m_InputSize를 사용해서 알아서 계산되도록 
    // 하기 위해서 넘겨줌
    void AddInputDesc(const char* Name, unsigned int Index,
        DXGI_FORMAT Fmt, unsigned int Slot, unsigned int Size,
        D3D11_INPUT_CLASSIFICATION SlotClass,
        unsigned int InstanceDataStep);
    bool CreateInputLayout();

public:
    virtual bool Init() = 0;
    // Shader를 렌더링 파이프라인에 세팅해주는 함수
    virtual void SetShader();

    // Shader코드를 로딩하는 함수들
public:
    // HLSL코드에서 만든 전역함수중 어떤 함수를
    // entry point로 지정할지 인자로 넘겨줘야 하므로
    // 그걸 EntryName이라는 인자로 넘겨줌
    bool LoadVertexShader(const char* EntryName,
        const TCHAR* FileName, const std::string& PathName);
    bool LoadPixelShader(const char* EntryName,
        const TCHAR* FileName, const std::string& PathName);
    bool LoadHullShader(const char* EntryName,
        const TCHAR* FileName, const std::string& PathName);
    bool LoadDomainShader(const char* EntryName,
        const TCHAR* FileName, const std::string& PathName);
    bool LoadGeometryShader(const char* EntryName,
        const TCHAR* FileName, const std::string& PathName);
};

