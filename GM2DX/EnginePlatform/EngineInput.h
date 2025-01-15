#pragma once
#include <EngineBase/EngineDelegate.h>
#include <EngineBase/EngineDebug.h>

#include <vector>
#include <functional>

// �̱����� ����

// ������ ������ �����Ҷ� 4���� �η��� �������.
// ��������, xx����, xx����, ��������
// ���������� �̱����� �����Ҷ� ������ ���� �������.
// �̱����̶� ��ü�� ���α׷��� ��Ʋ� ���� 1���̻� ������ ���ϰ� �ϱ� ���� �����̴�.
// �� �̰� �̿��� 

// �̱��� �Ҷ� �ϸ� �ȵǴ� �� 
// 1. ���� ���ϰ� �����Ϸ��� �̱����� ��� => �ȵ���.

// 1. �����ڸ� private���� �����ϴ�.

// � Ŭ������ �������. 
// EngineWindow�� �����غ���. 
// ���⿡�� �̱����� �����ϰڴ�.
// ���� : 

enum class KeyEvent
{
	Down,
	Press,
	Free,
	Up,
};

class UEngineInput
{
public:
	// constrcuter destructer
	~UEngineInput();

	// delete Function
	UEngineInput(const UEngineInput& _Other) = delete;
	UEngineInput(UEngineInput&& _Other) noexcept = delete;
	UEngineInput& operator=(const UEngineInput& _Other) = delete;
	UEngineInput& operator=(UEngineInput&& _Other) noexcept = delete;


	// �ܺο� �����ϱ�� �ȴ�.
private:
	class UEngineKey
	{
	public:
		int Key = -1;

		// Ű�� �ȴ����ٰ� ó�� ��������
		bool IsDown = false;

		// Ű�� ���� ���ķ� ��� ������ ������
		bool IsPress = false;

		// Ű�� �����ٰ� ������
		bool IsUp = false;

		// Ű�� �ȴ����� ������
		bool IsFree = true;

		float PressTime = 0.0f;
		float FreeTime = 0.0f;

		std::vector<std::function<void()>> PressEvents;
		std::vector<std::function<void()>> DownEvents;
		std::vector<std::function<void()>> UpEvents;
		std::vector<std::function<void()>> FreeEvents;

		void Reset()
		{
			IsDown = false;
			IsPress = false;
			IsUp = false;
			IsFree = true;
			PressTime = 0.0f;
			FreeTime = 0.0f;
		}

		// ���ͳ� ����Ʈ�� ����ϸ� ������ ����.
		// �л����� ��ó�� ���ϴµ�
		// �ᱹ ������ �ö󰡸� �˴ϴ�.

		UEngineKey()
		{
		}

		UEngineKey(int _Key)
			: Key(_Key)
		{
		}

		void EventCheck();

		void KeyCheck(float _DeltaTime);
		
	};

	ENGINEAPI static UEngineInput& GetInst();
	ENGINEAPI static void EventCheck(float _DeltaTime);

public:
	ENGINEAPI static void KeyCheck(float _DeltaTime);

	ENGINEAPI static void KeyReset();

	// UEngineInput::GetInst().IsDown('A')

	static bool IsDoubleClick(int _KeyIndex, float _Time)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("������ ��ϵ��� ���� Ű�� �����մϴ�.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].IsDown && GetInst().Keys[_KeyIndex].FreeTime < _Time;
	}

	static bool IsDown(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("������ ��ϵ��� ���� Ű�� �����մϴ�.");
			return false;
		}

		// 
		// Keys[_KeyIndex] => ������ ���ο��� ��带 �����.
		// UMapNode
		// {
		//     UEngineKey Value = UEngineKey();
		// }

		return GetInst().Keys[_KeyIndex].IsDown;
	}

	static bool IsUp(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("������ ��ϵ��� ���� Ű�� �����մϴ�.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].IsUp;
	}

	static bool IsPress(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("������ ��ϵ��� ���� Ű�� �����մϴ�.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].IsPress;
	}

	static float IsPressTime(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("������ ��ϵ��� ���� Ű�� �����մϴ�.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].PressTime;
	}


	static bool IsFree(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("������ ��ϵ��� ���� Ű�� �����մϴ�.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].IsFree;
	}

	void BindAction(int _KeyIndex, KeyEvent _EventType,  std::function<void()> _Function);

protected:

private:
	// �̷��� ����
	// static ���� ���� �����ϰ� 
	// �����ʿ� ����.
	// static UEngineInput Inst;
	
	// ��������
	// static UEngineInput* Inst;

	// �̱����� ����
	// �̱��� ����� 1
	// 1. �����ڸ� private���� �����ϴ�.

	// 100�������� 
	std::map<int, UEngineKey> Keys;

	UEngineInput();

};

