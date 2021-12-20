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
    // Rasterizer State는 ID3D11RasterizerState타입의 인터페이스를 사용
    // Blend State는 ID3D11BlendState타입의 인터페이스를 사용
    // DepthStencil State는 ID3D11DepthStencilState타입의 인터페이스를 사용
    // 이 3개의 State모두 ID3D11DeviceChild를 상속받으므로 m_State, m_PrevState를
    // ID3D11DeviceChild타입으로 선언해놓음
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

