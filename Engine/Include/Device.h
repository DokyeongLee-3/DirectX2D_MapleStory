#pragma once

#include "GameInfo.h"

class CDevice
{
private:
	// DX���� ����ϴ� ���ҽ�(ex. �ؽ���, ����)����
	// �������� �뵵
	ID3D11Device* m_Device;
	// �� Device���� ���� ���ҽ��� �̿��ؼ�
	// �������ϴ� �뵵
	ID3D11DeviceContext* m_Context;
	// ����۸� ���� ������ ������ �ø����� ���ִ� �뵵
	IDXGISwapChain* m_SwapChain;


	// BackBuffer�� �ȼ��� ���� ���� RenderTartgetView�� �����
	// BackBuffer�� �� TargetView�� �������ش�
	ID3D11RenderTargetView* m_TargetView;

	// ����, ���ٽ� ���� ���� ���� DepthStencilView�� �����
	// �������ش�
	ID3D11DepthStencilView* m_DepthView;

	HWND	m_hWnd;
	Resolution m_RS;


private:
	// UI�� D2D ����Ÿ��
	ID2D1RenderTarget* m_2DTarget;
	// Object�� D2D ����Ÿ��
	ID2D1RenderTarget* m_2DTargetWorld;
	// D2D ����Ÿ���� ������� �� �ִ� Factory
	ID2D1Factory* m_2DFactory;

public:
	ID2D1RenderTarget* Get2DRenderTarget()	const
	{
		return m_2DTarget;
	}

	ID2D1RenderTarget* Get2DWorldRenderTarget()	const
	{
		return m_2DTargetWorld;
	}

	ID2D1Factory* Get2DFactory()	const
	{
		return m_2DFactory;
	}
	
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

	// ���� �ػ� ����, �ػ� ���� ��� Ŭ���̾�Ʈ ���� Width, Height
	// ������ 1:1 �������ٵ�, Ŭ���̾�Ʈ ���� Width�� Height�� �ٲ������
	// ���콺 ���� ��ǥ�� �̵����� �ٲ��״ϱ�, ���� �ػ� ���
	// Ŭ���̾�Ʈ ���� ũ�� ������ ���ؾ� �Ѵ�(���ؼ� ���콺 ��ġ�� �̵�����
	// ���� ������ �� �ִ�)
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

