#pragma once

#include "../GameInfo.h"
#include "../Resource/Animation/AnimationSequence2D.h"

struct Animation2DNotify
{
	std::string Name;
	int Frame;
	bool	Call;
	std::function<void()>	Function;

	Animation2DNotify() :
		Frame(0),
		Call(false)
	{
	}
};

// Sequence�� Notify�� �����Ӽ�, �ִϸ��̼� ����ð� ��
// ���� ������ ��� �ִ� Ŭ����
class CAnimationSequence2DData
{
	friend class CAnimationSequence2DInstance;
	friend class CSpriteComponent;

private:
	CAnimationSequence2DData();
	~CAnimationSequence2DData();

private:
	std::string	m_Name;
	std::string	m_SequenceName;
	CSharedPtr<class CAnimationSequence2D>	m_Sequence;
	int		m_Frame;		// ���� �ִϸ��̼� ������
	float	m_Time;			// �ִϸ��̼� ���� �ð�
	float	m_FrameTime;	// 1�����Ӵ� �ð�
	float	m_PlayTime;
	float	m_PlayScale;		// ��� ����
	bool	m_Loop;
	bool	m_Reverse;
	std::function<void()>	m_EndFunction;
	std::vector<Animation2DNotify*>	m_vecNotify;

public:
	void Save(FILE* File);
	void Load(FILE* File);


public:
	void SetLoop(bool Loop)
	{
		m_Loop = Loop;
	}

	void SetReverse(bool Reverse)
	{
		m_Reverse = Reverse;
	}

	const std::string& GetName()	const
	{
		return m_Name;
	}

	bool IsLoop()	const
	{
		return m_Loop;
	}

	bool IsReverse()	const
	{
		return m_Reverse;
	}

	float GetPlayTime()	const
	{
		return m_PlayTime;
	}

	void SetPlayTime(float PlayTime)
	{
		m_PlayTime = PlayTime;
	}

	int GetFrameCount()	const
	{
		return m_Sequence->GetFrameCount();
	}

	float GetPlayScale()	const
	{
		return m_PlayScale;
	}

	void SetPlayScale(float PlayScale)
	{
		m_PlayScale = PlayScale;
	}

	float GetFrameTime()	const
	{
		return m_FrameTime;
	}

	int GetCurrentFrame()	const
	{
		return m_Frame;
	}

	void AddFrameData(const AnimationFrameData& FrameData)
	{
		m_Sequence->AddFrame(FrameData.Start, FrameData.Size);
	}

	void AddFrameData(const Vector2& Start, const Vector2& Size)
	{
		m_Sequence->AddFrame(Start, Size);
	}

	class CAnimationSequence2D* GetAnimationSequence()	const;

	const AnimationFrameData& GetFrameData(int Index)	const;

	void SetCurrentAnimationFirstFrame();

public:
	template <typename T>
	void SetEndFunction(T* Obj, void(T::* Func)())
	{
		m_EndFunction = std::bind(Func, Obj);
	}

	template <typename T>
	void AddNotify(const std::string& Name, int Frame,
		T* Obj, void(T::* Func)())
	{
		Animation2DNotify* Notify = new Animation2DNotify;

		Notify->Name = Name;
		Notify->Frame = Frame;
		Notify->Function = std::bind(Func, Obj);

		m_vecNotify.push_back(Notify);
	}
};

