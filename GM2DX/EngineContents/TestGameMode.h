#pragma once
#include <EngineCore/GameMode.h>

class ATestActor;

// Ό³Έν :
class ATestGameMode : public AGameMode
{
public:
	// constrcuter destructer
	ATestGameMode();
	~ATestGameMode();

	// delete Function
	ATestGameMode(const ATestGameMode& _Other) = delete;
	ATestGameMode(ATestGameMode&& _Other) noexcept = delete;
	ATestGameMode& operator=(const ATestGameMode& _Other) = delete;
	ATestGameMode& operator=(ATestGameMode&& _Other) noexcept = delete;

	void Tick(float _DeltaTime);

protected:

private:
	std::shared_ptr<ATestActor> TestActor = nullptr;

};
