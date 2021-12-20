#pragma once

#include "../Ref.h"

class CMesh :
    public CRef
{
    friend class CResourceManager;

protected:
    CMesh();
    virtual ~CMesh() = 0;

protected:
    std::vector<MeshContainer> m_vecContainer;

    Vector3 m_Min;
    Vector3 m_Max;

public:
    virtual bool Init();

protected:
    /*
    Size->���۾ȿ� ������(Vertex or Index)�ϳ��� ũ��
    Count -> ���۾ȿ� ������(Vertex or Index)�� ����
    Usage ->    D3D11_USAGE_DEFAULT	= 0,
                D3D11_USAGE_IMMUTABLE = 1,
                D3D11_USAGE_DYNAMIC = 2,
                D3D11_USAGE_STAGING = 3
    */
    bool CreateBuffer(Buffer_Type Type, void* Data, int Size, int Count,
        D3D11_USAGE Usage, ID3D11Buffer** Buffer);

    virtual void Render();
};

