#pragma once

#include "../Component/TileMapComponent.h"

enum class Node_Dir
{
	T,
	RT,
	R,
	RB,
	B,
	LB,
	L,
	LT,
	End
};

struct NavNode
{
	NavNode* Parent;
	Nav_Node_Type	NodeType;
	Tile_Type	TileType;
	Vector3		Pos;			// 좌 하단 기준.
	Vector3		Size;
	Vector3		Center;
	int			IndexX;
	int			IndexY;
	int			Index;
	float		Cost;
	float		Dist;
	float		Total;

	NavNode() :
		Parent(nullptr),
		NodeType(Nav_Node_Type::None),
		TileType(Tile_Type::Normal),
		IndexX(-1),
		IndexY(-1),
		Index(-1),
		Cost(0.f),
		Dist(0.f),
		Total(0.f)
	{
	}
};

class CNavigation
{
	friend class CNavigationThread;

private:
	CNavigation();
	~CNavigation();

private:
	Tile_Shape	m_NodeShape;
	std::vector<NavNode*>	m_vecNode;
	int     m_CountX;
	int     m_CountY;
	Vector3 m_TileSize;
	CSharedPtr<CTileMapComponent>	m_TileMap;
	std::vector<NavNode*>	m_vecOpen;
	std::vector<NavNode*>	m_vecUseNode;

public:
	void CreateNavigationNode(class CTileMapComponent* TileMap);
	bool FindPath(const Vector3& Start, const Vector3& End, std::vector<Vector3>& vecPath);

private:
	void FindNode(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);
	NavNode* GetCorner(Node_Dir Dir, NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);

	// 위방향으로 한칸 이동하고 보니 오른쪽이나 왼쪽으로 코너 돌 수 있는 노드면 그 노드 리턴
	NavNode* GetRectNodeTop(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath, bool Digonal = true);
	// RightTop 대각선 방향으로 한칸 이동하고 보니 거기가 코너돌 수 있는 노드면 그 노드 리턴
	NavNode* GetRectNodeRightTop(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);
	// 오른쪽으로 한칸 이동하고 보니 오른쪽이나 왼쪽으로 코너 돌 수 있는 노드면 그 노드 리턴
	NavNode* GetRectNodeRight(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath, bool Digonal = true);
	// 
	NavNode* GetRectNodeRightBottom(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);
	NavNode* GetRectNodeBottom(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath, bool Digonal = true);
	NavNode* GetRectNodeLeftBottom(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);
	NavNode* GetRectNodeLeft(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath, bool Digonal = true);
	NavNode* GetRectNodeLeftTop(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);

	NavNode* GetRhombusNodeTop(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);
	NavNode* GetRhombusNodeRightTop(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);
	NavNode* GetRhombusNodeRight(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);
	NavNode* GetRhombusNodeRightBottom(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);
	NavNode* GetRhombusNodeBottom(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);
	NavNode* GetRhombusNodeLeftBottom(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);
	NavNode* GetRhombusNodeLeft(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);
	NavNode* GetRhombusNodeLeftTop(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath);

private:
	static bool SortNode(NavNode* Src, NavNode* Dest);
};

