#pragma once
#include "EngineConstantBuffer.h"
#include <EngineBase/Object.h>
#include "EngineTexture.h"
#include "EngineSampler.h"

class UEngineShaderRes 
{
public:
	std::string Name;
	EShaderType ShaderType = EShaderType::MAX;
	UINT BindIndex = 0;
};

class UEngineConstantBufferRes : public UEngineShaderRes
{
public:
	void* Data = nullptr; // �ڽſ��� ���õ� �����ʹ� ������ ������ ���� ���̴�.
	UINT BufferSize;
	std::shared_ptr<UEngineConstantBuffer> Res;

	void Setting()
	{
		if (nullptr != Data)
		{
			Name;
			Res->ChangeData(Data, BufferSize);
		}

		Res->Setting(ShaderType, BindIndex);
	}
};

class UEngineTextureRes : public UEngineShaderRes
{
public:
	UEngineTexture* Res;

	void Setting()
	{
		Res->Setting(ShaderType, BindIndex);
	}
};

class UEngineSamplerRes : public UEngineShaderRes
{
public:
	std::shared_ptr<UEngineSampler> Res;

	void Setting()
	{
		Res->Setting(ShaderType, BindIndex);
	}

};

// Render�� 2�� �������.
// Renderer1
// ������ �׸��� �ʹ�.
// 
// Renderer2
// ����� �׸��� �;�.

// ���� : �ְ� ���̴��� �˻��ؼ� 
// �� ���������� �����ؾ��� ���� �ٸ���?
// �̳༮�� ������ ũ�� 2���� �Դϴ�.

// ���̴��� ������ �ִ� �� �������� ���̴� ���ҽ��� �����ϴ� ����
// �������� ������ �ִ� �� �׷��� �ؼ� ����� ���ҽ����� �����͸� �����ϴ� ���ҷ� ������ ���� �̴ϴ�.
class UEngineShaderResources
{
	// 

public:
	// constrcuter destructer
	UEngineShaderResources();
	~UEngineShaderResources();

	// delete Function
	// �̳༮�� ���縦 ���������� �غ����̴�.
	//UEngineShaderResources(const UEngineShaderResources& _Other) = delete;
	//UEngineShaderResources(UEngineShaderResources&& _Other) noexcept = delete;
	//UEngineShaderResources& operator=(const UEngineShaderResources& _Other) = delete;
	//UEngineShaderResources& operator=(UEngineShaderResources&& _Other) noexcept = delete;

	void CreateSamplerRes(std::string_view _Name, UEngineSamplerRes _Res);

	void CreateTextureRes(std::string_view _Name, UEngineTextureRes _Res);

	void CreateConstantBufferRes(std::string_view _Name, UEngineConstantBufferRes Res);

	template<typename DataType>
	void ConstantBufferLinkData(std::string_view _Name, DataType& Data)
	{
		ConstantBufferLinkData(_Name, reinterpret_cast<void*>(&Data));
	}

	void ConstantBufferLinkData(std::string_view _Name, void* Data);

	void SamplerSetting(std::string_view _Name, std::string_view _ResName);
	void TextureSetting(std::string_view _Name, std::string_view _ResName);
	void TextureSetting(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture);

	void TextureSetting(std::string_view _Name, UEngineTexture* _Texture);

	bool IsSampler(std::string_view _Name);
	bool IsTexture(std::string_view _Name);
	bool IsConstantBuffer(std::string_view _Name);
	void Setting();

protected:

private:
	std::map<std::string, UEngineConstantBufferRes> ConstantBufferRes;
	std::map<std::string, UEngineTextureRes> TextureRes;
	std::map<std::string, UEngineSamplerRes> SamplerRes;
	// std::map<std::string, UEngineConstantBufferRes> ConstantBufferSetters;

};

