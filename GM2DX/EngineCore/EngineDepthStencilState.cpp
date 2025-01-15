#include "PreCompile.h"
#include "EngineDepthStencilState.h"
#include "EngineCore.h"

UEngineDepthStencilState::UEngineDepthStencilState()
{
}

UEngineDepthStencilState::~UEngineDepthStencilState()
{
}



std::shared_ptr<UEngineDepthStencilState> UEngineDepthStencilState::Create(std::string_view _Name, const D3D11_DEPTH_STENCIL_DESC& _Value)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� �ؽ�ó�� �� �ε��Ϸ��� �߽��ϴ�." + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineDepthStencilState> NewRes = std::make_shared<UEngineDepthStencilState>();
	PushRes<UEngineDepthStencilState>(NewRes, _Name, "");
	NewRes->ResCreate(_Value);

	return NewRes;
}


void UEngineDepthStencilState::ResCreate(const D3D11_DEPTH_STENCIL_DESC& _Value)
{
	if (UEngineCore::GetDevice().GetDevice()->CreateDepthStencilState(&_Value, &State))
	{
		MSGASSERT("���� ������Ʈ ������ �����߽��ϴ�");
		return;
	}
	
}

void UEngineDepthStencilState::Setting()
{
	// Omset ���� �ȼ��׽�Ʈ�� ������ �ݴϴ�. 
	UEngineCore::GetDevice().GetContext()->OMSetDepthStencilState(State.Get(), 0);
}
