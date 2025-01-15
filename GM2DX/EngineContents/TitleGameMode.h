#pragma once
#include <EngineCore/GameMode.h>

// ���� :
class ATitleGameMode : public AGameMode
{
public:
	// constrcuter destructer
	ATitleGameMode();
	~ATitleGameMode();

	// delete Function
	ATitleGameMode(const ATitleGameMode& _Other) = delete;
	ATitleGameMode(ATitleGameMode&& _Other) noexcept = delete;
	ATitleGameMode& operator=(const ATitleGameMode& _Other) = delete;
	ATitleGameMode& operator=(ATitleGameMode&& _Other) noexcept = delete;

	void Tick(float _DeltaTime);


protected:
	void LevelChangeStart() override;

private:
	// �̷��� ��� ������ ���� �׿��ٰ� �����ϴµ�. ��������� �ִ�.
	// ATitleLogo* Logo; => ���۸� ������ ��Ȱ.
	std::shared_ptr<class ATitleLogo> Logo;
};

//
//// �ﰢ�� �ϳ�
//struct FNaviData
//{
//	int Index;
//
//	FVector Arr[3];
//
//	int ArrIndex[3];
//
//	// std::vector<FNaviData*> LinkArr;
//	// std::vector<int> Arr;
//
//	void Intersect(AActor* Player, float& Dis)
//	{
//		// DirectX::TriangleTests::Intersects()
//	}
//};
//
//class ANaviMeshManager : AActor
//{
//	std::vector<FNaviData> Data;
//
//	void LoadModel(std::string_view _Path)
//	{
//		std::vector<FVector> AllVector;
//	}
//
//	// ������ ������ָ� �˴ϴ�.
//	void LinkProgress();
//};
//
//class Player
//{
//	FNaviData* CurNaviMesh;
//};