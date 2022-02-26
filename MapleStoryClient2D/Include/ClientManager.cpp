
#include "ClientManager.h"
#include "Engine.h"
#include "resource.h"
#include "Scene/SceneManager.h"
#include "Scene/LobbyScene.h"
#include "Scene/OnionScene.h"
#include "Scene/WayToZakumScene.h"
#include "Scene/Library2ndScene.h"
#include "Scene/StartScene.h"
#include "Client.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/DragCollider.h"
#include "Component/StaticMeshComponent.h"
#include "Component/TileMapComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/OnionMonsterAnimation.h"
#include "Input.h"
#include "Widget/MouseNormal.h"
#include "Widget/MouseClick.h"

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

	CSceneManager::GetInst()->SetCreateSceneModeFunction<CClientManager>(this, &CClientManager::CreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectFunction<CClientManager>(this, &CClientManager::CreateObject);
	CSceneManager::GetInst()->SetCreateComponentFunction<CClientManager>(this, &CClientManager::CreateComponent);
	CSceneManager::GetInst()->SetCreateAnimInstanceFunction<CClientManager>(this, &CClientManager::CreateAnimInstance);

	CInput::GetInst()->CreateKey("Inventory", 'I');
	CInput::GetInst()->CreateKey("Configuration", 'C');	
	CInput::GetInst()->CreateKey("BossMatching", 'B');
	CInput::GetInst()->CreateKey("TurnOffUIWindow", VK_ESCAPE);

	CInput::GetInst()->SetKeyCallback<CClientManager>("Inventory", KeyState_Down, this, &CClientManager::OnOffInventory);
	CInput::GetInst()->SetKeyCallback<CClientManager>("Configuration", KeyState_Down, this, &CClientManager::OnOffConfiguration);
	CInput::GetInst()->SetKeyCallback<CClientManager>("BossMatching", KeyState_Down, this, &CClientManager::OnOffBossMatching);
	CInput::GetInst()->SetKeyCallback<CClientManager>("TurnOffUIWindow", Key_State::KeyState_Down, this, &CClientManager::TurnOffWindow);

	//CInput::GetInst()->CreateKey("MovePoint", VK_RBUTTON);







	CResourceManager::GetInst()->CreateSoundChannelGroup("UI");
	CResourceManager::GetInst()->CreateSoundChannelGroup("BGM");
	CResourceManager::GetInst()->CreateSoundChannelGroup("Effect");

	CResourceManager::GetInst()->SetVolume("BGM", 70);

	// ���콺 ���� ����
	CMouseNormal* MouseNormal = CEngine::GetInst()->CreateMouse<CMouseNormal>(Mouse_State::Normal, "MouseNormal");
	CMouseClick* MouseClick = CEngine::GetInst()->CreateMouse<CMouseClick>(Mouse_State::Click, "MouseClick");



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

	//else if (Type == typeid(CMonsterRadish).hash_code())
	//{
	//	CMonsterRadish* Obj = Scene->LoadGameObject<CMonsterRadish>();

	//	return Obj;
	//}

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

	//else if (Type == typeid(CSingleHelixBlinkTree).hash_code())
	//{
	//	CSingleHelixBlinkTree* Obj = Scene->LoadGameObject<CSingleHelixBlinkTree>();

	//	return Obj;
	//}


	//else if (Type == typeid(CLowerClassBook).hash_code())
	//{
	//	CLowerClassBook* Obj = Scene->LoadGameObject<CLowerClassBook>();

	//	return Obj;
	//}

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
			}
		}

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

		// ĳ���� ����â�̳� ����ġ UI Window���� �͵��� ���� �ȵ�
		if (TopMostWindow)
		{
			std::string Name = TopMostWindow->GetName();

			if (Name.find("MainStatus") != std::string::npos || Name.find("EXPWindow") != std::string::npos || Name.find("SkillQuickSlot") != std::string::npos)
			{
				return;
			}

			// ĳ���� HP/MP UI, ����ġUI, ��ų ������ UI���� ���� ZOrder�� 1�� �Ǿ������Ƿ� 1���ٴ� ������ ũ���ؾ���
			TopMostWindow->SetZOrder(2);
			TopMostWindow->Enable(false);
		}
	}

}

void CClientManager::OnOffInventory(float DeltaTime)
{
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	CSceneMode* Mode = Scene->GetSceneMode();
	if (Mode->GetTypeID() == typeid(CStartScene).hash_code())
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
					// ĳ���� HP/MP UI, ����ġUI, ��ų ������ UI���� ���� ZOrder�� 1�� �Ǿ������Ƿ� 1���ٴ� ������ ũ���ؾ���
					Inventory->SetZOrder(2);
				}

				else
				{
					Inventory->Enable(true);
					int ZOrder = Viewport->GetTopmostWindowZOrder();
					Inventory->SetZOrder(ZOrder + 1);
				}

				// ���� â�� ������ִ� UIâ(Enable = true)�� ���� ZOrder ������ �����ϸ鼭
				// ���� ���� ZOrder�� ���� �ִ� Window�� ���� ������ Window�� ���� ZOrder�� Swap�ؼ� ���� ������ â�� ���� ���� �����ϰ�
			}
		}
	}
}

void CClientManager::OnOffConfiguration(float DeltaTime)
{
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	CSceneMode* Mode = Scene->GetSceneMode();
	if (Mode->GetTypeID() == typeid(CStartScene).hash_code())
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
					// ĳ���� HP/MP UI, ����ġUI, ��ų ������ UI���� ���� ZOrder�� 1�� �Ǿ������Ƿ� 1���ٴ� ������ ũ���ؾ���
					Configuaration->SetZOrder(2);
				}

				else
				{
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
	if (Mode->GetTypeID() == typeid(CStartScene).hash_code())
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
					BossMatching->Enable(true);
					int ZOrder = Viewport->GetTopmostWindowZOrder();
					BossMatching->SetZOrder(ZOrder + 1);
				}
			}
		}
	}
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
//	// ũ��Ƽ�� �������� �� ��� -> �߰������� ����Ʈ �޾��ֱ�
//	return random > 0;
//}

