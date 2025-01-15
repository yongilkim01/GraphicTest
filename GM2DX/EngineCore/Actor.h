#pragma once
#include "SceneComponent.h"

// ���ϱ����� �̾߱��� ���ô�.
// ���� :
// �𸮾󿡼� Actor�� ���� Ʈ�������� ������ �ʴ´�.
class AActor : public UObject
{
	friend class ULevel;

public:
	// constrcuter destructer
	ENGINEAPI AActor();
	ENGINEAPI ~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

	// �����Լ��� ������ ��������ִ� �̴ϴ�.
	// ����ȣ���ϴ� ���� ������ �ȵ˴ϴ�.
	ENGINEAPI virtual void BeginPlay();
	ENGINEAPI virtual void Tick(float _DeltaTime);

	virtual void LevelChangeStart() {}
	virtual void LevelChangeEnd() {}


	// �̳༮ �� ���� 
	template<typename ComponentType>
	inline std::shared_ptr<ComponentType> CreateDefaultSubObject()
	{
		static_assert(std::is_base_of_v<UActorComponent, ComponentType>, "���� ������Ʈ�� ��ӹ��� ���� Ŭ������ CreateDefaultSubObject�Ϸ��� �߽��ϴ�.");

		if (false == std::is_base_of_v<UActorComponent, ComponentType>)
		{
			MSGASSERT("���� ������Ʈ�� ��ӹ��� ���� Ŭ������ CreateDefaultSubObject�Ϸ��� �߽��ϴ�.");
			return nullptr;
			// static_assert
		}

		char* ComMemory = new char[sizeof(ComponentType)];

		UActorComponent* ComPtr = reinterpret_cast<ComponentType*>(ComMemory);
		ComPtr->Actor = this;

		ComponentType* NewPtr = reinterpret_cast<ComponentType*>(ComMemory);
		// �������� �����ϰ�
		// �÷��̽���Ʈ new 
		std::shared_ptr<ComponentType> NewCom(new(ComMemory) ComponentType());

		// ���� �׳� ActorComponent
		// ���� �׳� SceneComponent
		if (std::is_base_of_v<UActorComponent, ComponentType> 
			&& !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			ActorComponentList.push_back(NewCom);
		}
		else if(!std::is_base_of_v<UActorComponent, ComponentType>
			&& !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			MSGASSERT("���� �ȵ�");
		}

		return NewCom;
	}

	ULevel* GetWorld()
	{
		return World;
	}

	void SetActorLocation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->SetWorldLocation(_Value);
	}

	void AddActorLocation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->AddWorldLocation(_Value);
	}

	void SetActorRelativeScale3D(const FVector& _Scale)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->SetRelativeScale3D(_Scale);
	}

	void AddRelativeLocation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->AddRelativeLocation(_Value);
	}

	void SetActorRotation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->SetRotation(_Value);
	}

	void AddActorRotation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->AddWorldRotation(_Value);
	}

	ENGINEAPI void AttachToActor(AActor* _Parent);

	FVector GetActorLocation()
	{
		return RootComponent->Transform.WorldLocation;
	}

	// Ʈ������ ��ü�� ��ĥ���� ����. ���纻�� �ִ� �Լ�.
	FTransform GetActorTransform()
	{
		if (nullptr == RootComponent)
		{
			return FTransform();
		}

		return RootComponent->GetTransformRef();
	}

	void SetActorTransform(const FTransform& _Transform)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->Transform = _Transform;

		return;
	}


	ENGINEAPI FVector GetActorUpVector();
	ENGINEAPI FVector GetActorRightVector();
	ENGINEAPI FVector GetActorForwardVector();

protected:
	std::shared_ptr<class USceneComponent> RootComponent = nullptr;

private:
	// ������ �ڽ������� �˰� 
	AActor* Parent = nullptr;
	// �ڱ� �ڽĵ鵵 �˰� �ȴ�.
	std::list<std::shared_ptr<AActor>> ChildList;

	// �ʱ�ȭ �ϸ� �ȵ˴ϴ�.
	// �������� ����� ����Ǿ����ϱ�.
	ULevel* World;

	std::list<std::shared_ptr<class UActorComponent>> ActorComponentList;
};

