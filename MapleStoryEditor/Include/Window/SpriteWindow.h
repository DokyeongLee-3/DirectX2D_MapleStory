#pragma once

#include "IMGUIWindow.h"

class CSpriteWindow :
	public CIMGUIWindow
{
public:
	CSpriteWindow();
	virtual ~CSpriteWindow();

private:
	// 드래그한 영역을 나타내주는 Image
	class CIMGUIImage* m_SpriteFrame;
	class CIMGUITextInput* m_LoadFileName;

	class CIMGUIRadioButton* m_SpriteEditRadioButton;
	class CIMGUIRadioButton* m_SceneEditRadioButton;
	class CIMGUIRadioButton* m_MapEditRadioButton;

	// 애니메이션들의 시퀀스들을 갖고 있는 리스트
	class CIMGUIListBox* m_AnimationList;
	class CIMGUIListBox* m_AnimationFrameList;
	class CIMGUITextInput* m_AnimName;
	class CIMGUITextInput* m_AnimFrameName;
	class CIMGUITextInput* m_FrameStartPosX;
	class CIMGUITextInput* m_FrameStartPosY;
	class CIMGUITextInput* m_FrameEndPosX;
	class CIMGUITextInput* m_FrameEndPosY;
	// 에디터모드에서 (클라이언트 영역에 나오는)Scene에 출력되는 Object
	CSharedPtr<class CSpriteEditObject> m_SpriteObject;
	class CAnimationSequence2DInstance* m_AnimInstance;

	class CIMGUITextInput* m_AnimPlayTime;
	class CIMGUITextInput* m_AnimPlayScale;


	class CIMGUIText* m_DragPivotXPos;
	class CIMGUIText* m_DragPivotYPos;

	// Object Hierarchy에서 고른 Object가 현재 DefaultScene에서 애니메이션 재생중이라면
	// 애니메이션의 현재 프레임
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

