#pragma once

#include "IMGUIWindow.h"

class CSpriteWindow :
	public CIMGUIWindow
{
public:
	CSpriteWindow();
	virtual ~CSpriteWindow();

private:
	// �巡���� ������ ��Ÿ���ִ� Image
	class CIMGUIImage* m_SpriteFrame;
	class CIMGUITextInput* m_LoadFileName;

	class CIMGUIRadioButton* m_SpriteEditRadioButton;
	class CIMGUIRadioButton* m_SceneEditRadioButton;
	class CIMGUIRadioButton* m_MapEditRadioButton;

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

	class CIMGUITextInput* m_AnimPlayTime;
	class CIMGUITextInput* m_AnimPlayScale;


	class CIMGUIText* m_DragPivotXPos;
	class CIMGUIText* m_DragPivotYPos;

	// Object Hierarchy���� �� Object�� ���� DefaultScene���� �ִϸ��̼� ������̶��
	// �ִϸ��̼��� ���� ������
	class CIMGUIText* m_CurrentFrameText;


public:
	CIMGUIRadioButton* GetTileMapEditRadioButton()	const
	{
		return m_MapEditRadioButton;
	}

	class CIMGUIText* GetCurrentFrameText()	const
	{
		return m_CurrentFrameText;
	}

	class CIMGUIText* GetDragPivotXPosText()	const
	{
		return m_DragPivotXPos;
	}

	class CIMGUIText* GetDragPivotYPosText()	const
	{
		return m_DragPivotYPos;
	}

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

	float GetAnimPlayTime()	const;

	void SetPlayTime(float PlayTime);

	void SetSpriteObject(class CSpriteEditObject* Obj);
	void SetSpriteFrameTexture(const std::string& Name, const TCHAR* FileName);
	void SetSpriteFrameImageStart(const Vector2 Start);
	void SetSpriteFrameImageEnd(const Vector2 End);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void LoadTextureButton();
	void SpriteEditButton();
	void MapEditButton();
	void ObjectArrangeButton();
	void AddAnimationButton();
	void AddAnimationFrameButton();
	void SelectAnimationFrame(int Index, const char* Item);
	void DeleteSequenceButton();
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

	void ChangePlayTime();
	void ChangePlayScale();

public:
	void StyleCallback();
	void MyShowStyleEditor(ImGuiStyle* ref = (ImGuiStyle*)0);

};

