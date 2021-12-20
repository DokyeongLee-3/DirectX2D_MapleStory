#pragma once

#include "RenderState.h"

class CBlendState :
    public CRenderState
{
    friend class CRenderStateManager;

protected:
    CBlendState();
    virtual ~CBlendState();

protected:
    // 렌더 타겟마다 블렌드 옵션을 다르게 지정할 수 있으므로 벡터로 
    std::vector<D3D11_RENDER_TARGET_BLEND_DESC> m_vecDesc;
    unsigned int    m_SampleMask;
    unsigned int    m_PrevSampleMask;
    float           m_BlendFactor[4];
    float           m_PrevBlendFactor[4];

public:
    void SetBlendFactor(float r, float g, float b, float a)
    {
        m_BlendFactor[0] = a;
        m_BlendFactor[1] = r;
        m_BlendFactor[2] = g;
        m_BlendFactor[3] = b;
    }

public:
    // D3D11_BLEND_OP로 D3D11_BLEND_OP_ADD를 주면 
    // Src의 r,g,b * Src의 Blend Factor + Dest의 r,g,b * Dest의 Blend Factor 이런식의 계산으로 블렌딩을 하는데
    // D3D11_BLEND_SRC_ALPHA는 Src의 Blend Factor로 Src의 Alpha값
    // D3D11_BLEND_INV_SRC_ALPHA는 Dest의 Blend Factor로 1.f - Src의 Alpha값
    // SrcBlendAlpha, DestBlendAlpha는 Src와 Dest의 Alpha값 끼리의 블렌딩할때 값
    // Alpha = (SrcAlpha * SrcBlendAlpha) BlendOpAlpha (DestAlpha * DestBlendAlpha) 이렇게 블렌딩후 알파값이 결정되는데
    // 보통 Src가 1, Dest가 0이 되게해서 Alpha값은 Src것으로 덮어쓰도록 한다
    void AddBlendInfo(bool BlendEnable = true, D3D11_BLEND SrcBlend = D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND DestBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD,
        D3D11_BLEND SrcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND DestBlendAlpha = D3D11_BLEND_ZERO,
        D3D11_BLEND_OP BlendOpAlpha = D3D11_BLEND_OP_ADD,
        UINT8 RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
    bool CreateState(bool AlphaToCoverageEnable, bool IndependentBlendEnable);

public:
    virtual void SetState();
    virtual void ResetState();
};

