#pragma once
#include "IMGUIWidget.h"
class CIMGUILabel :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUILabel();
	virtual ~CIMGUILabel();

protected:
	// 버튼내 텍스트 정렬위치
	ImVec2	m_Align;

public:
	void SetAlign(float x, float y)
	{
		m_Align = ImVec2(x, y);
	}

public:
	virtual bool Init();
	virtual void Render();
};

