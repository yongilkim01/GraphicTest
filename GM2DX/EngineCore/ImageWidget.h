#pragma once
#include "Widget.h"

// ���� :
class UImageWidget : public UWidget
{
public:
	// constrcuter destructer
	ENGINEAPI UImageWidget();
	ENGINEAPI ~UImageWidget();

	// delete Function
	UImageWidget(const UImageWidget& _Other) = delete;
	UImageWidget(UImageWidget&& _Other) noexcept = delete;
	UImageWidget& operator=(const UImageWidget& _Other) = delete;
	UImageWidget& operator=(UImageWidget&& _Other) noexcept = delete;

	void Render(UEngineCamera* Camera, float _DeltaTime) override;

	URenderUnit& GetRenderUnit()
	{
		return RenderUnit;
	}

	ENGINEAPI void SetSprite(std::string_view _Name, UINT _Index = 0);
	ENGINEAPI void SetTexture(std::string_view _Name, bool AutoScale = false, float _Ratio = 1.0f);

	void SetAutoScale(bool _Value)
	{
		IsAutoScale = _Value;
	}

	void SetAutoScaleRatio(float _Scale)
	{
		AutoScaleRatio = _Scale;
	}

	FResultColor ColorData;
	FUVValue UVValue;
	FSpriteData SpriteData;


protected:

private:
	int CurIndex = 0;
	bool IsAutoScale = true;
	float AutoScaleRatio = 1.0f;
	class UEngineSprite* Sprite = nullptr;

	URenderUnit RenderUnit;

};

