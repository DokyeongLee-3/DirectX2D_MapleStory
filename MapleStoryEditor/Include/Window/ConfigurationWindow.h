#pragma once

#include "IMGUIWindow.h"

class CConfigurationWindow	:
	public CIMGUIWindow
{

public:
	CConfigurationWindow();
	virtual ~CConfigurationWindow();

private:


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void StyleCallback();
	void MyShowStyleEditor(ImGuiStyle* ref = (ImGuiStyle*)0);

private:
	void MapEditMode();
	void SpriteEditMode();
};

