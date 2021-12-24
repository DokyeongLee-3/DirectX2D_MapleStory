#pragma once

#include "../Shader/GraphicShader.h"
#include "../Texture/Texture.h"

struct MaterialTextureInfo
{
    std::string Name;
    Sampler_Type SamplerType;
    CSharedPtr<CTexture>    Texture;
    // CMaterial::Render���� ���� �ִ� m_TextureInfo�� �ִ� Texture���� ���鼭
    // SetShader�� ȣ���������ν�, **SetShaderResource(ex. VSSetShaderResource)�� ���ٶ� 
    // ù��° ������ StartSlot�� ���� Register�� ��Ƶд�.
    // TextureŸ���� ArrayŸ���� �ƴϸ�(ex. Atlas, Frame) �ؽ��� Ŭ������ �ϳ��� �ؽ��ĸ� ���� �ְ�,
    // ArrayŸ���� TextureŬ������ �������� �ؽ��ĸ� ���� �ִٴ� �ǹ��̴�.
    //  ArrayŸ���̸� **SetShaderResource�Լ��� ù��° ������ StartSlot�� ����
    // �Ʒ��� Register�� �ؽ��ĸ��� �ٸ���, **SetShaderResource �Լ��� 2��°������
    // NueViews�� ������ �ؽ��ĵ��� �����ؼ� ���ο� �ؽ��ĸ� ���� �� �� �ִ�
    // ���� : https://copynull.tistory.com/266
    int             Register;
    int             ShaderType;

    MaterialTextureInfo() :
        SamplerType(Sampler_Type::Linear),
        Register(0),
        ShaderType((int)ConstantBuffer_Shader_Type::Pixel)
    {
    }
};

// Material�� ������۸� ���� �־�� �Ѵ�
// Material�� Material���� ���̴��� ���� �־�� �Ѵ�
class CMaterial :
    public CRef
{
    friend class CMaterialManager;

protected:
    CMaterial();
    CMaterial(const CMaterial& Material);
    virtual ~CMaterial();

private:
    // Scene�� �Ҽӵ��� ���� �������� Material�� m_Scene = nullptr�� ����
    // Material�� Ư�� Scene�� �ҼӵǾ� �ִٸ� SceneResource�� ���ؼ� Texture�� ã�ų� Load�� �� ����
    class CScene* m_Scene;

public:
    void SetScene(class CScene* Scene)
    {
        m_Scene = Scene;
    }

protected:
    CSharedPtr<CGraphicShader>  m_Shader;
    std::vector<MaterialTextureInfo>    m_TextureInfo;
    Vector4     m_BaseColor;
    // ������
    float       m_Opacity;
    // 3���� Render State(Blend State, DepthStencil State, Rasterizer State)
    // ��� ���� �ְ� RenderState���� SetState�� ȣ�����༭
    // Render State���� ��� ���ձ�ܰ迡 Set�ǵ��� �Ѵ�
    CSharedPtr<class CRenderState>  m_RenderStateArray[(int)RenderState_Type::Max];
    class CMaterialConstantBuffer* m_CBuffer;

private:
    void SetConstantBuffer(class CMaterialConstantBuffer* Buffer)
    {
        m_CBuffer = Buffer;
    }

public:
    CTexture* GetTexture(int TextureIndex = 0)  const
    {
        return m_TextureInfo[TextureIndex].Texture;
    }

    unsigned int GetTextureWidth(int TextureIndex = 0)  const
    {
        return m_TextureInfo[TextureIndex].Texture->GetWidth();
    }

    unsigned int GetTextureHeight(int TextureIndex = 0)  const
    {
        return m_TextureInfo[TextureIndex].Texture->GetHeight();
    }

    const std::string& GetTextureName(int TextureIndex = 0)  const
    {
        return m_TextureInfo[TextureIndex].Name;
    }


public:
    void SetRenderState(class CRenderState* State);
    void SetRenderState(const std::string& Name);
    void SetTransparency(bool Enable);
    void SetOpacity(float Opacity);
    void AddOpacity(float Opacity);

public:
    void SetBaseColor(const Vector4& Color);
    void SetBaseColor(float r, float g, float b, float a);

public:
    // �̹� Load�Ǿ� �ִ� Texture�� m_TextureInfo�� �������� �Լ�
    void AddTexture(int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
    // Load�Ǿ� ���� ���� Texture�� Load�ϰ� m_TextureInfo�� �������� �Լ�
    void AddTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void AddTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
    void AddTexture(int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);

    // �̹� Load�Ǿ� �ִ� Texture�� m_TextureInfo�� �������� �Լ�
    void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
    // Load�Ǿ� ���� ���� Texture�� Load�ϰ� m_TextureInfo�� �������� �Լ�
    void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
    void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);

public:
    void SetShader(const std::string& Name);
    void Render();
    void Reset();
    CMaterial* Clone();
    void Save(FILE* File);
    void Load(FILE* File);
};

