#pragma once

#include "../../GameInfo.h"
#include "Material.h"

class CMaterialManager
{
	friend class CResourceManager;

private:
	CMaterialManager();
	~CMaterialManager();

private:
	std::unordered_map<std::string, CSharedPtr<CMaterial>>	m_mapMaterial;
	// Material의 BaseColor에 대한 상수버퍼
	class CMaterialConstantBuffer* m_CBuffer;

public:
	bool Init();
	CMaterial* FindMaterial(const std::string& Name);
	void ReleaseMaterial(const std::string& Name);

public:
	template <typename T>
	bool CreateMaterial(const std::string& Name)
	{
		T* Material = (T*)FindMaterial(Name);

		if (Material)
			return false;

		Material = new T;

		// 각 Material들이 MatarialManager가 갖고 있는 m_CBuffer 포인터를 갖고있게 한다
		// Material 복사생성자에서도 얕은 복사로 포인터를 복사해서 갖고있는다
		Material->SetConstantBuffer(m_CBuffer);

		Material->SetName(Name);

		m_mapMaterial.insert(std::make_pair(Name, Material));

		return true;
	}

	template <typename T>
	T* CreateMaterialEmpty()
	{
		T* Material = new T;

		Material->SetConstantBuffer(m_CBuffer);

		return Material;
	}
};

