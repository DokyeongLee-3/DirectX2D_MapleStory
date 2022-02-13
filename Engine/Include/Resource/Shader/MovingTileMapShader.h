#pragma once
#include "GraphicShader.h"
class CMovingTileMapShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CMovingTileMapShader();
    virtual ~CMovingTileMapShader();

public:
    virtual bool Init();
};

