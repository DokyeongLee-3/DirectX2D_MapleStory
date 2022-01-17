#include "Device.h"
#include "Vector4.h"

DEFINITION_SINGLE(CDevice)

CDevice::CDevice()	:
	m_Device(nullptr),
	m_Context(nullptr),
	m_SwapChain(nullptr),
	m_TargetView(nullptr),
	m_DepthView(nullptr),
	m_2DTarget(nullptr),
	m_2DTargetWorld(nullptr),
	m_2DFactory(nullptr)
{

}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_2DTarget);
	SAFE_RELEASE(m_2DTargetWorld);
	SAFE_RELEASE(m_2DFactory);

	SAFE_RELEASE(m_TargetView);
	SAFE_RELEASE(m_DepthView);

	SAFE_RELEASE(m_SwapChain);

	if (m_Context)
		m_Context->ClearState();

	SAFE_RELEASE(m_Context);
	SAFE_RELEASE(m_Device);
}

Vector2 CDevice::GetViewportAspectRatio()
{
	RECT	ClientRC = {};

	GetClientRect(m_hWnd, &ClientRC);

	float	Width = (float)(ClientRC.right - ClientRC.left);
	float	Height = (float)(ClientRC.bottom - ClientRC.top);

	return Vector2(m_RS.Width / Width, m_RS.Height / Height);
}

bool CDevice::Init(HWND hWnd, unsigned int Width,
	unsigned int Height, bool WindowMode)
{
	m_hWnd = hWnd;
	m_RS.Width = Width;
	m_RS.Height = Height;

	// ��Ʈ ����� ���ؼ� DirectWrite�� ����� ���ε� DirectWrite�� Direct2D�� ����Ѵ�. 
	// �Ʒ��� ���� RenderTarget�� 3D���ε� Direct2D�� ���� Ÿ�ٵ� �������
	// �� BGRA_SUPPORT flag�� true���� ��밡���ϴ�
	unsigned int Flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG

	Flag |= D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

	D3D_FEATURE_LEVEL FLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL FLevel1 = D3D_FEATURE_LEVEL_11_0;
	
	// ��ó�� ���� pFeatureLevels�� ���� ������ ��ó ���� �迭�� �����ּ�
	// ���� ������ �����ߴٸ� pFeatureLevels�� �Ѱ���� �迭�� �����ּҰ�
	// pFeatureLevel�� ��ȯ�� ����
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, Flag,
		&FLevel, 1, D3D11_SDK_VERSION, &m_Device, &FLevel1, &m_Context)))
		return false;

	int		SampleCount = 4;

	UINT	Check = 0;
	m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &Check);

	if (Check < 1)
		SampleCount = 1;

	DXGI_SWAP_CHAIN_DESC SwapDesc = {};

	SwapDesc.BufferDesc.Width = Width;
	SwapDesc.BufferDesc.Height = Height;
	// 32bit���� r,g,b,a 8bit�� ����ϴ� �ȼ� ������ ����Ұ�
	// ���⼭ U�� unsigned�� �ǹ��ϰ� NORM�� ����ȭ(normalize)�� ����
	// �ǹ��Ѵ�. ���� 0~255������ ���� 0~1 ������ ������ ����ȭ ���״ٴ� �ǹ� 
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// ����� �ֻ����� ������� 60���������� ���� ����
	SwapDesc.BufferDesc.RefreshRate.Numerator = 1;
	SwapDesc.BufferDesc.RefreshRate.Denominator = 60;
	// ��ü�� Ȯ��/��� ������ ���������� �ּ�ȭ�Ϸ��� ����
	SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// ����� ����
	SwapDesc.BufferCount = 1;
	// �̷� ����۴� �׻� ���� Ÿ�ٺ�� ����Ǿ� �־�� �ϹǷ�
	// Usage�� RenderTarget�� �����Ǿ� �־�� �Ѵ�
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapDesc.OutputWindow = hWnd;
	// ��Ƽ�ٸ���� ���� ����
	SwapDesc.SampleDesc.Count = SampleCount;
	SwapDesc.SampleDesc.Quality = 0;
	// â��� ����(Ǯ��ũ�� ��带 ��������� �θ���)
	SwapDesc.Windowed = WindowMode;
	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGIDevice* DXGIDevice = nullptr;
	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&DXGIDevice);

	IDXGIAdapter* Adapter = nullptr;
	DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&Adapter);

	IDXGIFactory* Factory = nullptr;
	Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&Factory);

	if (FAILED(Factory->CreateSwapChain(m_Device, &SwapDesc, &m_SwapChain)))
	{
		SAFE_RELEASE(DXGIDevice);
		SAFE_RELEASE(Adapter);
		SAFE_RELEASE(Factory);
		return false;
	}

	SAFE_RELEASE(DXGIDevice);
	SAFE_RELEASE(Adapter);
	SAFE_RELEASE(Factory);

	// ������� Ÿ��. �� Ÿ���� __uuidof�Լ��� �־��ָ�
	// �� Ÿ�Կ� �´� IID�� ��ȯ���༭ �� IID��
	// GetBuffer�� �ι�° ���ڷ� �־��ָ� ����۸� ������ �� �ִ�
	ID3D11Texture2D* BackBuffer = nullptr;

	// SwapChain�� ���� �ִ� ����۸� ���´�
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(void**)&BackBuffer);

	// BackBuffer�� ����� RenderTargetView�� �����Ѵ�
	m_Device->CreateRenderTargetView(BackBuffer, nullptr,
		&m_TargetView);

	// ������ GetBuffer�� �ϸ� BackBuffer�� ���� ref count�� 1 ������Ű�Ƿ�
	// ���⼭ Release������Ѵ�
	SAFE_RELEASE(BackBuffer);
	
	// ���� ���۸� ���� �ؽ��ĸ� �����
	D3D11_TEXTURE2D_DESC DepthDesc = {};

	DepthDesc.Width = Width;
	DepthDesc.Height = Height;
	DepthDesc.ArraySize = 1;

	// ���� ���۴� 24��Ʈ�� ����ȭ�� ������,
	// ���ٽ� ���۴� 8��Ʈ�� ǥ�� 
	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// ���� ���۴� ��� �뵵(��� �뵵�� ���̴� ���ҽ��� ��)��
	// �ƴϰ� Depth/Stencil�뵵�̴�
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	/* DX���� ����ϴ� ���۴� ���
	D3D11_USAGE_DEFAULT,
	D3D11_USAGE_IMMUTABLE,
	D3D11_USAGE_DYNAMIC,
	D3D11_USAGE_STAGING
	�� ���� �ϳ��� ���ϴµ� D3D11_USAGE_DEFAULT�� ���� �޸𸮿�
	������ �����͸� ����. �������� ������ ���� ���� ���� ������ ���
	D3D11_USAGE_IMMUTABLE�� ������ ���� �Ұ���, ��¿����θ�
	��� ����
	D3D11_USAGE_DYNAMIC�� ���� ����(lock/unlock�� �̿��ؼ�
	atomic�� ������ ����) ���� �޸𸮿� ������ ������ RAM�� ���纻��
	����� ���� ������ RAM�� �ִ°ŷ� �ϰ� ������ ������ RAM�� �ִ�
	�����͸� ���� �޸𸮷� ��� ����. �� Dynamic�� GPU���� �б⸸
	�����ϰ� CPU������ RAM�� �ִ°� ���⸸ �����ϴ� */
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthDesc.SampleDesc.Count = SampleCount;
	DepthDesc.SampleDesc.Quality = 0;
	DepthDesc.MipLevels = 1;

	ID3D11Texture2D* DepthBuffer = nullptr;

	m_Device->CreateTexture2D(&DepthDesc, nullptr, &DepthBuffer);

	m_Device->CreateDepthStencilView(DepthBuffer, nullptr, &m_DepthView);

	SAFE_RELEASE(DepthBuffer);

	// ����Ʈ(��µ� ������ ����) ����
	D3D11_VIEWPORT VP = {};
	VP.Width = (float)Width;
	VP.Height = (float)Height;
	VP.MaxDepth = 1.f;

	// ����Ʈ�� Context�� �̿��ؼ� ����
	m_Context->RSSetViewports(1, &VP);
 

	// TextUI�� ���� 2D����Ÿ���� ������ִ� 2D Factory����
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_2DFactory)))
		return false;

	// ������ �����״� 3D Backbuffer�� �̿��ؼ�
	// IDXGISurface* Ÿ���� surface�� ���ͼ� ���� surface��
	// ID2D1RenderTargetŸ���� ����Ÿ���� ����� �ű⿡ Write�ϸ�
	// ������ ���� 3D�뵵�� D3D�� 2D�� DWrite�� �� �� �ִ�
	// Direct2D�� Direct3D���� ��ȣȣȯ�� ���� DirectX�� DXGI�� ����Ѵ�
	IDXGISurface* BackSurface = nullptr;

	// D3D������ ����� Surface�� ID3D11Texture2DŸ������ �޾Ҵٸ�
	// D2D������ ����� Surface�� IDXGISurface���� �޴°� �ٸ���
	m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&BackSurface));

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	// ������ ���� 3D�� ����ۿ� ȣȯ�Ǵ� ����
	if (FAILED(m_2DFactory->CreateDxgiSurfaceRenderTarget(BackSurface, props, &m_2DTarget)))
		return false;

	SAFE_RELEASE(BackSurface);

	return true;
}

void CDevice::ClearRenderTarget(float ClearColor[4])
{
	m_Context->ClearRenderTargetView(m_TargetView, ClearColor);
}

void CDevice::ClearDepthStencil(float Depth, unsigned char Stencil)
{
	m_Context->ClearDepthStencilView(m_DepthView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		Depth, Stencil);
}

void CDevice::RenderStart()
{
	// ��º��ձ�(Output Merger)�ܰ迡�� �� ���� �������
	// ����ϴ� ��. �̰� �� �����Ӹ��� �ϴ� ������ ���߿�
	// RenderTargetView�� �ٲ� �� �ֱ� �����̴�
	// ������ ������������ ����� ���� �ڿ����� RenderTargetView�� DepthStencilView�� Set���ش�
	m_Context->OMSetRenderTargets(1, &m_TargetView, m_DepthView);
}

void CDevice::Flip()
{
	m_SwapChain->Present(0, 0);
}
