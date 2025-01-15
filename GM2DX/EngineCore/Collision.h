#pragma once
#include "SceneComponent.h"
#include "EngineSprite.h"
#include "RenderUnit.h"
#include <set>

// ���� : � �������̵� �Ҽ� �մ� ������ ����ڴ�.
// �������̶� �������� �ϴ°� �ƴմϴ�. 3D
class UCollision : public USceneComponent
{
public:
	// constrcuter destructer
	ENGINEAPI UCollision();
	ENGINEAPI ~UCollision();

	// delete Function
	UCollision(const UCollision& _Other) = delete;
	UCollision(UCollision&& _Other) noexcept = delete;
	UCollision& operator=(const UCollision& _Other) = delete;
	UCollision& operator=(UCollision&& _Other) noexcept = delete;

	ENGINEAPI void SetCollisionProfileName(std::string_view _ProfileName);
	ENGINEAPI void BeginPlay() override;
	ENGINEAPI void DebugRender(UEngineCamera* _Camera, float _DeltaTime);

	std::string GetCollisionProfileName()
	{
		return ProfileName;
	}

	void SetRadius(float _Value);

	void Release();

	// vector�ΰ� �� vector�� �ϳ�
	// unreal���� vector
	ENGINEAPI bool CollisionCheck(std::string_view _OtherName, std::vector<UCollision*>& _Vector);

	ENGINEAPI bool CollisionCheck(std::string_view _OtherName, FVector _NextPos, std::vector<UCollision*>& _Vector);

	void SetCollisionType(ECollisionType _Type)
	{
		CollisionType = _Type;
	}

	void CollisionEventCheck(std::shared_ptr<UCollision> _Other);

	ENGINEAPI void SetCollisionEnter(std::function<void(UCollision*, UCollision*)> _Function);
	ENGINEAPI void SetCollisionStay(std::function<void(UCollision*, UCollision*)> _Function);
	ENGINEAPI void SetCollisionEnd(std::function<void(UCollision*, UCollision*)> _Function);

	bool IsEvent()
	{
		return Enter != nullptr || Stay != nullptr || End != nullptr;
	}

private:
	ECollisionType CollisionType = ECollisionType::OBB2D;
	// ���� �浹�� ��븦 ����ϴ� �뵵�� set
	std::set<UCollision*> CollisionCheckSet;

	// ���� Monster �׷��̴�.
	std::string ProfileName = "NONE";

	// ���� �浹
	std::function<void(UCollision*, UCollision*)> Enter;

	// ���� �浹 <= �𸮾��� �̰� �����ϴ�.
	std::function<void(UCollision*, UCollision*)> Stay;

	// �浹 ��.
	std::function<void(UCollision*, UCollision*)> End;
};

