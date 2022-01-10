#pragma once

#include "AnimationSequence2DData.h"

class CAnimationSequence2DInstance
{
public:
	CAnimationSequence2DInstance();
	CAnimationSequence2DInstance(const CAnimationSequence2DInstance& Anim);
	virtual ~CAnimationSequence2DInstance();

private:
	size_t		m_TypeID;
	class CSpriteComponent* m_Owner;
	class CScene* m_Scene;
	// CAnimationSequence2DData의 이름을 Key, CAnimationSequence2DData* 를 Value로
	std::unordered_map<std::string, CAnimationSequence2DData*>	m_mapAnimation;
	CAnimationSequence2DData* m_CurrentAnimation;
	class CAnimation2DConstantBuffer* m_CBuffer;
	bool		m_PlayAnimation;
	bool m_Flip;

public:
	size_t GetTypeID()	const
	{
		return m_TypeID;
	}

	template <typename T>
	void SetTypeID()
	{
		m_TypeID = typeid(T).hash_code();
	}

	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

	void SetOwner(class CSpriteComponent* Owner)
	{
		m_Owner = Owner;
	}

	class CSpriteComponent* GetOwner()	const
	{
		return m_Owner;
	}


	int GetAnimationCount()	const
	{
		return (int)m_mapAnimation.size();
	}

	void Play()
	{
		m_PlayAnimation = true;
	}

	void Stop()
	{
		m_PlayAnimation = false;
	}

	bool IsPlay()	const
	{
		return m_PlayAnimation;
	}

	CAnimationSequence2DData* GetCurrentAnimation()	const
	{
		return m_CurrentAnimation;
	}

	void InsertAnimationSequence2DData(CAnimationSequence2DData* Data)
	{
		m_mapAnimation.insert(std::make_pair(Data->GetName(), Data));
	}

	CAnimationSequence2DData* GetAnimationSequence2DData(int Index) const
	{
		auto iter = m_mapAnimation.begin();

		std::advance(iter, Index);

		return iter->second;
	}

	bool IsFlip()	const
	{
		return m_Flip;
	}

public:
	void AddAnimation(const std::string& SequenceName, const std::string& Name, bool Loop = true, float PlayTime = 1.f,
		float PlayScale = 1.f, bool Reverse = false);
	void AddAnimation(const TCHAR* FileName, const std::string& PathName, const std::string& Name, bool Loop = true, float PlayTime = 1.f,
		float PlayScale = 1.f, bool Reverse = false);
	void DeleteAnimation(const std::string& Name);
	void SetPlayTime(const std::string& Name, float PlayTime);
	void SetPlayScale(const std::string& Name, float PlayScale);
	void SetReverse(const std::string& Name, bool Reverse);
	void SetLoop(const std::string& Name, bool Loop);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	bool CheckCurrentAnimation(const std::string& Name);
	// 애니메이션 좌우 반전
	void Flip();
	// map에 저장된 애니메이션중 다음 애니메이션으로 현재 애니메이션을 바꾼다
	void NextAnimation();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetShader();
	void ResetShader();
	virtual CAnimationSequence2DInstance* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	// 갖고 있는 AnimationData중에서 Name매칭해서 찾아줌
	// AnimationManager는 
	CAnimationSequence2DData* FindAnimation(const std::string& Name);

public:
	template <typename T>
	void SetEndFunction(const std::string& Name, T* Obj, void (T::* Func)())
	{
		CAnimationSequence2DData* Data = FindAnimation(Name);

		if (!Data)
			return;

		Data->SetEndFunction<T>(Obj, Func);
	}

	template <typename T>
	void AddNotify(const std::string& Name, const std::string& NotifyName, int Frame,
		T* Obj, void (T::* Func)())
	{
		CAnimationSequence2DData* Data = FindAnimation(Name);

		if (!Data)
			return;

		Data->AddNotify<T>(NotifyName, Frame, Obj, Func);
	}
};

