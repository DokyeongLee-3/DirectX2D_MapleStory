#pragma once

#include "IMGUIWindow.h"

class CSpriteWindow :
	public CIMGUIWindow
{
public:
	CSpriteWindow();
	virtual ~CSpriteWindow();

private:
	// ������ â�� ������ �̹����� ���� IMGUIImage
	class CIMGUIImage* m_Sprite;
	class CIMGUIImage* m_SpriteFrame;
	class CIMGUITextInput* m_LoadFileName;

	// �ִϸ��̼ǵ��� ���������� ���� �ִ� ����Ʈ
	class CIMGUIListBox* m_AnimationList;
	class CIMGUIListBox* m_AnimationFrameList;
	class CIMGUITextInput* m_AnimName;
	class CIMGUITextInput* m_AnimFrameName;
	class CIMGUITextInput* m_FrameStartPosX;
	class CIMGUITextInput* m_FrameStartPosY;
	class CIMGUITextInput* m_FrameEndPosX;
	class CIMGUITextInput* m_FrameEndPosY;
	// �����͸�忡�� (Ŭ���̾�Ʈ ������ ������)Scene�� ��µǴ� Object
	CSharedPtr<class CSpriteEditObject> m_SpriteObject;
	class CAnimationSequence2DInstance* m_AnimInstance;

	class CIMGUICheckBox* m_LoopCheckBox;
	class CIMGUICheckBox* m_ReverseCheckBox;
	class CIMGUITextInput* m_AnimPlayTime;
	class CIMGUITextInput* m_AnimPlayScale;

	//class CAnimationLoadObject* m_EditorAnimationLoadObject;

public:
	class CSpriteEditObject* GetSpriteObject()	const
	{
		return m_SpriteObject;
	}

	CIMGUITextInput* GetFrameStartPosX()	const
	{
		return m_FrameStartPosX;
	}

	CIMGUITextInput* GetFrameStartPosY()	const
	{
		return m_FrameStartPosY;
	}

	CIMGUITextInput* GetFrameEndPosX()	const
	{
		return m_FrameEndPosX;
	}

	CIMGUITextInput* GetFrameEndPosY()	const
	{
		return m_FrameEndPosY;
	}

	void SetSpriteObject(class CSpriteEditObject* Obj);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void LoadTextureButton();
	void SpriteEditButton();
	void AddAnimationButton();
	void AddAnimationFrameButton();
	void SelectAnimationFrame(int Index, const char* Item);
	void PlayAnimation();
	void StopAnimation();
	void DeleteAnimButton();
	void DeleteFrameButton();
	void SelectAnimation(int Index, const char* Item);

	void SaveSequence();
	void LoadSequence();
	void SaveAnimation();
	void LoadAnimation();

	void AdjustFrameDataStartX();
	void AdjustFrameDataStartY();
	void AdjustFrameDataEndX();
	void AdjustFrameDataEndY();


};

