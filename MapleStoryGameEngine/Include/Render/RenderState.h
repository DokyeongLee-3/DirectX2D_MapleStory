#pragma once

#include "../Ref.h"

class CRenderState :
    public CRef
{
    friend class CRenderStateManager;

protected:
    CRenderState();
    virtual ~CRenderState() = 0;

protected:
    // Rasterizer State�� ID3D11RasterizerStateŸ���� �������̽��� ���
    // Blend State�� ID3D11BlendStateŸ���� �������̽��� ���
    // DepthStencil State�� ID3D11DepthStencilStateŸ���� �������̽��� ���
    // �� 3���� State��� ID3D11DeviceChild�� ��ӹ����Ƿ� m_State, m_PrevState��
    // ID3D11DeviceChildŸ������ �����س���
    ID3D11DeviceChild* m_State;
    ID3D11DeviceChild* m_PrevState;
    RenderState_Type    m_Type;

public:
    RenderState_Type GetType()  const
    {
        return m_Type;
    }

public:
    virtual void SetState() = 0;
    virtual void ResetState() = 0;
};

