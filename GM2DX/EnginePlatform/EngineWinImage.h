#pragma once
#include <Windows.h>
#include <EngineBase/EngineMath.h>
#include <EngineBase/Object.h>

class UEngineWinImage : public UObject
{
public:
	// constrcuter destructer
	ENGINEAPI UEngineWinImage();
	ENGINEAPI ~UEngineWinImage();

	// delete Function
	UEngineWinImage(const UEngineWinImage& _Other) = delete;
	UEngineWinImage(UEngineWinImage&& _Other) noexcept = delete;
	UEngineWinImage& operator=(const UEngineWinImage& _Other) = delete;
	UEngineWinImage& operator=(UEngineWinImage&& _Other) noexcept = delete;

	// void Load(std::string_view _Path);
	ENGINEAPI HDC GetDC()
	{
		return ImageDC;
	}
	
	// �̹� ������� DC�� ���ؼ� �̹����� ����� Ÿ��
	ENGINEAPI void Create(HDC _DC)
	{
		ImageDC = _DC;
	}

	ENGINEAPI void Create(UEngineWinImage* _TargetImage, FVector _Scale);

	// bit���� ũ�� �����Ƶ˴ϴ�. 
	ENGINEAPI void CopyToBit(UEngineWinImage* _TargetImage,
		const FTransform& _Trans);

	// Ʈ���� ���� ���� ����Ѵ�.
	// �������ܵ� �ǰ� �̹��� �Ϻκи� ó���ϴ� �͵� �ȴ�.
	// �̹����� ��� �κ��� �׸��� ��Ȯ�ϰ� ǥ���ϱ� ���ؼ�
	// Ʈ���� ���� �ϳ��� �ʿ��մϴ�.
	ENGINEAPI void CopyToTrans(UEngineWinImage* _TargetImage,
		const FTransform& _RenderTrans, 
		const FTransform& _LTImageTrans,
		UColor _Color = UColor(255, 0, 255, 0));

	ENGINEAPI void CopyToAlpha(UEngineWinImage* _TargetImage,
		const FTransform& _RenderTrans,
		const FTransform& _LTImageTrans,
		unsigned char _Alpha);


	ENGINEAPI void Load(UEngineWinImage* _TargetImage, std::string_view _Path);

	// �̹��� ũ�⸦ ����
	ENGINEAPI FVector GetImageScale() const
	{
		return { Info.bmWidth, Info.bmHeight };
	}

	ENGINEAPI UColor GetColor(FVector _Point, UColor _DefaultColor = UColor::WHITE)
	{
		return GetColor(_Point.ConvertToPoint(), _DefaultColor);
	}

	ENGINEAPI UColor GetColor(FIntPoint _Point, UColor _DefaultColor);

protected:

private:
	// BMP �̹��� ���� �� ��ü����
	// FVector Size;
	HDC ImageDC = nullptr;
	HBITMAP hBitMap = nullptr;

	// �̹��� ���� ������
	BITMAP Info;
};

