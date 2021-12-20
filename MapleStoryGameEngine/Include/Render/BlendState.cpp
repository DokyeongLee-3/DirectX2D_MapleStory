
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
	// ��Ƽ���ø����� ����Ÿ���� �ȼ����� �����Ҷ� ���İ��� ����Ұ�����
	// ����� ��Ƽ ���ø��� ��ģ �̹����� �ε巴�� ���ִ� ���
	// �̷� ����� ��Ƽ �˸����(Anti - Aliasing) �̶�� �θ��µ�
	// ��ǥ������ ���� ���ø��̶� ��Ƽ ���ø� ����� �ִ�. �̷� �������
	// �������(�ȼ��� ���ó�� ��Ÿ���� ����)�� ��ȭ �ϱ� ���� ����Ѵ�
	// ���� : https://lipcoder.tistory.com/entry/DirectX-12-%EB%8B%A4%EC%A4%91%ED%91%9C%EB%B3%B8%ED%99%94Multisampling
	Desc.AlphaToCoverageEnable = AlphaToCoverageEnable;
	// IndependentBlendEnable = true�� 8���� ����Ÿ�ٿ� ���������� ����
	// IndependentBlendEnable = false�� ���� ����Ÿ�ٿ� �����ϴ°�����
	// ������ ����Ÿ�ٵ� �����ϰ� �����Ѵٴ� �ǹ�
	Desc.IndependentBlendEnable = IndependentBlendEnable;

	memcpy(Desc.RenderTarget, &m_vecDesc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * m_vecDesc.size());
	//m_State�� ID3D11BlendState*Ÿ���� State�� �����ش�
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBlendState(&Desc, (ID3D11BlendState**)&m_State)))
		return false;

	return true;
}

void CBlendState::SetState()
{
	// OMGetBlendState�� reference count�� ���� MSDN����
	// The reference count of the returned interface will be incremented by one
	// when the blend state is retrieved. Applications must release returned pointer(s) 
	// when they are no longer needed, or else there will be a memory leak.
	CDevice::GetInst()->GetContext()->OMGetBlendState((ID3D11BlendState**)&m_PrevState, m_PrevBlendFactor, &m_PrevSampleMask);

	// ���� �־��ִ� m_BlendFactor�� ���� ID3D11_BLENDŸ���� �־���ϴ°���
	// D3D11_BLEND_BLEND_FACTOR�� D3D11_BLEND_INV_BLEND_FACTOR�� �־��� ������
	// ���� ���ڷ� �־��� m_BlendFactor�� ����ؼ� ����Ѵ�
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)m_State, m_BlendFactor, m_SampleMask);
}

void CBlendState::ResetState()
{
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)m_PrevState, m_PrevBlendFactor, m_PrevSampleMask);

	SAFE_RELEASE(m_PrevState);
}
