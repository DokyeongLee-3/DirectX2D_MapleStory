#pragma once
#include "IMGUIWidget.h"
class CIMGUISameLine :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUISameLine();
	virtual ~CIMGUISameLine();

protected:
	// OffsetX 는 윈도우 창의 왼쪽으로부터 어느정도 위치만큼 떨어뜨린 후에 그려낼지를 정한다.
	// 0이면 윈도우 가장 왼쪽변에 딱 붙은 상태를 의미한다
	float	m_OffsetX;

	// Offset으로부터 떨어질 거리를 의미한다.
	// Offset, Spacing 적용했는데 옆 위젯과 닿아 있으면 
	// 시작위치(Offset)은 왼쪽 위젯의 오른쪽 끝부터로 적용되고
	// 그러고나서 Spacing을 적용
	float	m_Spacing;

public:
	void SetOffsetX(float OffsetX)
	{
		m_OffsetX = OffsetX;
	}

	void SetSpacing(float Spacing)
	{
		m_Spacing = Spacing;
	}

public:
	virtual bool Init();
	virtual void Render();
};

