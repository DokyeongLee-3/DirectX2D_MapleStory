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
	// OffsetX �� ������ â�� �������κ��� ������� ��ġ��ŭ ����߸� �Ŀ� �׷������� ���Ѵ�.
	// 0�̸� ������ ���� ���ʺ��� �� ���� ���¸� �ǹ��Ѵ�
	float	m_OffsetX;

	// Offset���κ��� ������ �Ÿ��� �ǹ��Ѵ�.
	// Offset, Spacing �����ߴµ� �� ������ ��� ������ 
	// ������ġ(Offset)�� ���� ������ ������ �����ͷ� ����ǰ�
	// �׷����� Spacing�� ����
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

