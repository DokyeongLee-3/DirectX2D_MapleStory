#pragma once
#include "IMGUIWindow.h"
class CIMGUITestWindow :
	public CIMGUIWindow
{
public:
	CIMGUITestWindow();
	virtual ~CIMGUITestWindow();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);


public:
	void ClickButton1();
	void ClickButton2();

public:
	void ListBoxCallback1(int Index, const char* Name);

public:
	void RadioCallback1();
	void RadioCallback2();
	void RadioCallback3();
public:
	void CheckBoxCallback1();
};

