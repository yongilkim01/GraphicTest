#include "PreCompile.h"
#include "ImageWidget.h"
#include "EngineSprite.h"

UImageWidget::UImageWidget()
{
	// ���ϱ����� ���� �༮�̶�� ���� �˷��ش�.
	RenderUnit.TransformObject = this;
	RenderUnit.SetMesh("Rect");
	RenderUnit.SetMaterial("WidgetMaterial");

	RenderUnit.ConstantBufferLinkData("ResultColor", ColorData);
	RenderUnit.ConstantBufferLinkData("FSpriteData", SpriteData);
	RenderUnit.ConstantBufferLinkData("FUVValue", UVValue);

	UVValue.PlusUVValue = { 0.0f, 0.0f, 0.0f, 0.0f };
	SpriteData.CuttingPos = { 0.0f, 0.0f };
	SpriteData.CuttingSize = { 1.0f, 1.0f };
	SpriteData.Pivot = { 0.5f, 0.5f };

	ColorData.PlusColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
}

UImageWidget::~UImageWidget()
{
}

void UImageWidget::Render(UEngineCamera* Camera, float _DeltaTime)
{
	UWidget::Render(Camera, _DeltaTime);

	if (true == IsAutoScale && nullptr != Sprite)
	{
		FVector Scale = Sprite->GetSpriteScaleToReal(CurIndex);
		Scale.Z = 1.0f;
		SetRelativeScale3D(Scale * AutoScaleRatio);
	}

	CameraTransUpdate(Camera);

	RenderUnit.Render(Camera, _DeltaTime);
}


void UImageWidget::SetSprite(std::string_view _Name, UINT _Index /*= 0*/)
{
	Sprite = UEngineSprite::Find<UEngineSprite>(_Name).get();

	if (nullptr == Sprite)
	{
		MSGASSERT("�������� �ʴ� �ؽ�ó�� �����Ϸ��� �߽��ϴ�.");
		return;
	}

	GetRenderUnit().SetTexture("ImageTexture", Sprite->GetTexture(_Index)->GetName());
	SpriteData = Sprite->GetSpriteData(_Index);
}

void UImageWidget::SetTexture(std::string_view _Name, bool AutoScale /*= false*/, float _Ratio /*= 1.0f*/)
{
	std::shared_ptr<UEngineTexture> Texture = UEngineTexture::Find<UEngineTexture>(_Name);

	if (nullptr == Texture)
	{
		MSGASSERT("�ε����� ���� �ؽ�ó�� ����Ϸ��� �߽��ϴ�.");
	}

	GetRenderUnit().SetTexture("ImageTexture", _Name);

	if (true == AutoScale)
	{
		SetRelativeScale3D(Texture->GetTextureSize() * _Ratio);
	}
}