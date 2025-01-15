#include "PreCompile.h"
#include "TestGameMode.h"
#include "TitleLogo.h"
#include "Monster.h"
#include "Monster2.h"
#include <EngineCore/CameraActor.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include <EngineCore/EngineCamera.h>
#include "ContentsEditorGUI.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineContents/TestActor.h>


ATestGameMode::ATestGameMode()
{
	// 레벨마다 해주셔야 합니다.
// 이걸 UI공유할건지 
	GetWorld()->CreateCollisionProfile("Monster");

	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	TestActor = GetWorld()->SpawnActor<ATestActor>();

	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f, 1.0f });
	Camera->GetCameraComponent()->SetZSort(0, true);

}

ATestGameMode::~ATestGameMode()
{

}

void ATestGameMode::Tick(float _DeltaTime)
{
	// 부모 호출
	AActor::Tick(_DeltaTime);

	if (true == UEngineInput::IsDown('P'))
	{
		GetWorld()->GetMainCamera()->FreeCameraSwitch();

	}

	TestActor->AddActorRotation({ 0.0f, 0.0f, 100.0f * _DeltaTime });

}