#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineWinImage.h>

// Ό³Έν :
class ATestActor : public AActor
{
public:
	// constrcuter destructer
	ATestActor();
	~ATestActor();

	// delete Function
	ATestActor(const ATestActor& _Other) = delete;
	ATestActor(ATestActor&& _Other) noexcept = delete;
	ATestActor& operator=(const ATestActor& _Other) = delete;
	ATestActor& operator=(ATestActor&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:

	std::shared_ptr<class TestRenderer> Renderer;

};

