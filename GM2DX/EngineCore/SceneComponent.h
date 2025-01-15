#pragma once
#include <EngineBase/EngineMath.h>
#include "ActorComponent.h"
#include "TransformObject.h"

// ���� :
class USceneComponent : public UActorComponent, public UTransformObject
{
	friend class AActor;

	// Local �θ��� ����
	// World �ȹ���
	// �ƹ��͵� ���� �ȹ���

public:
	// constrcuter destructer
	USceneComponent();
	virtual ~USceneComponent() = 0;

	// delete Function
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

	ENGINEAPI void ComponentTick(float _DeltaTime) override;

protected:

	ENGINEAPI void BeginPlay() override;


private:
};

