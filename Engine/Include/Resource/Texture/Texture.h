#pragma once

#include "../../Ref.h"

struct TextureResourceInfo
{
	// 픽셀 정보를 담고 있음
	ScratchImage* Image;
	// 로드한 텍스쳐의 픽셀 정보를 가지고 만든다
	// 이걸 픽셀 셰이더에 ID3D11ShaderResourceView를 넘겨서 
	// 픽셀 셰이더에서 텍스쳐에서 픽셀 정보를 뽑아서 메시에 입혀서 쓸 수 있도록 한다
	ID3D11ShaderResourceView* SRV;
	unsigned int	Width;
	unsigned int	Height;
	TCHAR* FileName;
	char* PathName;
	TCHAR* FullPath;

	TextureResourceInfo() :
		Image(nullptr),
		SRV(nullptr),
		FileName(nullptr),
		PathName(nullptr),
		FullPath(nullptr),
		Width(0),
		Height(0)
	{
	}

	~TextureResourceInfo()
	{
		SAFE_RELEASE(SRV);
		SAFE_DELETE_ARRAY(FileName);
		SAFE_DELETE_ARRAY(PathName);
		SAFE_DELETE_ARRAY(FullPath);
		SAFE_DELETE(Image);
	}
};

// 나중에 렌더타겟 만들때 렌더타겟이
// 이 Texture클래스를 상속받아서 만들어질것임
// 따라서 소멸자를 virtual로
class CTexture :
	public CRef
{
	friend class CTextureManager;

protected:
	CTexture();
	virtual ~CTexture();

protected:
	class CScene* m_Scene;
	// CTexture 클래스가 여러개의 텍스쳐를 들고 있을 수 있다
	std::vector<TextureResourceInfo*>	m_vecTextureInfo;
	Image_Type	m_ImageType;

public:
	ID3D11ShaderResourceView* GetResource(int Index = 0)	const
	{
		return m_vecTextureInfo[Index]->SRV;
	}

	Image_Type GetImageType()	const
	{
		return m_ImageType;
	}

	unsigned int GetWidth(int Index = 0)	const
	{
		return m_vecTextureInfo[Index]->Width;
	}

	unsigned int GetHeight(int Index = 0)	const
	{
		return m_vecTextureInfo[Index]->Height;
	}

	size_t GetImageCount()	const
	{
		return m_vecTextureInfo.size();
	}


public:
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const std::vector<TCHAR*>& vecFullPath);

private:
	// 텍스쳐에 대한 리소스뷰를 생성
	bool CreateResource(int Index);

public:
	// Register는 ShaderInfo.fx에서 볼수 있듯이 Texture2D g_BaseTexture : register(t0); 이렇게 선언해놨듯이
	// Texture용 레지스터 0번을 사용하도록 해놓았기 때문에 Array타입이 아니면 0번을 쓴다
	void SetShader(int Register, int ShaderType, int Index);
	void ResetShader(int Register, int ShaderType, int Index);
	void Save(FILE* pFile);
};

