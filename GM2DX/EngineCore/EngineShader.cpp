#include "PreCompile.h"
#include "EngineShader.h"
#include "EngineVertexShader.h"
#include "EnginePixelShader.h"
#include "EngineConstantBuffer.h"

UEngineShader::UEngineShader()
{
}

UEngineShader::~UEngineShader()
{
}

void UEngineShader::ReflectionCompile(UEngineFile& _File)
{
	// �ȼ��� ���ؽ��� �� ������ �Ѵ�.

	_File.FileOpen("rt");
	std::string ShaderCode = _File.GetAllFileText();
	

	{
		size_t EntryIndex = ShaderCode.find("_VS(");

		if (EntryIndex != std::string::npos)
		{
			{
				// �������� ã�Ƴ����� �Լ�
				size_t FirstIndex = ShaderCode.find_last_of(" ", EntryIndex);

				std::string EntryName = ShaderCode.substr(FirstIndex + 1, EntryIndex - FirstIndex - 1);
				EntryName += "_VS";

				UEngineVertexShader::Load(_File.GetPathToString(), EntryName);
				
			}
		}
	}

	{
		size_t EntryIndex = ShaderCode.find("_PS(");

		if (EntryIndex != std::string::npos)
		{
			{
				// �������� ã�Ƴ����� �Լ�
				size_t FirstIndex = ShaderCode.find_last_of(" ", EntryIndex);

				std::string EntryName = ShaderCode.substr(FirstIndex + 1, EntryIndex - FirstIndex - 1);
				EntryName += "_PS";

				UEnginePixelShader::Load(_File.GetPathToString(), EntryName);
			}
		}
	}
}

void UEngineShader::ShaderResCheck()
{
	// ���÷����̶�� ���� c#���� �ֽž��鿡�� ���� �����ϴµ�.
	// RTTI��� ���ø� �˴ϴ�.
	// RTTI ��Ÿ�� Ÿ�� �������̼�
	// C#���� ������ �帮�� c#�� � Ŭ���� 
	// classInfo Info = typeid(Player);
	// Info.FunctionCount();
	// Info.PrivateFunctionCount();

	// shader��� ���?
	// Info.ConstantBufferCount();

	// ������� �� ��� ���̴����� ����� ���ҽ� �˻�����Դϴ�.
	// �翬�� ���̷�Ʈ x���� �������ִ� ����� ������� 
	if (nullptr == ShaderCodeBlob)
	{
		MSGASSERT("���̴��� �����ϵ��� �ʾƼ� ���̴��� ���ҽ��� �����Ҽ��� �����ϴ�.");
		return;
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> CompileInfo = nullptr;


	// #include <d3dcompiler.h>
	if (S_OK != D3DReflect(ShaderCodeBlob->GetBufferPointer(), ShaderCodeBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &CompileInfo))
	{
		MSGASSERT("���÷��ǿ� �����߽��ϴ�.");
		return;
	}

	D3D11_SHADER_DESC Info;
	CompileInfo->GetDesc(&Info);

	D3D11_SHADER_INPUT_BIND_DESC ResDesc;

	// ������� �ؽ�ó ������ �ʰ� �׳� ���ҽ� ���� Info.BoundResources
	for (UINT i = 0; i < Info.BoundResources; i++)
	{
		CompileInfo->GetResourceBindingDesc(i, &ResDesc);

		std::string Name = ResDesc.Name;
		std::string UpperName = UEngineString::ToUpper(Name);

		D3D_SHADER_INPUT_TYPE Type = ResDesc.Type;

		switch (Type)
		{
		case D3D_SIT_CBUFFER:
		{
			// ��üȭ�� ������ ���ɴϴ�.
			ID3D11ShaderReflectionConstantBuffer* Info = CompileInfo->GetConstantBufferByName(ResDesc.Name);
			D3D11_SHADER_BUFFER_DESC BufferInfo = { 0 };
			Info->GetDesc(&BufferInfo);

			// ������� ���ҽ� �����ϴ� ������
			// �� ���̴��� ����ؼ� �������� �Ҷ����� �� ������۸� ��������� ���̴�. 
			std::shared_ptr<UEngineConstantBuffer> Buffer = UEngineConstantBuffer::CreateOrFind(BufferInfo.Size, UpperName);

			// ������ۿ� ������ ���ؼ� ���ڰ� �������� ������ �����ϱ� ���� Ŭ������
			UEngineConstantBufferRes NewRes;
			NewRes.ShaderType = ShaderType;
			NewRes.Name = UpperName;
			NewRes.BindIndex = ResDesc.BindPoint;
			NewRes.Res = Buffer;
			NewRes.BufferSize = BufferInfo.Size;

			ShaderResources.CreateConstantBufferRes(UpperName, NewRes);
			break;
		}
		case D3D_SIT_TEXTURE:
		{
			std::shared_ptr<UEngineTexture> Res = UEngineTexture::Find<UEngineTexture>("NSBase.png");

			UEngineTextureRes NewRes;
			NewRes.ShaderType = ShaderType;
			NewRes.Name = UpperName;
			NewRes.BindIndex = ResDesc.BindPoint;
			NewRes.Res = Res.get();

			ShaderResources.CreateTextureRes(UpperName, NewRes);

			break;
		}
		case D3D_SIT_SAMPLER:
		{
			std::shared_ptr<UEngineSampler> Res = UEngineSampler::Find<UEngineSampler>("WRapSampler");

			UEngineSamplerRes NewRes;
			NewRes.ShaderType = ShaderType;
			NewRes.Name = UpperName;
			NewRes.BindIndex = ResDesc.BindPoint;
			NewRes.Res = Res;

			ShaderResources.CreateSamplerRes(UpperName, NewRes);

			break;
		}
		case D3D_SIT_STRUCTURED:
		{
			int a = 0;

			break;
		}
		case D3D_SIT_UAV_RWSTRUCTURED: // ��ǻƮ
		{
			int a = 0;

			break;
		}
		default:
			break;
		}

		int a = 0;
	}

	// 
	EntryName;
	ShaderResources;

}



