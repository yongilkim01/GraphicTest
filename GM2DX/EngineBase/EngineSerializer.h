#pragma once
#include <vector>
#include <string>
#include "EngineMath.h"

// ���� :
class UEngineSerializer
{
public:
	// constrcuter destructer
	ENGINEAPI UEngineSerializer();
	ENGINEAPI ~UEngineSerializer();

	// delete Function
	//UEngineSerializer(const UEngineSerializer& _Other) = delete;
	//UEngineSerializer(UEngineSerializer&& _Other) noexcept = delete;
	//UEngineSerializer& operator=(const UEngineSerializer& _Other) = delete;
	//UEngineSerializer& operator=(UEngineSerializer&& _Other) noexcept = delete;

	// �������� ũ��
	ENGINEAPI void Write(const void* _Data, unsigned int _Size);

	void operator<<(const int& _Data)
	{
		Write(&_Data, sizeof(int));
	}

	void operator<<(const bool& _Data)
	{
		Write(&_Data, sizeof(bool));
	}

	void operator<<(const FVector& _Data)
	{
		Write(&_Data, sizeof(FVector));
	}

	void operator<<(const FIntPoint& _Data)
	{
		Write(&_Data, sizeof(FIntPoint));
	}

	void operator<<(const std::string& _Data)
	{
		// int�ϳ��� ������ �ϳ� ��� ����?
		// ���̵� ���� �����ؾ� �Ѵ�.
		int Size = static_cast<int>(_Data.size());
		operator<<(Size);
		if (0 != Size)
		{
			Write(&_Data[0], static_cast<int>(_Data.size()));
		}
	}

	void operator<<(class ISerializObject& _Data);

	template<typename DataType>
	void operator<<(std::vector<DataType>& _vector)
	{
		int Size = static_cast<int>(_vector.size());
		operator<<(Size);

		for (size_t i = 0; i < _vector.size(); i++)
		{
			// �ڷ��� ��ȯ�� �ȵȴٴ� ���̴�.
			operator<<(_vector[i]);
		}
	}

	ENGINEAPI void Read(void* _Data, unsigned int _Size);

	void operator>>(int& _Data)
	{
		Read(&_Data, sizeof(int));
	}

	void operator>>(bool& _Data)
	{
		Read(&_Data, sizeof(bool));
	}

	void operator>>(FVector& _Data)
	{
		Read(&_Data, sizeof(FVector));
	}

	void operator>>(FIntPoint& _Data)
	{
		Read(&_Data, sizeof(FIntPoint));
	}

	void operator>>(std::string& _Data)
	{
		// int�ϳ��� ������ �ϳ� ��� ����?
		// ���̵� ���� �����ؾ� �Ѵ�.
		int Size;
		operator>>(Size);
		// ����� �о��.
		_Data.resize(Size);

		Read(&_Data[0], static_cast<int>(_Data.size()));
	}

	void operator>>(class ISerializObject& _Data);

	template<typename DataType>
	void operator>>(std::vector<DataType>& _vector)
	{
		int Size = 0;
		operator>>(Size);
		_vector.resize(Size);

		for (size_t i = 0; i < _vector.size(); i++)
		{
			// �ڷ��� ��ȯ�� �ȵȴٴ� ���̴�.
			operator>>(_vector[i]);
		}
	}

	void* GetDataPtr()
	{
		return &Data[0];
	}

	int GetWriteOffset()
	{
		return WriteOffset;
	}

	void DataResize(int _Value)
	{
		Data.resize(_Value);
	}

	size_t GetDataSize()
	{
		return Data.size();
	}

protected:

private:
	// ��� �����ʹ� ����Ʈ ����̴�.
	// �״�� Ŭ������ ��쿡�� �߰��� ����Ʈ �е������ ������ 
	// ������� �����.

	// �󸶳� �����Ͱ� ä������?
	int WriteOffset = 0;

	// �󸶳� �����Ͱ� ä������?
	int ReadOffset = 0;

	// �ڷ����̶�°� �������� ����?
	std::vector<char> Data;
};

// �𸮾� ���� �������̽���� �Ҹ��� ���縦 ���鶧
// �տ� I�� ���Դϴ�.
class ISerializObject
{
	// �������̽��� ����?
	// 1. �ɹ������� ����
	// ũ�⸦ ������ �ʾ�. �����Լ� ���̺��� ũ�⸸ ������ ���.
	// 2. ������ ����
	// ������ �Լ��� �����ҰŴٶ�� �������̽��θ� �̷���� �ִ�.
	// �� ������ �����ϸ� 
	// ���߻�ӿ� �δ��� ����. 
	// *. C++�� ġ�ڸ� ������ ���������Լ��θ� ������ Ŭ������ 
	// �𸮾��� �ƴմϴ�. ���������Լ��� ���Ƽ� �����ŵ��.
	// 3. ���� �����Լ����� Private�� �����Ҽ� ����.

public:
	ENGINEAPI virtual ~ISerializObject() = 0
	{

	}

public:
	// �����͸� ����ȭ(����)
	ENGINEAPI virtual void Serialize(UEngineSerializer& _Ser);
	// �����͸� ����(�Ҷ�)
	ENGINEAPI virtual void DeSerialize(UEngineSerializer& _Ser);
};

//class Test
//{
//	// ���Ѵٰ� �ϸ�
//	// Data 5 ����Ʈ �Ҵ�
//	//             [C][V][V][V][V]
//	// ����ȭ�� ù��° �ǹ�
//	// �������� ����ȭ
//
//	bool Check; // [C][ ][ ][ ][v][v][v][v]
//	int Value;  // 
//};
