#pragma once

#include "GameInfo.h"

class CDevice
{
private:
	// DX에서 사용하는 리소스(ex. 텍스쳐, 버퍼)들을
	// 만들어놓는 용도
	ID3D11Device* m_Device;
	// 위 Device에서 만든 리소스를 이용해서
	// 렌더링하는 용도
	ID3D11DeviceContext* m_Context;
	// 백버퍼를 갖고 있으며 페이지 플리핑을 해주는 용도
	IDXGISwapChain* m_SwapChain;


	// BackBuffer에 픽셀을 쓰기 위해 RenderTartgetView를 만들고
	// BackBuffer를 이 TargetView에 연결해준다
	ID3D11RenderTargetView* m_TargetView;

	// 깊이, 스텐실 값을 쓰기 위해 DepthStencilView를 만들고
	// 연결해준다
	ID3D11DepthStencilView* m_DepthView;

	HWND	m_hWnd;
	Resolution m_RS;

	

public:
	ID3D11Device* GetDevice()	const
	{
		return m_Device;
	}

	ID3D11DeviceContext* GetContext()	const
	{
		return m_Context;
	}

	IDXGISwapChain* GetSwapChain()	const
	{
		return m_SwapChain;
	}

	Resolution GetResolution()	const
	{
		return m_RS;
	}

	// 원래 해상도 가로, 해상도 세로 대비 클라이언트 영역 Width, Height
	// 비율이 1:1 비율일텐데, 클라이언트 영역 Width나 Height를 바꿔버리면
	// 마우스 현재 좌표나 이동량도 바뀔테니까, 현재 해상도 대비
	// 클라이언트 영역 크기 비율을 구해야 한다(구해서 마우스 위치나 이동량을
	// 새로 구해줄 수 있다)
	Vector2 GetViewportAspectRatio();

public:
	bool Init(HWND hWnd, unsigned int Width, unsigned int Height,
		bool WindowMode);
	void ClearRenderTarget(float ClearColor[4]);
	void ClearDepthStencil(float Depth, unsigned char Stencil);
	void RenderStart();
	void Flip();

	DECLARE_SINGLE(CDevice)
};

