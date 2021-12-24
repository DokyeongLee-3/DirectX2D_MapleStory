#pragma once
#include "Mesh.h"
class CColorMesh :
    public CMesh
{
	friend class CMeshManager;

protected:
	CColorMesh();
	virtual ~CColorMesh();

public:
	virtual bool Init();
};

