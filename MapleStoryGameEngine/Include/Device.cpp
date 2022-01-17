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

	// 폰트 출력을 위해서 DirectWrite를 사용할 것인데 DirectWrite는 Direct2D를 사용한다. 
	// 아래에 만든 RenderTarget은 3D용인데 Direct2D의 렌더 타겟도 만들려면
	// 이 BGRA_SUPPORT flag가 true여야 사용가능하다
	unsigned int Flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG

	Flag |= D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

	D3D_FEATURE_LEVEL FLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL FLevel1 = D3D_FEATURE_LEVEL_11_0;
	
	// 맨처음 들어가는 pFeatureLevels는 지원 가능한 피처 레벨 배열의 시작주소
	// 만약 생성에 성공했다면 pFeatureLevels에 넘겨줬던 배열의 시작주소가
	// pFeatureLevel에 반환될 것임
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
	// 32bit에서 r,g,b,a 8bit씩 사용하는 픽셀 포맷을 사용할것
	// 여기서 U는 unsigned를 의미하고 NORM은 정규화(normalize)된 것을
	// 의미한다. 따라서 0~255사이의 값을 0~1 사이의 값으로 정규화 시켰다는 의미 
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// 모니터 주사율과 상관없이 60프레임으로 맞출 것임
	SwapDesc.BufferDesc.RefreshRate.Numerator = 1;
	SwapDesc.BufferDesc.RefreshRate.Denominator = 60;
	// 물체의 확대/축소 했을때 깨짐현상을 최소화하려는 필터
	SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// 백버퍼 개수
	SwapDesc.BufferCount = 1;
	// 이런 백버퍼는 항상 렌더 타겟뷰와 연결되어 있어야 하므로
	// Usage에 RenderTarget이 설정되어 있어야 한다
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapDesc.OutputWindow = hWnd;
	// 안티앨리어싱 지원 여부
	SwapDesc.SampleDesc.Count = SampleCount;
	SwapDesc.SampleDesc.Quality = 0;
	// 창모드 여부(풀스크린 모드를 독점모드라고 부른다)
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

	// 백버퍼의 타입. 이 타입을 __uuidof함수에 넣어주면
	// 이 타입에 맞는 IID를 반환해줘서 그 IID를
	// GetBuffer의 두번째 인자로 넣어주면 백버퍼를 가져올 수 있다
	ID3D11Texture2D* BackBuffer = nullptr;

	// SwapChain이 갖고 있는 백버퍼를 얻어온다
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(void**)&BackBuffer);

	// BackBuffer와 연결된 RenderTargetView를 생성한다
	m_Device->CreateRenderTargetView(BackBuffer, nullptr,
		&m_TargetView);

	// 위에서 GetBuffer를 하면 BackBuffer에 대한 ref count를 1 증가시키므로
	// 여기서 Release해줘야한다
	SAFE_RELEASE(BackBuffer);
	
	// 깊이 버퍼를 위한 텍스쳐를 만든다
	D3D11_TEXTURE2D_DESC DepthDesc = {};

	DepthDesc.Width = Width;
	DepthDesc.Height = Height;
	DepthDesc.ArraySize = 1;

	// 깊이 버퍼는 24비트로 정규화된 값으로,
	// 스텐실 버퍼는 8비트로 표현 
	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 깊이 버퍼는 출력 용도(출력 용도는 쉐이더 리소스로 들어감)가
	// 아니고 Depth/Stencil용도이다
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	/* DX에서 사용하는 버퍼는 모두
	D3D11_USAGE_DEFAULT,
	D3D11_USAGE_IMMUTABLE,
	D3D11_USAGE_DYNAMIC,
	D3D11_USAGE_STAGING
	이 넷중 하나에 속하는데 D3D11_USAGE_DEFAULT는 비디오 메모리에
	버퍼의 데이터를 저장. 데이터의 수정이 비교적 자주 있지 않을때 사용
	D3D11_USAGE_IMMUTABLE은 수정이 절대 불가능, 출력용으로만
	사용 가능
	D3D11_USAGE_DYNAMIC은 수정 가능(lock/unlock을 이용해서
	atomic한 수정을 보장) 비디오 메모리에 저장을 하지만 RAM에 복사본을
	만들어 놓고 수정도 RAM에 있는거로 하고 수정이 끝나면 RAM에 있는
	데이터를 비디오 메모리로 고속 복사. 단 Dynamic은 GPU에서 읽기만
	가능하고 CPU에서는 RAM에 있는걸 쓰기만 가능하다 */
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthDesc.SampleDesc.Count = SampleCount;
	DepthDesc.SampleDesc.Quality = 0;
	DepthDesc.MipLevels = 1;

	ID3D11Texture2D* DepthBuffer = nullptr;

	m_Device->CreateTexture2D(&DepthDesc, nullptr, &DepthBuffer);

	m_Device->CreateDepthStencilView(DepthBuffer, nullptr, &m_DepthView);

	SAFE_RELEASE(DepthBuffer);

	// 뷰포트(출력될 영역을 지정) 생성
	D3D11_VIEWPORT VP = {};
	VP.Width = (float)Width;
	VP.Height = (float)Height;
	VP.MaxDepth = 1.f;

	// 뷰포트는 Context를 이용해서 만듬
	m_Context->RSSetViewports(1, &VP);
 

	// TextUI를 위한 2D렌더타겟을 만들어주는 2D Factory생성
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_2DFactory)))
		return false;

	// 기존에 만들어뒀던 3D Backbuffer를 이용해서
	// IDXGISurface* 타입의 surface를 얻어와서 얻어온 surface로
	// ID2D1RenderTarget타입의 렌더타겟을 만들고 거기에 Write하면
	// 기존에 만든 3D용도의 D3D에 2D인 DWrite를 쓸 수 있다
	// Direct2D와 Direct3D와의 상호호환을 위해 DirectX는 DXGI를 사용한다
	IDXGISurface* BackSurface = nullptr;

	// D3D에서는 백버퍼 Surface를 ID3D11Texture2D타입으로 받았다면
	// D2D에서는 백버퍼 Surface를 IDXGISurface으로 받는게 다르다
	m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&BackSurface));

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	// 기존에 만든 3D용 백버퍼와 호환되는 렌더
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
	// 출력병합기(Output Merger)단계에서 이 값을 넣으라고
	// 명령하는 것. 이걸 매 프레임마다 하는 이유는 나중에
	// RenderTargetView를 바꿀 수 있기 때문이다
	// 렌더링 파이프라인의 출력을 받을 자원으로 RenderTargetView와 DepthStencilView를 Set해준다
	m_Context->OMSetRenderTargets(1, &m_TargetView, m_DepthView);
}

void CDevice::Flip()
{
	m_SwapChain->Present(0, 0);
}
