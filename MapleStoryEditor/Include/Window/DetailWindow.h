#pragma once

#include "IMGUIWindow.h"

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

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void PositionXCallback();
	void PositionYCallback();
	void PositionZCallback();

private:
	void RotationXCallback();
	void RotationYCallback();
	void RotationZCallback();
};

