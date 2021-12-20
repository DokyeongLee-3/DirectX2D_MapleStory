
#include "BlendState.h"
#include "../Device.h"

CBlendState::CBlendState()	:
	m_SampleMask(0xffffffff),
	m_PrevSampleMask(0),
	m_BlendFactor{},
	m_PrevBlendFactor{}

{
	m_Type = RenderState_Type::Blend;
}

CBlendState::~CBlendState()
{
}

void CBlendState::AddBlendInfo(bool BlendEnable, D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha, D3D11_BLEND_OP BlendOpAlpha, UINT8 RenderTargetWriteMask)
{
	D3D11_RENDER_TARGET_BLEND_DESC	Desc = {};

	Desc.BlendEnable = BlendEnable;
	Desc.SrcBlend = SrcBlend;
	Desc.DestBlend = DestBlend;
	Desc.BlendOp = BlendOp;
	Desc.SrcBlendAlpha = SrcBlendAlpha;
	Desc.DestBlendAlpha = DestBlendAlpha;
	Desc.BlendOpAlpha = BlendOpAlpha;
	Desc.RenderTargetWriteMask = RenderTargetWriteMask;

	m_vecDesc.push_back(Desc);
}

bool CBlendState::CreateState(bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	if (m_vecDesc.empty())
		return false;

	D3D11_BLEND_DESC	Desc = {};
	// 멀티샘플링에서 렌더타겟의 픽셀값을 설정할때 알파값을 사용할것인지
	// 참고로 멀티 샘플링은 거친 이미지를 부드럽게 해주는 기술
	// 이런 기술을 안티 알리어싱(Anti - Aliasing) 이라고 부르는데
	// 대표적으로 슈퍼 샘플링이랑 멀티 샘플링 기법이 있다. 이런 기법들은
	// 계단현상(픽셀이 계단처럼 나타나는 현상)을 완화 하기 위해 사용한다
	// 참고 : https://lipcoder.tistory.com/entry/DirectX-12-%EB%8B%A4%EC%A4%91%ED%91%9C%EB%B3%B8%ED%99%94Multisampling
	Desc.AlphaToCoverageEnable = AlphaToCoverageEnable;
	// IndependentBlendEnable = true면 8개의 렌더타겟에 개별적으로 적용
	// IndependentBlendEnable = false면 지금 렌더타겟에 적용하는것으로
	// 나머지 렌더타겟도 동일하게 적용한다는 의미
	Desc.IndependentBlendEnable = IndependentBlendEnable;

	memcpy(Desc.RenderTarget, &m_vecDesc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * m_vecDesc.size());
	//m_State에 ID3D11BlendState*타입의 State를 돌려준다
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBlendState(&Desc, (ID3D11BlendState**)&m_State)))
		return false;

	return true;
}

void CBlendState::SetState()
{
	// OMGetBlendState의 reference count에 대한 MSDN설명
	// The reference count of the returned interface will be incremented by one
	// when the blend state is retrieved. Applications must release returned pointer(s) 
	// when they are no longer needed, or else there will be a memory leak.
	CDevice::GetInst()->GetContext()->OMGetBlendState((ID3D11BlendState**)&m_PrevState, m_PrevBlendFactor, &m_PrevSampleMask);

	// 여기 넣어주는 m_BlendFactor는 만약 ID3D11_BLEND타입을 넣어야하는곳에
	// D3D11_BLEND_BLEND_FACTOR나 D3D11_BLEND_INV_BLEND_FACTOR를 넣어준 곳에는
	// 여기 인자로 넣어준 m_BlendFactor를 사용해서 계산한다
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)m_State, m_BlendFactor, m_SampleMask);
}

void CBlendState::ResetState()
{
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)m_PrevState, m_PrevBlendFactor, m_PrevSampleMask);

	SAFE_RELEASE(m_PrevState);
}
