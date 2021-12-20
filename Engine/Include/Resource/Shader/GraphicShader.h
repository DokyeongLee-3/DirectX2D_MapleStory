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
    // GraphicShader���� InputLayer�� ���� �ִ� ������
    // Load*Shader�� fx���ϰ� entry point�� �Ѱ��༭ ���̴� �ڵ带 �ε��� ��
    // AddInputDesc, CreateInputLayout�� ����� �ϱ� �����̴� �̱��� �ϰ� ����
    // ���������� Shader�� Render�Ҷ� SetShader�� �ϼ��� VS,PS,HS,DS,GS����
    // Set���ֱ� �����̴�
    ID3D11InputLayout* m_InputLayout;
    // �츮�� ���� ������ Vertex�� Pos, Color �̷��� 2����
    // ����� �̷�����־ 2���� Description�� �ʿ�������
    // �ٸ��� ������ �����ϸ� �� ��������,
    // �� �������� �����Ƿ� vector�� �Է� ���̾ƿ� Description��
    // ��Ƴ��´�. Description�� �̿��ؼ� InputLayout�� �����
    std::vector<D3D11_INPUT_ELEMENT_DESC> m_vecInputDesc;
    unsigned int m_InputSize;

private:
    // Vertex Shader
    ID3D11VertexShader* m_VS;
    // ���̴� �ڵ带 ������ �� �� ����� ����Ʈ�ڵ带 �����ϴ� �������̽�
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
    <D3D11_INPUT_ELEMENT_DESC ����ü �����>
    LPCSTR SemanticName : HLSL�ڵ忡�� �� �������� �̸�
    UINT SemanticIndex : 
    DXGI_FORMAT Format : Pos�� ��� 12����Ʈ�̴ϱ� R32G32B32,
    Color�� ��� 16����Ʈ�ϱ� DXGI_FORMAT_R32G32B32A32_FLOAT
    UINT InputSlot : ���߿� Instancing�Ҷ� ���̹Ƿ� �н�
    UINT AlignedByteOffset : Pos�� VertexColor ���� 0����Ʈ���� �����̴ϱ�
    Offset�� 0, Color�� 12����Ʈ���� �����̴ϱ� Offset�� 12
    D3D11_INPUT_CLASSIFICATION InputSlotClass : Vertex �����ͳ�,
    Instance �����ͳ�
    UINT InstanceDataStepRate :
    */

    // ���� Size�� Pos�� ��� 12����Ʈ, Color�� ��� 16����Ʈ
    // �̷� Size�� �־��ָ� m_InputSize�� ����ؼ� �˾Ƽ� ���ǵ��� 
    // �ϱ� ���ؼ� �Ѱ���
    void AddInputDesc(const char* Name, unsigned int Index,
        DXGI_FORMAT Fmt, unsigned int Slot, unsigned int Size,
        D3D11_INPUT_CLASSIFICATION SlotClass,
        unsigned int InstanceDataStep);
    bool CreateInputLayout();

public:
    virtual bool Init() = 0;
    // Shader�� ������ ���������ο� �������ִ� �Լ�
    virtual void SetShader();

    // Shader�ڵ带 �ε��ϴ� �Լ���
public:
    // HLSL�ڵ忡�� ���� �����Լ��� � �Լ���
    // entry point�� �������� ���ڷ� �Ѱ���� �ϹǷ�
    // �װ� EntryName�̶�� ���ڷ� �Ѱ���
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

