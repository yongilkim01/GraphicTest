#pragma once
#include <EngineCore/HUD.h>

// ���� :
class ATitleHUD : public AHUD
{
public:
	// constrcuter destructer
	ATitleHUD();
	~ATitleHUD();

	// delete Function
	ATitleHUD(const ATitleHUD& _Other) = delete;
	ATitleHUD(ATitleHUD&& _Other) noexcept = delete;
	ATitleHUD& operator=(const ATitleHUD& _Other) = delete;
	ATitleHUD& operator=(ATitleHUD&& _Other) noexcept = delete;

	void BeginPlay();
	void Tick(float _DeltaTime);

protected:

private:
};

