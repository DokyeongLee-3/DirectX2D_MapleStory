#pragma once

#include "SceneMode.h"
#include "SceneResource.h"
#include "SceneCollision.h"
#include "CameraManager.h"
#include "Viewport.h"
#include "../GameObject/GameObject.h"

class CScene
{
	friend class CSceneManager;

private:
	CScene();
	~CScene();

private:
	CSharedPtr<CSceneMode> m_Mode;
	CSceneResource* m_Resource;
	CSceneCollision* m_Collision;
	CCameraManager* m_CameraManager;
	CViewport* m_Viewport;

	std::list<CSharedPtr<CGameObject>>		m_ObjList;
	// 이펙트들을 저장해놓은 map 
	std::unordered_map<std::string, CSharedPtr<CGameObject>> m_mapPrototype;
	bool		m_Start;
	bool		m_Change;


public:
	void AddObject(CGameObject* Object)
	{
		m_ObjList.push_back(Object);
	}

	void SetAutoChange(bool Change)
	{
		m_Change = Change;
	}

public:
	CSceneMode*	GetSceneMode()	const
	{
		return m_Mode;
	}

	CSceneResource* GetResource()	const
	{
		return m_Resource;
	}

	CSceneCollision* GetCollision()	const
	{
		return m_Collision;
	}

	CCameraManager* GetCameraManager()	const
	{
		return m_CameraManager;
	}

	CViewport* GetViewport()	const
	{
		return m_Viewport;
	}

	CGameObject* GetPlayerObject()    const
	{
		return m_Mode->GetPlayerObject();
	}

	CGameObject* FindObject(const std::string& Name)
	{
		auto	iter = m_ObjList.begin();
		auto	iterEnd = m_ObjList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == Name)
				return *iter;
		}

		return nullptr;
	}

	// 인자로 받은 Name을 포함하고, 인자로 넘겨준 Pos와 Distance이내의 거리에 있는 GameObject를 찾아서 리턴
	CGameObject* FindIncludingNameObject(const std::string& Name, const Vector3& Pos, const Vector2& Distance)
	{
		auto	iter = m_ObjList.begin();
		auto	iterEnd = m_ObjList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName().find(Name) != std::string::npos && abs((*iter)->GetWorldPos().x - Pos.x) < Distance.x &&
				abs((*iter)->GetWorldPos().y - Pos.y) < Distance.y)
			{
				CSceneComponent* Body = (*iter)->FindComponentIncludingName("Body");

				if(Body && Body->IsEnable())
					return *iter;
			}
		}

		return nullptr;
	}

	size_t GetObjectCount()	const
	{
		return m_ObjList.size();
	}

	CGameObject* GetGameObject(int Index)	const
	{
		auto iter = m_ObjList.begin();

		std::advance(iter, Index);

		return *iter;
	}

public:
	void Start();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Save(const char* FileName, const std::string& PathName = SCENE_PATH);
	void SaveFullPath(const char* FullPath);
	void Load(const char* FileName, const std::string& PathName = SCENE_PATH);
	void LoadFullPath(const char* FullPath);

public:
	template<typename T>
	void FindObjectByType(std::vector<T*>& vecObj)
	{
		auto	iter = m_ObjList.begin();
		auto	iterEnd = m_ObjList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTypeID() == typeid(T).hash_code())
				vecObj.push_back((T*)((*iter).Get()));
		}
	}

public:
	template <typename T>
	bool CreateSceneMode()
	{
		m_Mode = new T;

		m_Mode->m_Scene = this;

		if (!m_Mode->Init())
		{
			m_Mode = nullptr;
			return false;
		}

		return true;
	}

	template <typename T>
	T* CreateSceneModeEmpty()
	{
		m_Mode = new T;

		m_Mode->m_Scene = this;

		return (T*)*m_Mode;
	}

	template <typename T>
	bool LoadSceneMode()
	{
		m_Mode = new T;

		m_Mode->m_Scene = this;

		return true;
	}

	template <typename T>
	T* CreateGameObject(const std::string& Name)
	{
		T* Obj = new T;

		Obj->SetName(Name);
		Obj->SetScene(this);

		if (!Obj->Init())
		{
			SAFE_RELEASE(Obj);
			return nullptr;
		}

		m_ObjList.push_back(Obj);

		if (m_Start)
			Obj->Start();

		return Obj;
	}

	template <typename T>
	T* LoadGameObject()
	{
		T* Obj = new T;

		CScene* NextScene = CSceneManager::GetInst()->GetNextScene();

		if (NextScene)
		{
			NextScene->AddObject(Obj);
			Obj->SetScene(NextScene);
		}

		else
		{
			m_ObjList.push_back(Obj);
			Obj->SetScene(this);
		}

		if (m_Start)
			Obj->Start();

		return Obj;
	}

	template <typename T>
	T* CreatePrototype(const std::string& Name)
	{
		T* Obj = new T;

		Obj->SetScene(this);
		Obj->SetName(Name);

		if (!Obj->Init())
		{
			SAFE_DELETE(Obj);
			return nullptr;
		}

		m_mapPrototype.insert(std::make_pair(Name, Obj));

		return Obj;
	}

	template <typename T>
	T* CloneFromPrototype(const std::string& Name, const std::string& PrototypeName,
		const Vector3& Pos = Vector3(0.f, 0.f, 0.f),
		const Vector3& Size = Vector3(100.f, 100.f, 0.f))
	{
		CGameObject* Prototype = FindPrototype(PrototypeName);

		if (!Prototype)
			return nullptr;

		T* Obj = (T*)Prototype->Clone();

		if (m_Start)
			Obj->Start();

		Obj->SetScene(this);
		Obj->SetName(Name);
		Obj->SetWorldPos(Pos);

		m_ObjList.push_back(Obj);

		return Obj;
	}

private:
	CGameObject* FindPrototype(const std::string& Name);
};

