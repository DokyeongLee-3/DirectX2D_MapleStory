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
	std::list<Node_Dir>	SearchDirList;

	NavNode() :
		Parent(nullptr),
		NodeType(Nav_Node_Type::None),
		TileType(Tile_Type::Normal),
		IndexX(-1),
		IndexY(-1),
		Index(-1),
		Cost(FLT_MAX),
		Dist(FLT_MAX),
		Total(FLT_MAX)
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
	// 이전 길찾기에서 열린목록에 들어가서 Cost나 Dist같은 값들이 설정되어 있을 것인데
	// 다음 길찾기에선 그 값들을 싹다 초기화되어 있어야 하므로 열린목록에 노드를 넣을때 m_vecUseNode에도 넣어주고
	// 다음 길찾기 시작할때 m_vecUseNode에 노드들의 여러 값들(ex. Cost, Dist)을 초기화해준다
	std::vector<NavNode*>	m_vecUseNode;

public:
	void CreateNavigationNode(class CTileMapComponent* TileMap);
	bool FindPath(const Vector3& Start, const Vector3& End, std::list<Vector3>& vecPath);

private:
	bool FindNode(NavNode* Node, NavNode* EndNode, const Vector3& End, std::list<Vector3>& vecPath);
	NavNode* GetCorner(Node_Dir Dir, NavNode* Node, NavNode* EndNode, const Vector3& End);

	// 위방향으로 한칸 이동하고 보니 오른쪽이나 왼쪽으로 코너 돌 수 있는 노드면 그 노드 리턴
	NavNode* GetRectNodeTop(NavNode* Node, NavNode* EndNode, const Vector3& End, bool Digonal = true);
	// RightTop 대각선 방향으로 한칸 이동하고 보니 거기가 코너돌 수 있는 노드면 그 노드 리턴
	NavNode* GetRectNodeRightTop(NavNode* Node, NavNode* EndNode, const Vector3& End);
	// 오른쪽으로 한칸 이동하고 보니 오른쪽이나 왼쪽으로 코너 돌 수 있는 노드면 그 노드 리턴
	NavNode* GetRectNodeRight(NavNode* Node, NavNode* EndNode, const Vector3& End, bool Digonal = true);
	// 
	NavNode* GetRectNodeRightBottom(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRectNodeBottom(NavNode* Node, NavNode* EndNode, const Vector3& End, bool Digonal = true);
	NavNode* GetRectNodeLeftBottom(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRectNodeLeft(NavNode* Node, NavNode* EndNode, const Vector3& End, bool Digonal = true);
	NavNode* GetRectNodeLeftTop(NavNode* Node, NavNode* EndNode, const Vector3& End);

	NavNode* GetRhombusNodeTop(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeRightTop(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeRight(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeRightBottom(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeBottom(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeLeftBottom(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeLeft(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeLeftTop(NavNode* Node, NavNode* EndNode, const Vector3& End);

	void AddDir(Node_Dir Dir, NavNode* Node);

private:
	static bool SortNode(NavNode* Src, NavNode* Dest);
};

