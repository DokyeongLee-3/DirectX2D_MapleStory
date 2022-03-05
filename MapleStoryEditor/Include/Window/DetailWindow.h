#pragma once

#include "IMGUIWindow.h"
#include "IMGUITextInput.h"

class CDetailWindow :
	public CIMGUIWindow
{	
public:
	CDetailWindow();
	virtual ~CDetailWindow();

private:
	class CIMGUITextInput* m_PosX;
	class CIMGUITextInput* m_PosY;
	class CIMGUITextInput* m_PosZ;

	class CIMGUITextInput* m_RotX;
	class CIMGUITextInput* m_RotY;
	class CIMGUITextInput* m_RotZ;

	class CIMGUITextInput* m_ScalingX;
	class CIMGUITextInput* m_ScalingY;

	class CIMGUIButton* m_FlipButton;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	class CIMGUITextInput* GetPosXInput()	const
	{
		return m_PosX;
	}

	class CIMGUITextInput* GetPosYInput()	const
	{
		return m_PosY;
	}

	class CIMGUITextInput* GetPosZInput()	const
	{
		return m_PosZ;
	}

	class CIMGUITextInput* GetRotXInput()	const
	{
		return m_RotX;
	}

	class CIMGUITextInput* GetRotYInput()	const
	{
		return m_RotY;
	}

	class CIMGUITextInput* GetRotZInput()	const
	{
		return m_RotZ;
	}

	class CIMGUITextInput* GetScaleXInput()	const
	{
		return m_ScalingX;
	}

	class CIMGUITextInput* GetScaleYInput()	const
	{
		return m_ScalingY;
	}

public:
	void SetScaleXInput(float ScaleX)
	{
		m_ScalingX->SetValueFloat(ScaleX);
	}

	void SetScaleYInput(float ScaleY)
	{
		m_ScalingY->SetValueFloat(ScaleY);
	}

private:
	void PositionXCallback();
	void PositionYCallback();
	void PositionZCallback();

private:
	void RotationXCallback();
	void RotationYCallback();
	void RotationZCallback();

private:
	void ScalingXCallback();
	void ScalingYCallback();

private:
	void FlipComponent();

public:
	void ClearDetailWindowInfo();
};

