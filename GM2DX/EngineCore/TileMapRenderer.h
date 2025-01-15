#pragma once
#include "Renderer.h"
#include <EngineBase/EngineDelegate.h>
#include "SpriteRenderer.h"
#include <unordered_map>

enum ETileMapType
{
	Rect,
	Iso,
};


struct FTileIndex
{
	union
	{
		struct
		{
			int X;
			int Y;
		};
		__int64 Key;
	};
};


struct FTileData
{
	bool IsBlock = false;
	int SpriteIndex = 0;
	FTileIndex Index;
	FResultColor ColorData;
	FSpriteData SpriteData;
};

// ���� :
class UTileMapRenderer : public URenderer
{
public:
	class FrameAnimation
	{
	public:
		UEngineSprite* Sprite = nullptr;
		std::vector<int> FrameIndex;
		std::vector<float> FrameTime;
		std::map<int, EngineDelegate> Events;

		int CurIndex = 0;
		int ResultIndex = 0;
		float CurTime = 0.0f;
		bool Loop = true;
		bool IsEnd = false;

		void Reset()
		{
			CurIndex = 0;
			CurTime = 0;
			ResultIndex = 0;
			IsEnd = false;
		}
	};

	// constrcuter destructer
	ENGINEAPI UTileMapRenderer();
	ENGINEAPI ~UTileMapRenderer();

	// delete Function
	UTileMapRenderer(const UTileMapRenderer& _Other) = delete;
	UTileMapRenderer(UTileMapRenderer&& _Other) noexcept = delete;
	UTileMapRenderer& operator=(const UTileMapRenderer& _Other) = delete;
	UTileMapRenderer& operator=(UTileMapRenderer&& _Other) noexcept = delete;

	ENGINEAPI std::string GetCurSpriteName()
	{
		return Sprite->GetName();
	}

	// �����Ҷ� ��ũ�⸦ �� ���ϰ� �����մϴ�.
	// �׷��� Unordered map�̵Ǹ� �߰��� �󸶵��� ũ�⸦ ���� �ֽ��ϴ�.
	// ENGINEAPI void CreateTileMap(int _X, int _Y, )

	ENGINEAPI void SetTileSetting(ETileMapType _Type, std::string_view _Name, FVector _TileSize, FVector _ImageSize, FVector _Pivot);


	ENGINEAPI void SetTile(FVector _Pos, int _Spriteindex);
	ENGINEAPI void RemoveTile(FVector _Pos);

	// ������ ������.
	ENGINEAPI void SetTile(int _X, int _Y, int _Spriteindex);

	ENGINEAPI void RemoveTile(int _X, int _Y);

	ENGINEAPI class UEngineSprite* GetSprite()
	{
		return Sprite;
	}

	FTileIndex WorldPosToTileIndex(FVector _Pos);

	FVector TileIndexToWorldPos(FTileIndex _Pos);

	// �����͸� ����ȭ(����)
	ENGINEAPI void Serialize(UEngineSerializer& _Ser) override;
	// �����͸� ����(�Ҷ�)
	ENGINEAPI void DeSerialize(UEngineSerializer& _Ser) override;

protected:
	ENGINEAPI void Render(class UEngineCamera* _Camera, float _DeltaTime) override;
	void BeginPlay() override;
	void ComponentTick(float _DeltaTime) override;

private:
	bool IsAutoScale = true;
	float AutoScaleRatio = 1.0f;

	FVector TileSize;
	FVector ImageSize;
	FVector TilePivot;
	ETileMapType TileMapType = ETileMapType::Rect;

	class UEngineSprite* Sprite = nullptr;
	// map���� ������.
// hash ����� ���̴�.
// �̰� �� �̷������� ���Ŀ� ��������� �մϴ�.
// std::vector<std::vector> 
// �߰� Ȯ���� �ɼ� �ִ�.
// 2D �̹����� ����
	std::unordered_map<__int64, FTileData> Tiles;
};

