#include "PreCompile.h"
#include "Actor.h"
#include "SceneComponent.h"

AActor::AActor()
{
}

AActor::~AActor()
{
	RootComponent = nullptr;
}

void AActor::BeginPlay()
{
	if (nullptr != RootComponent)
	{
		RootComponent->BeginPlay();
	}

	for (std::shared_ptr<class UActorComponent>& ActorComponent : ActorComponentList)
	{
		ActorComponent->BeginPlay();
	}
	
}

void AActor::Tick(float _DeltaTime)
{
	// �ڱ� �ڽĵ��� tick�� �����ش�.
	for (std::shared_ptr<AActor>& Actor : ChildList)
	{
		Actor->Tick(_DeltaTime);
	}

	// �����ƺ��� �� rootcomponent�� tick�� �÷��ٲ���
	// �����ƺ��� �ִ� actor�� �ڽ��� root�� ������Ʈ�� �����ʿ䰡 �����.
	if (nullptr == Parent)
	{
		if (nullptr != RootComponent)
		{
			RootComponent->ComponentTick(_DeltaTime);
		}
	}
	

	for (std::shared_ptr<class UActorComponent> ActorComponent : ActorComponentList)
	{
		if (false == ActorComponent->IsActive())
		{
			continue;
		}

		ActorComponent->ComponentTick(_DeltaTime);
	}
}

void AActor::AttachToActor(AActor* _Parent)
{
	if (nullptr == RootComponent)
	{
		MSGASSERT("�� ������Ʈ�� ��Ʈ�� �ƴ� ���Ͱ� �θ� ������ �����ϴ�.");
		return;
	}

	if (nullptr == _Parent->RootComponent)
	{
		MSGASSERT("�� ������Ʈ�� ��Ʈ�� �ƴ� ���Ͱ� �θ� ������ �����ϴ�.");
		return;
	}

	Parent = _Parent;
	// ���� �θ�� �ڽ����� ����.
	_Parent->ChildList.push_back(GetThis<AActor>());
	RootComponent->SetupAttachment(_Parent->RootComponent);
}


FVector AActor::GetActorUpVector()
{
	if (nullptr == RootComponent)
	{
		return FVector(0.0f, 0.0f, 0.0f, 1.0f);
	}

	return RootComponent->GetTransformRef().World.GetUp();
}

FVector AActor::GetActorRightVector()
{
	if (nullptr == RootComponent)
	{
		return FVector(0.0f, 0.0f, 0.0f, 1.0f);
	}
	return RootComponent->GetTransformRef().World.GetRight();
}

FVector AActor::GetActorForwardVector()
{
	if (nullptr == RootComponent)
	{
		return FVector(0.0f, 0.0f, 0.0f, 1.0f);
	}
	return RootComponent->GetTransformRef().World.GetFoward();
}