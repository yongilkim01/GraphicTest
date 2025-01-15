#include "PreCompile.h"
#include "Level.h"
#include "Actor.h"
#include "Renderer.h"
#include "Collision.h"
#include "EngineCore.h"
#include "EngineCamera.h"
#include "CameraActor.h"
#include "EngineGUI.h"
#include "HUD.h"
#include "EngineFont.h"
#include "EngineRenderTarget.h"

// �÷��̾� Renderer

// ī�޶� 1 Renderer
// ī�޶� 2 Renderer



std::shared_ptr<class ACameraActor> ULevel::SpawnCamera(int _Order)
{
	std::shared_ptr<ACameraActor> Camera = std::make_shared<ACameraActor>();

	if (true == Cameras.contains(_Order))
	{
		MSGASSERT("�̹� �����ϴ� ī�޶� �� ������� �߽��ϴ�.");
	}

	Camera->BeginPlay();

	Cameras.insert({ _Order , Camera });
	return Camera;
}

ULevel::ULevel()
{
	SpawnCamera(EEngineCameraType::MainCamera);

	SpawnCamera(EEngineCameraType::UICamera);

	LastRenderTarget = std::make_shared<UEngineRenderTarget>();
	LastRenderTarget->CreateTarget(UEngineCore::GetScreenScale());
	LastRenderTarget->CreateDepth();
}

ULevel::~ULevel()
{
	BeginPlayList.clear();

	AllActorList.clear();

	Cameras.clear();
}

void ULevel::LevelChangeStart()
{
	for (std::shared_ptr<class AActor> Actor : BeginPlayList)
	{
		Actor->LevelChangeStart();
	}


	for (std::shared_ptr<class AActor> Actor : AllActorList)
	{
		Actor->LevelChangeStart();
	}
}

void ULevel::LevelChangeEnd()
{
	for (std::shared_ptr<class AActor> Actor : BeginPlayList)
	{
		Actor->LevelChangeEnd();
	}

	for (std::shared_ptr<class AActor> Actor : AllActorList)
	{
		Actor->LevelChangeEnd();
	}
}


void ULevel::Tick(float _DeltaTime)
{
	if (GetMainCamera()->IsFreeCamera())
	{
		return;
	}

	std::list<std::shared_ptr<class AActor>>::iterator StartIter = BeginPlayList.begin();
	std::list<std::shared_ptr<class AActor>>::iterator EndIter = BeginPlayList.end();
	for (; StartIter != EndIter; )
	{
		std::shared_ptr<AActor> CurActor = *StartIter;

		if (false == CurActor->IsActive())
		{
			++StartIter;
			continue;
		}

		StartIter = BeginPlayList.erase(StartIter);

		CurActor->BeginPlay();

		if (nullptr != CurActor->Parent)
		{
			continue;
		}

		AllActorList.push_back(CurActor);
	}

	// ���� Ranged for�ȿ����� erase ����Ʈ�� ������ ����� ���� ���� ����� �ߤ����.
	for (std::shared_ptr<AActor> CurActor : AllActorList)
	{
		if (false == CurActor->IsActive())
		{
			continue;
		}

		CurActor->Tick(_DeltaTime);
	}
}

void ULevel::Render(float _DeltaTime)
{
	UEngineCore::GetDevice().RenderStart();

	LastRenderTarget->Clear();

	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : Cameras)
	{
		if (Camera.first == static_cast<int>(EEngineCameraType::UICamera))
		{
			continue;
		}
		
		if (false == Camera.second->IsActive())
		{
			continue;
		}

		Camera.second->Tick(_DeltaTime);
		Camera.second->GetCameraComponent()->Render(_DeltaTime);

		// �� �� �׷������� 
		Camera.second->GetCameraComponent()->CameraTarget->MergeTo(LastRenderTarget);
	}

	if (true == Cameras.contains(static_cast<int>(EEngineCameraType::UICamera)))
	{
		std::shared_ptr<ACameraActor> CameraActor = Cameras[static_cast<int>(EEngineCameraType::UICamera)];
		if (true == CameraActor->IsActive())
		{
			std::shared_ptr<UEngineCamera> CameraComponent = Cameras[static_cast<int>(EEngineCameraType::UICamera)]->GetCameraComponent();

			CameraActor->Tick(_DeltaTime);
			CameraComponent->CameraTarget->Clear();
			CameraComponent->CameraTarget->Setting();

			HUD->UIRender(CameraComponent.get(), _DeltaTime);

			


			CameraComponent->CameraTarget->MergeTo(LastRenderTarget);
		}

	} else 
	{
		MSGASSERT("UIī�޶� �������� �ʽ��ϴ�. ���� �����Դϴ�. UIī�޶� �������ּ���.");
	}
	
	// LastRenderTarget->PostEffect();

	std::shared_ptr<UEngineRenderTarget> BackBuffer = UEngineCore::GetDevice().GetBackBufferTarget();
	LastRenderTarget->MergeTo(BackBuffer);


	{
		std::shared_ptr<class ACameraActor> Camera = GetMainCamera();

		// �浹ü ������
		for (std::pair<const std::string, std::list<std::shared_ptr<UCollision>>>& Group : Collisions)
		{
			std::list<std::shared_ptr<UCollision>>& List = Group.second;

			for (std::shared_ptr<UCollision>& _Collision : List)
			{
				if (false == _Collision->IsActive())
				{
					continue;
				}

				_Collision->DebugRender(Camera->GetCameraComponent().get(), _DeltaTime);
			}
		}
	}

	if (true == UEngineWindow::IsApplicationOn())
	{
		UEngineGUI::GUIRender(this);
		// IMGUI�� ���������ϸ鼭 
	}

	UEngineCore::GetDevice().RenderEnd();
}



void ULevel::ChangeRenderGroup(int _CameraOrder, int _PrevGroupOrder, std::shared_ptr<URenderer> _Renderer)
{
	if (false == Cameras.contains(_CameraOrder))
	{
		// �̰� �ָ��������?
		// ī�޶�� �̷��� �߳׿�
		// ī�޶� ���� ������ �ϰ� �������� �����Ҽ� �ִ�.
		MSGASSERT("�������� �ʴ� ī�޶� �������� ����������� �߽��ϴ�.");
		return;
	}

	std::shared_ptr<ACameraActor> Camera = Cameras[_CameraOrder];

	Camera->GetCameraComponent()->ChangeRenderGroup(_PrevGroupOrder, _Renderer);
}

void ULevel::CreateCollisionProfile(std::string_view _ProfileName)
{
	std::string UpperName = UEngineString::ToUpper(_ProfileName);

	Collisions[UpperName];
}

void ULevel::LinkCollisionProfile(std::string_view _LeftProfileName, std::string_view _RightProfileName)
{
	std::string LeftUpperName = UEngineString::ToUpper(_LeftProfileName);
	std::string RightUpperName = UEngineString::ToUpper(_RightProfileName);

	CollisionLinks[LeftUpperName].push_back(RightUpperName);
}

void ULevel::PushCollisionProfileEvent(std::shared_ptr<class URenderer> _Renderer)
{

}

void ULevel::ChangeCollisionProfileName(std::string_view _ProfileName, std::string_view _PrevProfileName, std::shared_ptr<UCollision> _Collision)
{
	if (false == Collisions.contains(_ProfileName.data()))
	{
		MSGASSERT("�������� �ʴ� �ݸ��� �׷쿡 �������� ����������� �߽��ϴ�.");
		return;
	}

	std::string PrevUpperName = UEngineString::ToUpper(_PrevProfileName);

	if (_PrevProfileName != "")
	{
		std::list<std::shared_ptr<UCollision>>& PrevCollisionGroup = Collisions[PrevUpperName];
		PrevCollisionGroup.remove(_Collision);
	}

	std::string UpperName = UEngineString::ToUpper(_ProfileName);

	std::list<std::shared_ptr<UCollision>>& CollisionGroup = Collisions[UpperName];
	CollisionGroup.push_back(_Collision);
}

void ULevel::Collision(float _DeltaTime)
{
	// Monster Player �浹üũ �ؾ� �Ѵ�.
	
	for (std::pair<const std::string, std::list<std::string>>& Links : CollisionLinks)
	{
		const std::string& LeftProfile = Links.first;

		std::list<std::string>& LinkSecond = Links.second;

		for (std::string& RightProfile : LinkSecond)
		{
			std::list<std::shared_ptr<class UCollision>>& LeftList = CheckCollisions[LeftProfile];
			std::list<std::shared_ptr<class UCollision>>& RightList = Collisions[RightProfile];

			for (std::shared_ptr<class UCollision>& LeftCollision : LeftList)
			{
				for (std::shared_ptr<class UCollision>& RightCollision : RightList)
				{
					if (LeftCollision == RightCollision)
					{
						continue;
					}

					if (false == LeftCollision->IsActive())
					{
						continue;
					}

					LeftCollision->CollisionEventCheck(RightCollision);
				}
			}
		}
	}
}

void ULevel::Release(float _DeltaTime)
{
	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : Cameras)
	{
		Camera.second->GetCameraComponent()->Release(_DeltaTime);
	}

	{
		// �浹ü ������
		for (std::pair<const std::string, std::list<std::shared_ptr<UCollision>>>& Group : Collisions)
		{
			std::list<std::shared_ptr<UCollision>>& List = Group.second;

			std::list<std::shared_ptr<UCollision>>::iterator StartIter = List.begin();
			std::list<std::shared_ptr<UCollision>>::iterator EndIter = List.end();

			// �𸮾��� �߰��� �����Ҽ� ����.
			for (; StartIter != EndIter; )
			{
				if (false == (*StartIter)->IsDestroy())
				{
					++StartIter;
					continue;
				}

				// �������� ���� �ʿ䰡 �����ϴ�.
				// (*RenderStartIter) ���� ���� ������ ��������.
				// ������Ʈ�� �޸𸮸� �����Ҽ� ������ ������ ���͸� ������ �ִ�.
				StartIter = List.erase(StartIter);
			}
		}
	}

	{
		// �浹ü ������
		for (std::pair<const std::string, std::list<std::shared_ptr<UCollision>>>& Group : CheckCollisions)
		{
			std::list<std::shared_ptr<UCollision>>& List = Group.second;

			std::list<std::shared_ptr<UCollision>>::iterator StartIter = List.begin();
			std::list<std::shared_ptr<UCollision>>::iterator EndIter = List.end();

			// �𸮾��� �߰��� �����Ҽ� ����.
			for (; StartIter != EndIter; )
			{
				if (false == (*StartIter)->IsDestroy())
				{
					++StartIter;
					continue;
				}

				// �������� ���� �ʿ䰡 �����ϴ�.
				// (*RenderStartIter) ���� ���� ������ ��������.
				// ������Ʈ�� �޸𸮸� �����Ҽ� ������ ������ ���͸� ������ �ִ�.
				StartIter = List.erase(StartIter);
			}
		}
	}

	{
		std::list<std::shared_ptr<AActor>>& List = AllActorList;

		std::list<std::shared_ptr<AActor>>::iterator StartIter = List.begin();
		std::list<std::shared_ptr<AActor>>::iterator EndIter = List.end();

		// �𸮾��� �߰��� �����Ҽ� ����.
		for (; StartIter != EndIter; )
		{
			if (nullptr != (*StartIter)->Parent)
			{
				// �θ� �ִ� �ִ� ������ �θ� �� tick
				// ������ �������ʿ䰡 ��������.
				StartIter = List.erase(StartIter);
				continue;
			}

			if (false == (*StartIter)->IsDestroy())
			{
				++StartIter;
				continue;
			}

			// ���� delete�� �ʿ� ����.
			StartIter = List.erase(StartIter);
		}
	}
}

void ULevel::InitLevel(AGameMode* _GameMode, APawn* _Pawn, AHUD* _HUD)
{
	GameMode = _GameMode;

	MainPawn = _Pawn;

	HUD = _HUD;
}
