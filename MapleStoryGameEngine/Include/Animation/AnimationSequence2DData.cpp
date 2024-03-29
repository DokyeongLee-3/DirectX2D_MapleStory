
#include "AnimationSequence2DData.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CAnimationSequence2DData::CAnimationSequence2DData() :
	m_Frame(0),
	m_Time(0.f),
	m_FrameTime(0.f),
	m_PlayTime(0.f),
	m_PlayScale(1.f),
	m_Loop(0),
	m_Reverse(0),
	m_Sequence(nullptr)
{
}

CAnimationSequence2DData::~CAnimationSequence2DData()
{
	auto	iter = m_vecNotify.begin();
	auto	iterEnd = m_vecNotify.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
}

void CAnimationSequence2DData::Save(FILE* File)
{
	int	Length = (int)m_Name.length();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(m_Name.c_str(), sizeof(char), Length, File);


	fwrite(&m_Frame, sizeof(int), 1, File);
	fwrite(&m_Time, sizeof(float), 1, File);
	fwrite(&m_FrameTime, sizeof(float), 1, File);
	fwrite(&m_PlayTime, sizeof(float), 1, File);
	fwrite(&m_PlayScale, sizeof(float), 1, File);

	fwrite(&m_Loop, sizeof(bool), 1, File);
	fwrite(&m_Reverse, sizeof(bool), 1, File);

	std::string	SequenceName = m_Sequence->GetName();

	Length = (int)SequenceName.length();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(SequenceName.c_str(), sizeof(char), Length, File);
}

void CAnimationSequence2DData::Load(FILE* File)
{
	int	Length = 0;
	char	Name[256] = {};

	fread(&Length, sizeof(int), 1, File);
	fread(Name, sizeof(char), Length, File);
	m_Name = Name;


	fread(&m_Frame, sizeof(int), 1, File);
	fread(&m_Time, sizeof(float), 1, File);
	fread(&m_FrameTime, sizeof(float), 1, File);
	fread(&m_PlayTime, sizeof(float), 1, File);
	fread(&m_PlayScale, sizeof(float), 1, File);

	fread(&m_Loop, sizeof(bool), 1, File);
	fread(&m_Reverse, sizeof(bool), 1, File);

	Length = 0;
	char	SequenceName[256] = {};
	fread(&Length, sizeof(int), 1, File);
	fread(SequenceName, sizeof(char), Length, File);

	m_SequenceName = SequenceName;
}

CAnimationSequence2D* CAnimationSequence2DData::GetAnimationSequence() const
{
	return m_Sequence;
}

const AnimationFrameData& CAnimationSequence2DData::GetFrameData(int Index) const
{
	return m_Sequence->GetFrameData(Index);
}

void CAnimationSequence2DData::SetCurrentAnimationFrame(int Frame)
{
	m_Frame = Frame;
}

void CAnimationSequence2DData::SetCurrentAnimationPlayTime(float Time)
{
	m_Time = Time;
}

void CAnimationSequence2DData::InitializeCurrentAnimation()
{
	if (m_Reverse)
		m_Frame = m_Sequence->GetFrameCount() - 1;

	else
		m_Frame = 0;

	m_Time = 0.f;
}

void CAnimationSequence2DData::SetCurrentAnimationFirstFrame()
{
	if (m_Reverse)
		m_Frame = m_Sequence->GetFrameCount() - 1;
	else
		m_Frame = 0;
}

