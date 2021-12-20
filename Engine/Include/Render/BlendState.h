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
    // ���� Ÿ�ٸ��� ���� �ɼ��� �ٸ��� ������ �� �����Ƿ� ���ͷ� 
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
    // D3D11_BLEND_OP�� D3D11_BLEND_OP_ADD�� �ָ� 
    // Src�� r,g,b * Src�� Blend Factor + Dest�� r,g,b * Dest�� Blend Factor �̷����� ������� ������ �ϴµ�
    // D3D11_BLEND_SRC_ALPHA�� Src�� Blend Factor�� Src�� Alpha��
    // D3D11_BLEND_INV_SRC_ALPHA�� Dest�� Blend Factor�� 1.f - Src�� Alpha��
    // SrcBlendAlpha, DestBlendAlpha�� Src�� Dest�� Alpha�� ������ �����Ҷ� ��
    // Alpha = (SrcAlpha * SrcBlendAlpha) BlendOpAlpha (DestAlpha * DestBlendAlpha) �̷��� ������ ���İ��� �����Ǵµ�
    // ���� Src�� 1, Dest�� 0�� �ǰ��ؼ� Alpha���� Src������ ������� �Ѵ�
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

