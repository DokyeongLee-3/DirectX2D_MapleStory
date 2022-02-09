#pragma once

#include "../Shader/GraphicShader.h"
#include "../Texture/Texture.h"
#include "../Shader/MaterialConstantBuffer.h"

struct MaterialTextureInfo
{
    std::string Name;
    Sampler_Type SamplerType;
    CSharedPtr<CTexture>    Texture;
    // CMaterial::Render에서 갖고 있는 m_TextureInfo에 있는 Texture들을 돌면서
    // SetShader를 호출해줌으로써, **SetShaderResource(ex. VSSetShaderResource)를 해줄때 
    // 첫번째 인자인 StartSlot을 여기 Register에 담아둔다. 
    // Register는 ShaderInfo.fx에서 볼수 있듯이 Texture2D g_BaseTexture : register(t0); 이렇게 선언해놨듯이
    // Texture용 레지스터 0번을 사용하도록 해놓았기 때문에 Array타입이 아니면 0번을 쓴다
    // Texture타입이 Array타입이 아니면(ex. Atlas, Frame) 텍스쳐 클래스가 하나의 텍스쳐만 갖고 있고,
    // Array타입은 Texture클래스가 여러개의 텍스쳐를 갖고 있다는 의미이다.
    //  Array타입이면 **SetShaderResource함수의 첫번째 인자인 StartSlot에 쓰일
    // 아래에 Register도 텍스쳐마다 다르고, **SetShaderResource 함수의 2번째인자인
    // NueViews로 복수의 텍스쳐들을 블렌딩해서 새로운 텍스쳐를 만들어서 쓸 수 있다
    // 참고 : https://copynull.tistory.com/266
    int             Register;
    int             ShaderType;

    MaterialTextureInfo() :
        SamplerType(Sampler_Type::Linear),
        Register(0),
        ShaderType((int)Buffer_Shader_Type::Pixel)
    {
    }
};

struct RenderCallback
{
    std::function<void()>   Func;
    void* Obj;
};


// Material도 상수버퍼를 갖고 있어야 한다
// Material도 Material만의 셰이더를 갖고 있어야 한다
class CMaterial :
    public CRef
{
    friend class CMaterialManager;

protected:
    CMaterial();
    CMaterial(const CMaterial& Material);
    virtual ~CMaterial();

private:
    // Scene에 소속되지 않은 전역적인 Material은 m_Scene = nullptr일 것임
    // Material이 특정 Scene에 소속되어 있다면 SceneResource를 통해서 Texture를 찾거나 Load할 수 있음
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
    // 불투명도
    float       m_Opacity;
    // 3가지 Render State(Blend State, DepthStencil State, Rasterizer State)
    // 모두 갖고 있고 RenderState들의 SetState를 호출해줘서
    // Render State들이 출력 병합기단계에 Set되도록 한다
    CSharedPtr<class CRenderState>  m_RenderStateArray[(int)RenderState_Type::Max];
    class CMaterialConstantBuffer* m_CBuffer;
    std::list<RenderCallback*>    m_RenderCallback;

private:
    void SetConstantBuffer(class CMaterialConstantBuffer* Buffer)
    {
        //m_CBuffer = Buffer;
        m_CBuffer = Buffer->Clone();
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
    // 이미 Load되어 있는 Texture를 m_TextureInfo로 가져오는 함수
    void AddTexture(int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
    // Load되어 있지 않은 Texture를 Load하고 m_TextureInfo로 가져오는 함수
    void AddTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void AddTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
    void AddTexture(int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);

    // 이미 Load되어 있는 Texture를 m_TextureInfo로 가져오는 함수
    void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
    // Load되어 있지 않은 Texture를 Load하고 m_TextureInfo로 가져오는 함수
    void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
    void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);

public:
    void SetPaperBurn(bool Enable);

public:
    template <typename T>
    void AddRenderCallback(T* Obj, void(T::*Func)())
    {
        RenderCallback* Callback = new RenderCallback;
        Callback->Obj = Obj;
        Callback->Func = std::bind(Func, Obj);
        m_RenderCallback.push_back(Callback);
    }

    void DeleteRenderCallback(void* Obj)
    {
        auto    iter = m_RenderCallback.begin();
        auto    iterEnd = m_RenderCallback.end();

        for (; iter != iterEnd; ++iter)
        {
            if ((*iter)->Obj == Obj)
            {
                SAFE_DELETE((*iter));
                m_RenderCallback.erase(iter);
                break;
            }
        }
    }

public:
    void SetShader(const std::string& Name);
    void Render();
    void Reset();
    CMaterial* Clone();
    void Save(FILE* File);
    void Load(FILE* File);
};

