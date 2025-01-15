#pragma once
#include <string>
#include <memory>
#include "EngineSerializer.h"

// ���� : std::enable_shared_from_this<UObject>
class UObject : public std::enable_shared_from_this<UObject>, public ISerializObject
{
public:
	// constrcuter destructer
	ENGINEAPI UObject();

	// Ȥ���� �ڽĵ��� �Ҹ��ڰ� ȣ�� �ȵǴ� ��츦 ����������
	// �����Լ� ���̺��� ���� ���̹Ƿ� �ظ��ϸ� �ڽ����� �Ҹ��ڰ� ȣ��ȵǴ� ���� ���� ���̴�.
	ENGINEAPI virtual ~UObject();

	// delete Function
	UObject(const UObject& _Other) = delete;
	UObject(UObject&& _Other) noexcept = delete;
	UObject& operator=(const UObject& _Other) = delete;
	UObject& operator=(UObject&& _Other) noexcept = delete;

	template<typename ChildPtrType>
	std::shared_ptr<ChildPtrType> GetThis()
	{
		return std::static_pointer_cast<ChildPtrType>(shared_from_this());
	}

	std::string GetName() const
	{
		return Name;
	}

	std::string_view GetNameView() const
	{
		return Name.c_str();
	}

	// �̸� �����Ҷ� ���� �ϰ� ������ �������̵���.
	ENGINEAPI virtual void SetName(std::string_view _Name)
	{
		Name = _Name.data();
	}

	// bool IsActive()
	virtual bool IsActive()
	{
		return IsActiveValue && false == IsDestroyValue;
	}

	virtual bool IsDestroy()
	{
		return IsDestroyValue;
	}

	// �ٷ� �װ���?
	// _Time �ð��Ŀ� �׾��.
	void Destroy(float _Time = 0.0f)
	{
		DeathTime = _Time;

		if (0.0f < _Time)
		{
			IsDeathTimeCheck = true;
			return;
		}

		IsDestroyValue = true;
	}

	virtual void ReleaseTimeCheck(float _DeltaTime)
	{
		if (false == IsDeathTimeCheck)
		{
			return;
		}

		CurDeathTime += _DeltaTime;

		if (DeathTime <= CurDeathTime)
		{
			IsDestroyValue = true;
		}
	}

	virtual void ReleaseCheck(float _DeltaTime)
	{

	}

	// ��� ��� ����.
	// ���� �ܺο��� �ٸ� ��ü�� Ǯ����� �Ѵ�.
	void SetActive(bool _IsActive)
	{
		IsActiveValue = _IsActive;
	}

	void SetActiveSwitch()
	{
		IsActiveValue = !IsActiveValue;
	}

	bool IsDebug()
	{
		return IsDebugValue;
	}

	void DebugOn()
	{
		IsDebugValue = true;
	}

	void DebugOff()
	{
		IsDebugValue = false;
	}

	void DebugSwitch()
	{
		IsDebugValue = !IsDebugValue;
	}

	int GetOrder()
	{
		return Order;
	}

	virtual void SetOrder(int _Order)
	{
		Order = _Order;
	}

	bool& GetIsActiveValueRef()
	{
		return IsActiveValue;
	}

	

protected:

private:
	int Order = 0;

	bool IsDestroyValue = false;
	bool IsActiveValue = true;

	bool IsDeathTimeCheck = false;
	float DeathTime = 0.0f;
	float CurDeathTime = 0.0f;

	std::string Name;

	bool IsDebugValue = false;
};

