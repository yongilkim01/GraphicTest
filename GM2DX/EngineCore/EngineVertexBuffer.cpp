#include "PreCompile.h"
#include "EngineVertexBuffer.h"

UEngineVertexBuffer::UEngineVertexBuffer()
{
}

UEngineVertexBuffer::~UEngineVertexBuffer()
{
}

std::shared_ptr<UEngineVertexBuffer> UEngineVertexBuffer::Create(std::string_view _Name, const void* _InitData, size_t _VertexSize, size_t _VertexCount,
	UEngineInputLayOutInfo* _InfoPtr)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� �ؽ�ó�� �� �ε��Ϸ��� �߽��ϴ�." + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineVertexBuffer> NewRes = std::make_shared<UEngineVertexBuffer>();
	PushRes<UEngineVertexBuffer>(NewRes, _Name, "");
	NewRes->ResCreate(_InitData, _VertexSize, _VertexCount);
	NewRes->InfoPtr = _InfoPtr;
	

	return NewRes;
}

void UEngineVertexBuffer::ResCreate(const void* _InitData, size_t _VertexSize, size_t _VertexCount)
{
	VertexCount = static_cast<UINT>(_VertexCount);
	VertexSize = static_cast<UINT>(_VertexSize);
	BufferInfo.ByteWidth = static_cast<UINT>(_VertexSize * _VertexCount);
	BufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferInfo.CPUAccessFlags = 0;
	BufferInfo.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA Data; // �ʱⰪ �־��ִ� �뵵�� ����ü
	Data.pSysMem = _InitData;

	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, &Data, &Buffer))
	{
		MSGASSERT("���ؽ� ���� ������ �����߽��ϴ�.");
		return;
	}
}

void UEngineVertexBuffer::Setting()
{
	// UINT VertexSize = sizeof(EngineVertex);
	// �� ���ؽ� ���۰� 10��¥���ε� 3��° ���ؽ� ���� ��������.
	UINT Offset = 0;
	ID3D11Buffer* ArrBuffer[1];
	ArrBuffer[0] = Buffer.Get();
	UEngineCore::GetDevice().GetContext()->IASetVertexBuffers(0, 1, ArrBuffer, &VertexSize, &Offset);
	
}