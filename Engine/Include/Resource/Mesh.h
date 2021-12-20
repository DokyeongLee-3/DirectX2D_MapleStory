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
    Size->버퍼안에 데이터(Vertex or Index)하나의 크기
    Count -> 버퍼안에 데이터(Vertex or Index)의 개수
    Usage ->    D3D11_USAGE_DEFAULT	= 0,
                D3D11_USAGE_IMMUTABLE = 1,
                D3D11_USAGE_DYNAMIC = 2,
                D3D11_USAGE_STAGING = 3
    */
    bool CreateBuffer(Buffer_Type Type, void* Data, int Size, int Count,
        D3D11_USAGE Usage, ID3D11Buffer** Buffer);

    virtual void Render();
};

