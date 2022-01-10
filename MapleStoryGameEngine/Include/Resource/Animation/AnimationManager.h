#pragma once

#include "AnimationSequence2D.h"

class CAnimationManager
{
	friend class CResourceManager;

private:
	CAnimationManager();
	~CAnimationManager();

private:
	// CAnimationSequence2D는 AnimationManager에 넣어놓고
	// 각각의 Instance들이 공유해서 사용
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>>	m_mapSequence2D;
	class CAnimation2DConstantBuffer* m_Animation2DCBuffer;

public:
	class CAnimation2DConstantBuffer* GetAnimation2DCBuffer()	const
	{
		return m_Animation2DCBuffer;
	}

	void InsertAnimationSequence2D(CAnimationSequence2D* Sequence);

public:
	bool Init();
	bool CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName,
		const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& Name, class CTexture* Texture);
	void EraseAnimationSequence2D(const std::string& Name);
	void AddFrame(const std::string& Name, const Vector2& Start, const Vector2& Size);
	void AddFrame(const std::string& Name, float StartX, float StartY, float Width, float Height);
	bool SaveSequenceFullPath(const std::string& Name, const char* FullPath);
	bool LoadSequenceFullPath(std::string& resultName, const char* FullPath, class CScene* Scene = nullptr);
	bool SaveSequence(const std::string& Name, const char* FileName, const std::string& PathName);
	bool LoadSequence(std::string& resultName, const char* FileName, const std::string& PathName, class CScene* Scene = nullptr);

	CAnimationSequence2D* FindSequence(const std::string& Name);
	void ReleaseSequence(const std::string& Name);

};

