
#include "ClientManager.h"
#include "Engine.h"
#include "resource.h"
#include "Scene/SceneManager.h"
#include "Scene/LobbyScene.h"
#include "Scene/OnionScene.h"
#include "Scene/WayToZakumScene.h"
#include "Scene/Library2ndScene.h"
#include "Scene/StartScene.h"
#include "Scene/RadishScene.h"
#include "Scene/ZakumAltarScene.h"
#include "Client.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/DragCollider.h"
#include "Component/StaticMeshComponent.h"
#include "Component/TileMapComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/OnionMonsterAnimation.h"
#include "Animation/LowerClassBookAnimation.h"
#include "Animation/RadishMonsterAnimation.h"
#include "Animation/ZakumBodyAnimation.h"
#include "Animation/ZakumHandAnimation.h"
#include "Input.h"
#include "Widget/MouseNormal.h"
#include "Widget/MouseClick.h"
#include "Widget/ToolTip.h"
#include "PathManager.h"

#include <istream>
#include <fstream>
#include <sstream>

DEFINITION_SINGLE(CClientManager)

CClientManager::CClientManager()
{
}

CClientManager::~CClientManager()
{
	CEngine::DestroyInst();
}

bool CClientManager::Init(HINSTANCE hInst)
{
	if (!CEngine::GetInst()->Init(hInst, TEXT("MapleStory"),
		1280, 720, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}

	CEngine::GetInst()->EnableClientMode(true);

	CSceneManager::GetInst()->SetCreateSceneModeFunction<CClientManager>(this, &CClientManager::CreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectFunction<CClientManager>(this, &CClientManager::CreateObject);
	CSceneManager::GetInst()->SetCreateComponentFunction<CClientManager>(this, &CClientManager::CreateComponent);
	CSceneManager::GetInst()->SetCreateAnimInstanceFunction<CClientManager>(this, &CClientManager::CreateAnimInstance);

	CInput::GetInst()->CreateKey("Inventory", 'I');
	CInput::GetInst()->CreateKey("Configuration", 'C');	
	CInput::GetInst()->CreateKey("BossMatching", 'B');
	CInput::GetInst()->CreateKey("Stat", 'S');
	CInput::GetInst()->CreateKey("TurnOffUIWindow", VK_ESCAPE);

	CInput::GetInst()->SetKeyCallback<CClientManager>("Inventory", KeyState_Down, this, &CClientManager::OnOffInventory);
	CInput::GetInst()->SetKeyCallback<CClientManager>("Configuration", KeyState_Down, this, &CClientManager::OnOffConfiguration);
	CInput::GetInst()->SetKeyCallback<CClientManager>("BossMatching", KeyState_Down, this, &CClientManager::OnOffBossMatching);
	CInput::GetInst()->SetKeyCallback<CClientManager>("Stat", KeyState_Down, this, &CClientManager::OnOffStatWindow);
	CInput::GetInst()->SetKeyCallback<CClientManager>("TurnOffUIWindow", Key_State::KeyState_Down, this, &CClientManager::TurnOffWindow);

	//CInput::GetInst()->CreateKey("MovePoint", VK_RBUTTON);







	CResourceManager::GetInst()->CreateSoundChannelGroup("UI");
	CResourceManager::GetInst()->CreateSoundChannelGroup("BGM");
	CResourceManager::GetInst()->CreateSoundChannelGroup("Effect");

	CResourceManager::GetInst()->SetVolume("BGM", 70);

	// 마우스 위젯 설정
	CMouseNormal* MouseNormal = CEngine::GetInst()->CreateMouse<CMouseNormal>(Mouse_State::Normal, "MouseNormal");
	CMouseClick* MouseClick = CEngine::GetInst()->CreateMouse<CMouseClick>(Mouse_State::Click, "MouseClick");


	const PathInfo* Info = CPathManager::GetInst()->FindPath("MonsterInfo");

	TCHAR FullPath[MAX_PATH] = {};
	wcscpy_s(FullPath, Info->Path);
	wcscat_s(FullPath, TEXT("MonsterInfo.csv"));

	std::ifstream file(FullPath);

	if (file.fail())
		return true;

	std::string InfoStr;

	int Count = 0;
	while (std::getline(file, InfoStr))
	{
		++Count;

		if (Count == 1)
			continue;

		std::string Delimiter = ",";
		size_t FirstPos = 0;

		std::stringstream sstream(InfoStr);
		std::string SubStr;
		std::vector<std::string>	vecStr;

		while(std::getline(sstream, SubStr, ','))
		{
			vecStr.push_back(SubStr);
		}

		MonsterInfo MInfo = {};

		if (vecStr[0] != "Zakum")
		{
			MInfo.Name = vecStr[0];

			std::stringstream ss;

			ss << vecStr[1];
			ss >> MInfo.Level;

			ss = std::stringstream();

			ss << vecStr[2];
			ss >> MInfo.HPMax;

			ss = std::stringstream();

			ss << vecStr[3];
			ss >> MInfo.HP;

			ss = std::stringstream();

			ss << vecStr[4];
			ss >> MInfo.Attack;

			m_vecMonsterInfo.push_back(MInfo);
		}

		else
		{
			ZakumInfo ZInfo = {};

			std::stringstream ss;

			ss << vecStr[1];
			ss >> ZInfo.Level;

			ss = std::stringstream();

			ss << vecStr[2];
			ss >> ZInfo.BodyHPMax;

			ss = std::stringstream();

			ss << vecStr[3];
			ss >> ZInfo.BodyHP;

			ss = std::stringstream();

			ss << vecStr[5];
			ss >> ZInfo.LeftArm1HP;
			ss = std::stringstream();

			ss << vecStr[6];
			ss >> ZInfo.LeftArm2HP;
			ss = std::stringstream();

			ss << vecStr[7];
			ss >> ZInfo.LeftArm3HP;
			ss = std::stringstream();

			ss << vecStr[8];
			ss >> ZInfo.LeftArm4HP;
			ss = std::stringstream();

			ss << vecStr[9];
			ss >> ZInfo.RightArm1HP;
			ss = std::stringstream();

			ss << vecStr[10];
			ss >> ZInfo.RightArm2HP;
			ss = std::stringstream();

			ss << vecStr[11];
			ss >> ZInfo.RightArm3HP;
			ss = std::stringstream();

			ss << vecStr[12];
			ss >> ZInfo.RightArm4HP;
			ss = std::stringstream();

			ss << vecStr[13];
			ss >> ZInfo.ClapAttack;
			ss = std::stringstream();

			ss << vecStr[14];
			ss >> ZInfo.SmashAttack;
			ss = std::stringstream();

			ss << vecStr[15];
			ss >> ZInfo.FireAttack;

			m_ZakumInfo = ZInfo;
		}

	}


	file.close(); //파일 입출력 완료 후 닫아준다.

	return true;
}

void CClientManager::CreateStartSceneMode()
{
	CSceneManager::GetInst()->CreateSceneMode<CStartScene>();
}

int CClientManager::Run()
{
	return CEngine::GetInst()->Run();
}

void CClientManager::CreateSceneMode(CScene* Scene, size_t Type)
{
	//if (Type == typeid(CMainScene).hash_code())
	//{
	//	if (!(Scene->GetSceneMode()))
	//		Scene->LoadSceneMode<CMainScene>();
	//}
}

CGameObject* CClientManager::CreateObject(CScene* Scene, size_t Type)
{
	if (Type == typeid(CGameObject).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CGameObject>();

		return Obj;
	}

	//else if (Type == typeid(CPlayer2D).hash_code())
	//{
	//	CPlayer2D* Obj = Scene->LoadGameObject<CPlayer2D>();

	//	return Obj;
	//}

	else if (Type == typeid(CRadishMonster).hash_code())
	{
		CRadishMonster* Obj = Scene->LoadGameObject<CRadishMonster>();

		return Obj;
	}

	else if (Type == typeid(COnionMonster).hash_code())
	{
		COnionMonster* Obj = Scene->LoadGameObject<COnionMonster>();

		return Obj;
	}

	else if (Type == typeid(CLobbyBigLamp).hash_code())
	{
		CLobbyBigLamp* Obj = Scene->LoadGameObject<CLobbyBigLamp>();

		return Obj;
	}

	else if (Type == typeid(CLobbySmallLamp).hash_code())
	{
		CLobbySmallLamp* Obj = Scene->LoadGameObject<CLobbySmallLamp>();

		return Obj;
	}


	else if (Type == typeid(CLampLight).hash_code())
	{
		CLampLight* Obj = Scene->LoadGameObject<CLampLight>();

		return Obj;
	}

	else if (Type == typeid(CBlinkTree).hash_code())
	{
		CBlinkTree* Obj = Scene->LoadGameObject<CBlinkTree>();

		return Obj;
	}

	else if (Type == typeid(CButterfly).hash_code())
	{
		CButterfly* Obj = Scene->LoadGameObject<CButterfly>();

		return Obj;
	}

	else if (Type == typeid(CDoubleHelixBlinkTree).hash_code())
	{
		CDoubleHelixBlinkTree* Obj = Scene->LoadGameObject<CDoubleHelixBlinkTree>();

		return Obj;
	}

	else if (Type == typeid(CLowerClassBook).hash_code())
	{
		CLowerClassBook* Obj = Scene->LoadGameObject<CLowerClassBook>();

		return Obj;
	}

	else if (Type == typeid(CSingleHelixBlinkTree).hash_code())
	{
		CSingleHelixBlinkTree* Obj = Scene->LoadGameObject<CSingleHelixBlinkTree>();

		return Obj;
	}

	//else if (Type == typeid(CLibrary2ndLampLight).hash_code())
	//{
	//	CLibrary2ndLampLight* Obj = Scene->LoadGameObject<CLibrary2ndLampLight>();

	//	return Obj;
	//}

	//else if (Type == typeid(CLibrary2ndButterfly).hash_code())
	//{
	//	CLibrary2ndButterfly* Obj = Scene->LoadGameObject<CLibrary2ndButterfly>();

	//	return Obj;
	//}

	else if (Type == typeid(CStaticMapObj).hash_code())
	{
		CStaticMapObj* Obj = Scene->LoadGameObject<CStaticMapObj>();

		return Obj;
	}

	else if (Type == typeid(CTileObject).hash_code())
	{
		CTileObject* Obj = Scene->LoadGameObject<CTileObject>();

		return Obj;
	}

	else if (Type == typeid(CLopeTileObject).hash_code())
	{
		CLopeTileObject* Obj = Scene->LoadGameObject<CLopeTileObject>();

		return Obj;
	}

	else if (Type == typeid(CPortal).hash_code())
	{
		CPortal* Obj = Scene->LoadGameObject<CPortal>();

		return Obj;
	}

	else if (Type == typeid(CVerticalLampLight).hash_code())
	{
		CVerticalLampLight* Obj = Scene->LoadGameObject<CVerticalLampLight>();

		return Obj;
	}

	else if (Type == typeid(CStage).hash_code())
	{
		CStage* Obj = Scene->LoadGameObject<CStage>();

		CScene* NextScene = CSceneManager::GetInst()->GetNextScene();

		if (NextScene)
		{
			CSceneMode* NextSceneMode = NextScene->GetSceneMode();
			if (NextSceneMode)
			{
				if (NextSceneMode->GetTypeID() == typeid(CLobbyScene).hash_code())
				{
					((CLobbyScene*)(NextSceneMode))->SetStageObject(Obj);
				}

				else if (NextSceneMode->GetTypeID() == typeid(COnionScene).hash_code())
				{
					((COnionScene*)(NextSceneMode))->SetStageObject(Obj);
				}

				else if (NextSceneMode->GetTypeID() == typeid(CWayToZakumScene).hash_code())
				{
					((CWayToZakumScene*)(NextSceneMode))->SetStageObject(Obj);
				}

				else if (NextSceneMode->GetTypeID() == typeid(CLibrary2ndScene).hash_code())
				{
					((CLibrary2ndScene*)(NextSceneMode))->SetStageObject(Obj);
				}

				else if (NextSceneMode->GetTypeID() == typeid(CRadishScene).hash_code())
				{
					((CRadishScene*)(NextSceneMode))->SetStageObject(Obj);
				}

				else if (NextSceneMode->GetTypeID() == typeid(CZakumAltarScene).hash_code())
				{
					((CZakumAltarScene*)(NextSceneMode))->SetStageObject(Obj);
				}
			}
		}

		return Obj;
	}

	else if (Type == typeid(CNPCAdobis).hash_code())
	{
		CNPCAdobis* Obj = Scene->LoadGameObject<CNPCAdobis>();

		return Obj;
	}

	else if (Type == typeid(CZakumBody).hash_code())
	{
		CZakumBody* Obj = Scene->LoadGameObject<CZakumBody>();

		return Obj;
	}

	else if (Type == typeid(CNPCAmon).hash_code())
	{
		CNPCAmon* Obj = Scene->LoadGameObject<CNPCAmon>();

		return Obj;
	}

	return nullptr;
}

CComponent* CClientManager::CreateComponent(CGameObject* Obj, size_t Type)
{
	if (Type == typeid(CSceneComponent).hash_code())
	{
		CSceneComponent* Component = Obj->LoadComponent<CSceneComponent>();

		return Component;
	}

	else if (Type == typeid(CSpriteComponent).hash_code())
	{
		CSpriteComponent* Component = Obj->LoadComponent<CSpriteComponent>();

		return Component;
	}

	else if (Type == typeid(CStaticMeshComponent).hash_code())
	{
		CStaticMeshComponent* Component = Obj->LoadComponent<CStaticMeshComponent>();

		return Component;
	}

	else if (Type == typeid(CColliderBox2D).hash_code())
	{
		CColliderBox2D* Component = Obj->LoadComponent<CColliderBox2D>();

		CSceneManager::GetInst()->GetScene()->GetCollision()->AddCollider(Component);

		return Component;
	}

	else if (Type == typeid(CColliderCircle).hash_code())
	{
		CColliderCircle* Component = Obj->LoadComponent<CColliderCircle>();

		CSceneManager::GetInst()->GetScene()->GetCollision()->AddCollider(Component);

		return Component;
	}

	else if (Type == typeid(CColliderPixel).hash_code())
	{
		CColliderPixel* Component = Obj->LoadComponent<CColliderPixel>();

		return Component;
	}

	else if (Type == typeid(CDragCollider).hash_code())
	{
		CDragCollider* Component = Obj->LoadComponent<CDragCollider>();

		return Component;
	}

	else if (Type == typeid(CTileMapComponent).hash_code())
	{
		CTileMapComponent* Component = Obj->LoadComponent<CTileMapComponent>();

		return Component;
	}

	return nullptr;
}

void CClientManager::CreateAnimInstance(CSpriteComponent* Sprite, size_t Type)
{

	if (Type == typeid(CAnimationSequence2DInstance).hash_code())
	{
		Sprite->LoadAnimationInstance<CAnimationSequence2DInstance>();
	}

	else if (Type == typeid(COnionMonsterAnimation).hash_code())
	{
		Sprite->LoadAnimationInstance<COnionMonsterAnimation>();
	}

	else if (Type == typeid(CLowerClassBookAnimation).hash_code())
	{
		Sprite->LoadAnimationInstance<CLowerClassBookAnimation>();
	}

	else if (Type == typeid(CRadishMonsterAnimation).hash_code())
	{
		Sprite->LoadAnimationInstance<CRadishMonsterAnimation>();
	}

	else if (Type == typeid(CZakumBodyAnimation).hash_code())
	{
		Sprite->LoadAnimationInstance<CZakumBodyAnimation>();
	}

	else if (Type == typeid(CZakumHandAnimation).hash_code())
	{
		Sprite->LoadAnimationInstance<CZakumHandAnimation>();
	}
}

void CClientManager::TurnOffWindow(float DeltaTime)
{
	CScene* Scene = CSceneManager::GetInst()->GetScene();
	if (Scene)
	{
		CSceneMode* Mode = Scene->GetSceneMode();
		if (Mode->GetTypeID() == typeid(CStartScene).hash_code())
			return;

		CWidgetWindow* TopMostWindow = Scene->GetViewport()->FindTopmostWindow();

		// 캐릭터 정보창이나 경험치 UI Window같은 것들은 끄면 안됨
		if (TopMostWindow)
		{
			std::string Name = TopMostWindow->GetName();

			if (Name.find("MainStatus") != std::string::npos || Name.find("EXPWindow") != std::string::npos || Name.find("SkillQuickSlot") != std::string::npos)
			{
				return;
			}
			
			if (TopMostWindow->GetTypeID() == typeid(CToolTip).hash_code())
			{
				// 툴팁도 꺼주고, 툴팁을 나오게한 인벤토리도 꺼줘야함
				((CToolTip*)TopMostWindow)->GetOwnerInventory()->Enable(false);
				TopMostWindow->Enable(false);
				return;
			}
			// 캐릭터 HP/MP UI, 경험치UI, 스킬 퀵슬롯 UI들은 전부 ZOrder가 1로 되어있으므로 1보다는 무조건 크게해야함
			TopMostWindow->SetZOrder(2);
			TopMostWindow->Enable(false);
		}
	}

}

void CClientManager::OnOffInventory(float DeltaTime)
{
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	CSceneMode* Mode = Scene->GetSceneMode();
	if (Mode->GetTypeID() == typeid(CStartScene).hash_code() || 
		Mode->GetTypeID() == typeid(CZakumAltarScene).hash_code())
		return;

	if (Scene)
	{
		CViewport* Viewport = Scene->GetViewport();
		if (Viewport)
		{
			CInventory* Inventory = (CInventory*)Viewport->FindWidgetWindow<CInventory>("Inventory");

			if (Inventory)
			{
				bool IsEnable = Inventory->IsEnable();

				if (IsEnable)
				{
					Inventory->Enable(false);
					// 캐릭터 HP/MP UI, 경험치UI, 스킬 퀵슬롯 UI들은 전부 ZOrder가 1로 되어있으므로 1보다는 무조건 크게해야함
					Inventory->SetZOrder(2);

					Inventory->TurnOffAllToolTip();
				}

				else
				{
					CSceneManager::GetInst()->GetScene()->GetResource()->SoundPlay("UIOpen");

					Inventory->Enable(true);
					int ZOrder = Viewport->GetTopmostWindowZOrder();
					Inventory->SetZOrder(ZOrder + 1);
				}

				// 지금 창에 띄워져있는 UI창(Enable = true)들 간의 ZOrder 순서를 유지하면서
				// 가장 높은 ZOrder를 갖고 있는 Window과 지금 띄우려는 Window가 서로 ZOrder를 Swap해서 지금 띄우려는 창이 제일 위로 오게하게
			}
		}
	}
}

void CClientManager::OnOffConfiguration(float DeltaTime)
{
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	CSceneMode* Mode = Scene->GetSceneMode();
	if (Mode->GetTypeID() == typeid(CStartScene).hash_code() ||
		Mode->GetTypeID() == typeid(CZakumAltarScene).hash_code())
		return;

	if (Scene)
	{
		CViewport* Viewport = Scene->GetViewport();
		if (Viewport)
		{
			CConfigurationWindow* Configuaration = (CConfigurationWindow*)Viewport->FindWidgetWindow<CConfigurationWindow>("ConfigurationWindow");

			if (Configuaration)
			{
				bool IsEnable = Configuaration->IsEnable();

				if (IsEnable)
				{
					Configuaration->Enable(false);
					// 캐릭터 HP/MP UI, 경험치UI, 스킬 퀵슬롯 UI들은 전부 ZOrder가 1로 되어있으므로 1보다는 무조건 크게해야함
					Configuaration->SetZOrder(2);
				}

				else
				{
					CSceneManager::GetInst()->GetScene()->GetResource()->SoundPlay("UIOpen");

					Configuaration->Enable(true);
					int ZOrder = Viewport->GetTopmostWindowZOrder();
					Configuaration->SetZOrder(ZOrder + 1);
				}
			}
		}
	}
}

void CClientManager::OnOffBossMatching(float DeltaTime)
{
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	CSceneMode* Mode = Scene->GetSceneMode();
	if (Mode->GetTypeID() == typeid(CStartScene).hash_code() ||
		Mode->GetTypeID() == typeid(CZakumAltarScene).hash_code())
		return;

	if (Scene)
	{
		CViewport* Viewport = Scene->GetViewport();
		if (Viewport)
		{
			CBossMatching* BossMatching = (CBossMatching*)Viewport->FindWidgetWindow<CBossMatching>("BossMatching");

			if (BossMatching)
			{
				bool IsEnable = BossMatching->IsEnable();

				if (IsEnable)
				{
					BossMatching->Enable(false);
					BossMatching->SetZOrder(2);
				}

				else
				{
					CSceneManager::GetInst()->GetScene()->GetResource()->SoundPlay("UIOpen");

					BossMatching->Enable(true);
					int ZOrder = Viewport->GetTopmostWindowZOrder();
					BossMatching->SetZOrder(ZOrder + 1);
				}
			}
		}
	}
}

void CClientManager::OnOffStatWindow(float DeltaTime)
{
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	CSceneMode* Mode = Scene->GetSceneMode();
	if (Mode->GetTypeID() == typeid(CStartScene).hash_code() ||
		Mode->GetTypeID() == typeid(CZakumAltarScene).hash_code())
		return;

	if (Scene)
	{
		CViewport* Viewport = Scene->GetViewport();
		if (Viewport)
		{
			CStatWindow* StatWindow = (CStatWindow*)Viewport->FindWidgetWindow<CStatWindow>("Stat");

			if (StatWindow)
			{
				bool IsEnable = StatWindow->IsEnable();

				if (IsEnable)
				{
					StatWindow->Enable(false);
					StatWindow->SetZOrder(2);
				}

				else
				{
					CScene* Scene = CSceneManager::GetInst()->GetScene(); 
					
					if (Scene)
					{
						CPlayer2D* Player = (CPlayer2D*)Scene->GetPlayerObject();

						if (Player)
						{
							PlayerInfo Info = Player->GetInfo();

							StatWindow->SetHP(Info.HP);
							StatWindow->SetMP(Info.MP);
						}
					}

					CSceneManager::GetInst()->GetScene()->GetResource()->SoundPlay("UIOpen");

					StatWindow->Enable(true);
					int ZOrder = Viewport->GetTopmostWindowZOrder();
					StatWindow->SetZOrder(ZOrder + 1);
				}
			}
		}
	}
}

void CClientManager::NextMonsterState(Monster_State& State)
{
	switch (State)
	{
	case Monster_State::Idle:
		State = Monster_State::Move;
		break;
	case Monster_State::Move:
		State = Monster_State::Idle;
		break;
	default:
		State = Monster_State::Idle;
	}
}

MonsterInfo CClientManager::FindMonsterInfo(const std::string& Name)
{
	size_t Count = m_vecMonsterInfo.size();

	for (size_t i = 0; i < Count; ++i)
	{
		if (m_vecMonsterInfo[i].Name == Name)
			return m_vecMonsterInfo[i];
	}
	
	MonsterInfo DefaultInfo;

	return DefaultInfo;
}

//bool CClientManager::IsCritical(int Factor)
//{
//	int RamdomNumber = rand();
//
//	int random = (RamdomNumber % (Factor / 20 + 1)) - (Factor / 30);
//
//	float Damage = Factor / 10.f + random;
//
//
//	// 크리티컬 데미지가 뜬 경우 -> 추가적으로 이펙트 달아주기
//	return random > 0;
//}

