#include "PreCompile.h"
#include "TestActor.h"
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/TimeEventComponent.h>
#include <EngineCore/Collision.h>
#include "MyCustomRenderer.h"
#include "TestRenderer.h"

ATestActor::ATestActor()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	Renderer = CreateDefaultSubObject<TestRenderer>();
	Renderer->SetupAttachment(RootComponent);
	Renderer->SetScale3D({ 200.0f, 200.0f, 200.0f });
}

ATestActor::~ATestActor()
{
}

void ATestActor::BeginPlay()
{
	AActor::BeginPlay();


}

void ATestActor::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}