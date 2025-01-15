#include "PreCompile.h"
#include "EngineSprite.h"
#include "EngineBase/EngineDebug.h"
#include "EngineTexture.h"

UEngineSprite::UEngineSprite()
{
}

UEngineSprite::~UEngineSprite()
{
}


std::shared_ptr<UEngineSprite> UEngineSprite::CreateSpriteToFolder(std::string_view _Name, std::string_view _Path)
{
	UEngineDirectory Dir = _Path;

	std::vector<UEngineFile> Files = Dir.GetAllFile(false, { ".png"});

	if (0 == Files.size())
	{
		MSGASSERT("������ �������� �ʴ� ������ ��������Ʈ�� ������� �����ϴ�.");
	}

	std::shared_ptr<UEngineSprite> NewRes = std::make_shared<UEngineSprite>();
	PushRes<UEngineSprite>(NewRes, _Name, "");

	for (size_t i = 0; i < Files.size(); i++)
	{
		std::string UpperName = UEngineString::ToUpper(Files[i].GetFileName());

		std::shared_ptr<UEngineTexture> Texture = UEngineTexture::Find<UEngineTexture>(UpperName);

		if (nullptr == Texture)
		{
			MSGASSERT("�ؽ�ó�� ���� �ε��ϰ� ���� ��������Ʈ�� ����� �ּ���." + UpperName);
			return nullptr;
		}

		NewRes->SpriteTexture.push_back(Texture.get());

		FSpriteData SpriteData;
		SpriteData.CuttingPos = { 0.0f, 0.0f };
		SpriteData.CuttingSize = { 1.0f, 1.0f };
		SpriteData.Pivot = { 0.5f, 0.5f };
		NewRes->SpriteDatas.push_back(SpriteData);
	}

	return NewRes;
}

std::shared_ptr<UEngineSprite> UEngineSprite::CreateSpriteToMeta(std::string_view _Name, std::string_view _DataFileExt)
{
	std::shared_ptr<UEngineTexture> Tex = UEngineTexture::Find<UEngineTexture>(_Name);

	if (nullptr == Tex)
	{
		MSGASSERT("�������� �ʴ� �ؽ�ó�� ��������Ʈ�� ������� �����ϴ�.");
		return nullptr;
	}

	std::shared_ptr<UEngineSprite> NewRes = std::make_shared<UEngineSprite>();
	PushRes<UEngineSprite>(NewRes, _Name, "");


	// �Ľ� ���ڿ��� �ڸ��� �ɰ��� �ǹ̸� �ο��ϴ� ���� �Ľ��̶�� �մϴ�.
	// ���ڰ� ���� ��Ģ���� �����Ǿ��ִٴ� ���� �ľ��ϰ� 
	// �̰� ������� �� ������ ��Ʈ��ó�� ����� �θ���.
	// ��ǥ������ xml json ����� �ִ�.

	UEnginePath Path = Tex->GetPath();
	std::string FileName = Path.GetFileName();
	FileName += _DataFileExt;
	Path.MoveParent();
	Path.Append(FileName);

	UEngineFile File = Path;

	File.FileOpen("rt");

	std::string Text = File.GetAllFileText();

	std::vector<std::string> SpriteDataTexts;

	size_t StartPosition = 0;
	while (true)
	{
		size_t RectIndex = Text.find("rect:", StartPosition);
		size_t AligIndex = Text.find("outline:", RectIndex);

		if (RectIndex == std::string::npos || AligIndex == std::string::npos)
		{
			break;
		}

		NewRes->SpriteTexture.push_back(Tex.get());
		SpriteDataTexts.push_back(Text.substr(RectIndex, AligIndex - RectIndex));
		StartPosition = AligIndex;
	}

	FVector TexSize = Tex->GetTextureSize();

	std::vector<FSpriteData> TestData;

	for (size_t i = 0; i < SpriteDataTexts.size(); i++)
	{
		std::string Text = SpriteDataTexts[i];

		size_t Start = 0;

		FSpriteData SpriteData;


		{
			std::string Number = UEngineString::InterString(Text, "x:", "\n", Start);
			SpriteData.CuttingPos.X = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "y:", "\n", Start);
			SpriteData.CuttingPos.Y = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "width:", "\n", Start);
			SpriteData.CuttingSize.X = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "height:", "\n", Start);
			SpriteData.CuttingSize.Y = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "x:", ",", Start);
			SpriteData.Pivot.X = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "y:", "}", Start);
			SpriteData.Pivot.Y = static_cast<float>(atof(Number.c_str()));
		}



		SpriteData.CuttingPos.Y = TexSize.Y - SpriteData.CuttingPos.Y - SpriteData.CuttingSize.Y;

		SpriteData.CuttingPos.X /= TexSize.X;
		SpriteData.CuttingPos.Y /= TexSize.Y;
		SpriteData.CuttingSize.X /= TexSize.X;
		SpriteData.CuttingSize.Y /= TexSize.Y;


		TestData.push_back(SpriteData);
	}


	NewRes->SpriteDatas = TestData;

	return NewRes;

}


UEngineTexture* UEngineSprite::GetTexture(size_t _Index /*= 0*/)
{
	return SpriteTexture[_Index];
}

ID3D11ShaderResourceView* UEngineSprite::GetSRV(size_t _Index/* = 0*/)
{
	return SpriteTexture[_Index]->GetSRV();
}

FVector UEngineSprite::GetSpriteScaleToReal(size_t _Index)
{
	if (SpriteDatas.size() <= _Index)
	{
		MSGASSERT("��������Ʈ�� �ε����� �ʰ��Ͽ� ����Ϸ��� �߽��ϴ�.");
	}

	FVector Result;

	//                0~1������ �����̱� ������
	Result.X = SpriteDatas[_Index].CuttingSize.X * SpriteTexture[_Index]->GetTextureSize().X;
	Result.Y = SpriteDatas[_Index].CuttingSize.Y * SpriteTexture[_Index]->GetTextureSize().Y;

	return Result;
}