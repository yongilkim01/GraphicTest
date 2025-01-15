#pragma once

// ���� :
class UEngineInputLayOutInfo
{
public:
	// constrcuter destructer
	UEngineInputLayOutInfo();
	~UEngineInputLayOutInfo();

	// delete Function
	UEngineInputLayOutInfo(const UEngineInputLayOutInfo& _Other) = delete;
	UEngineInputLayOutInfo(UEngineInputLayOutInfo&& _Other) noexcept = delete;
	UEngineInputLayOutInfo& operator=(const UEngineInputLayOutInfo& _Other) = delete;
	UEngineInputLayOutInfo& operator=(UEngineInputLayOutInfo&& _Other) noexcept = delete;


	void AddInputLayout(
		LPCSTR _SemanticName, // ���� �������̴� ���� �÷���. "POSITION"
		DXGI_FORMAT _Format, // ���� n����Ʈ ¥�� ������ �ڷ���
		D3D11_INPUT_CLASSIFICATION _InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA, // ���ؽ� �����Ͱ� �ν��Ͻ� �����ʹ� ��°� ǥ��
		UINT _InstanceDataStepRate = 0, // ���߿� ���߿� �ν��Ͻ��̶�� ������ ��ﶧ ������ �������̴�.
		UINT _AlignedByteOffset = -1, // 0��° ����Ʈ���� 
		UINT _InputSlot = 0, // n���� ���ؽ� ���۸� �����Ҷ� n��° ���ؽ� ������ ��ǲ ���̾ƿ��̴� ��°� ǥ���ϴ°�
		UINT _SemanticIndex = 0
	);

	UINT FormatSize(DXGI_FORMAT _Value);

	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayOutData;

protected:

private:
	int Offset = 0;
};

