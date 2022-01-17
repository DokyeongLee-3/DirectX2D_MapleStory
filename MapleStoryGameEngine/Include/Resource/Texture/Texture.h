#pragma once

#include "../../Ref.h"

struct TextureResourceInfo
{
	// �ȼ� ������ ��� ����
	ScratchImage* Image;
	// �ε��� �ؽ����� �ȼ� ������ ������ �����
	// �̰� �ȼ� ���̴��� ID3D11ShaderResourceView�� �Ѱܼ� 
	// �ȼ� ���̴����� �ؽ��Ŀ��� �ȼ� ������ �̾Ƽ� �޽ÿ� ������ �� �� �ֵ��� �Ѵ�
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

// ���߿� ����Ÿ�� ���鶧 ����Ÿ����
// �� TextureŬ������ ��ӹ޾Ƽ� �����������
// ���� �Ҹ��ڸ� virtual��
class CTexture :
	public CRef
{
	friend class CTextureManager;

protected:
	CTexture();
	virtual ~CTexture();

protected:
	class CScene* m_Scene;
	// CTexture Ŭ������ �������� �ؽ��ĸ� ��� ���� �� �ִ�
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
	// �ؽ��Ŀ� ���� ���ҽ��並 ����
	bool CreateResource(int Index);

public:
	// Register�� ShaderInfo.fx���� ���� �ֵ��� Texture2D g_BaseTexture : register(t0); �̷��� �����س�����
	// Texture�� �������� 0���� ����ϵ��� �س��ұ� ������ ArrayŸ���� �ƴϸ� 0���� ����
	void SetShader(int Register, int ShaderType, int Index);
	void ResetShader(int Register, int ShaderType, int Index);
	void Save(FILE* pFile);
};

