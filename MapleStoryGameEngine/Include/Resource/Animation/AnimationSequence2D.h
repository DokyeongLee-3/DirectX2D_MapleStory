#pragma once

#include "../../Ref.h"


// CAnimationSequence2D는 AnimationManager에 넣어놓고
// AnimationInstance랑 CSceneResource랑 SharedPtr로 공유해서 사용
// -> 이 말은 공유하고 있는 곳중 한곳에서만 바꾸면 나머지에서도
// 다 적용된다는 의미
class CAnimationSequence2D :
    public CRef
{
    friend class CAnimationManager;
    friend class CAnimationSequence2DData;

protected:
    CAnimationSequence2D();
    ~CAnimationSequence2D();

private:
    class CScene* m_Scene;
    CSharedPtr<class CTexture>  m_Texture;
    std::vector<AnimationFrameData> m_vecFrameData;

public:
    void SetScene(class CScene* Scene)
    {
        m_Scene = Scene;
    }


public:
    class CTexture* GetTexture()    const
    {
        return m_Texture;
    }

    const AnimationFrameData& GetFrameData(int Index)   const
    {
        return m_vecFrameData[Index];
    }

    void SetFrameData(int Index, const Vector2& Start, const Vector2& Size)
    {
        if ((int)m_vecFrameData.size() - 1 < Index || Index < 0)
            return;

        m_vecFrameData[Index].Start = Start;
        m_vecFrameData[Index].Size = Size;
    }

    int GetFrameCount() const
    {
        return (int)m_vecFrameData.size();
    }

    void DeleteFrame(int Index);
    void ClearFrame();

public:
    bool Init(class CTexture* Texture);
    bool Init(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    void AddFrame(const Vector2& StartPos, const Vector2& Size);
    void AddFrame(float StartX, float StartY, float Width, float Height);
    bool SaveFullPath(const char* FullPath);
    bool LoadFullPath(const char* FullPath);
    bool Save(const char* FileName, const std::string& PathName);
    bool Load(const char* FileName, const std::string& PathName);
};

