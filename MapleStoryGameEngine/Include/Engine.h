#pragma once

#include "GameInfo.h"

class CEngine
{
	
private:
	HINSTANCE m_hInst;
	HWND	m_hWnd;
	Resolution m_RS;
	static bool m_Loop;
	float m_ClearColor[4];
	class CTimer* m_Timer;
	// 생성한 Scene이 생성되고나서(Init에서 new로 만드는거)
	// 실제로 Engine::Run에서 시작됐는지 안됐는지
	bool		m_Start;
	bool		m_Play;
	Engine_Space	m_Space;

public:
	Engine_Space GetEngineSpace()	const
	{
		return m_Space;
	}


	bool IsPlay()	const
	{
		return m_Play;
	}

	void SetPlay(bool Play)
	{
		m_Play = Play;
	}

	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}

	HINSTANCE GetWindowInstance()	const
	{
		return m_hInst;
	}

public:
	// WindowMode는 창여부모드 여부
	bool Init(HINSTANCE hInst, const TCHAR* Name, unsigned int Width,
		unsigned int Height, int IconID,
		bool WindowMode = true);
	// 위의 Init함수는 윈도우를 하나 만들면서 Device를 초기화하지만
	// 이 Init함수는 이미 만들어져있는 윈도우에 Device를 초기화한다 
	bool Init(HINSTANCE hInst, HWND hWnd, unsigned int Width,
		unsigned int Height, bool WindowMode = true);
	// 메세지 루프
	int Run();
	void Logic();

private:
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Render(float DeltaTime);

private:
	ATOM Register(const TCHAR* Name, int IconID);
	BOOL Create(const TCHAR* Name);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	DECLARE_SINGLE(CEngine)
};

