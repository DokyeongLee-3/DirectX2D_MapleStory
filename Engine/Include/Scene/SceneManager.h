#pragma once

#include "Scene.h"

class CSceneManager
{
private:
	CScene* m_Scene;
	CScene* m_NextScene;
	std::function<void(CScene*, size_t)>	m_CreateSceneModeCallback;
	std::function<void(CScene*, size_t)>	m_CreateObjectCallback;

public:
	CScene* GetScene()	const
	{
		return m_Scene;
	}

public:
	void Start();
	bool Init();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);

public:
	template <typename T>
	bool CreateSceneMode(bool Current = true)
	{
		if (Current)
			return m_Scene->CreateSceneMode<T>();

		return m_NextScene->CreateSceneMode<T>();
	}

	template <typename T>
	void SetCreateSceneModeFunction(T* Obj, void(T::* Func)(CScene*, size_t))
	{
		m_CreateSceneModeCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}

	template <typename T>
	void SetCreateObjectFunction(T* Obj, void(T::* Func)(CScene*, size_t))
	{
		m_CreateObjectCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}

	DECLARE_SINGLE(CSceneManager)
};

