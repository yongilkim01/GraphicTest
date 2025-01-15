#include "PreCompile.h"
#include "EngineCore.h"
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>
#include <EnginePlatform/EngineInput.h>
#include "IContentsCore.h"
#include "EngineResources.h"
#include "EngineConstantBuffer.h"
#include "EngineGUI.h"
#include "Level.h"


UEngineGraphicDevice& UEngineCore::GetDevice()
{
	return GEngine->Device;
}

UEngineWindow& UEngineCore::GetMainWindow()
{
	return GEngine->MainWindow;
}

std::map<std::string, std::shared_ptr<class ULevel>> UEngineCore::GetAllLevelMap()
{
	return GEngine->LevelMap;
}

UEngineCore* GEngine = nullptr;

FVector UEngineCore::GetScreenScale()
{
	return GEngine->Data.WindowSize;
}

UEngineCore::UEngineCore()
{
}

UEngineCore::~UEngineCore()
{
}

void UEngineCore::WindowInit(HINSTANCE _Instance)
{
	UEngineWindow::EngineWindowInit(_Instance);
	GEngine->MainWindow.Open("MainWindow");
}

void UEngineCore::LoadContents(std::string_view _DllName)
{
	UEngineDirectory Dir;

	Dir.MoveParentToDirectory("Build");
	Dir.Move("bin/x64");

	// ���� ��Ȳ�� ���� ��� ����
#ifdef _DEBUG
	Dir.Move("Debug");
#else
	Dir.Move("Release");
#endif

	UEngineFile File = Dir.GetFile(_DllName);

	std::string FullPath = File.GetPathToString();
	// ��Ģ�� ������ۿ� ����.
	GEngine->ContentsDLL = LoadLibraryA(FullPath.c_str());

	if (nullptr == GEngine->ContentsDLL)
	{
		MSGASSERT("������ ����� �ε��Ҽ��� �����ϴ�.");
		return;
	}

	INT_PTR(__stdcall * Ptr)(std::shared_ptr<IContentsCore>&) = (INT_PTR(__stdcall*)(std::shared_ptr<IContentsCore>&))GetProcAddress(GEngine->ContentsDLL, "CreateContentsCore");

	if (nullptr == Ptr)
	{
		MSGASSERT("������ �ھ� ���ο� CreateContentsCoreDefine�� �������� �ʾҽ��ϴ�.");
		return;
	}

	Ptr(GEngine->Core);

	if (nullptr == GEngine->Core)
	{
		MSGASSERT("������ �ھ� ������ �����߽��ϴ�.");
		return;
	}
}

void UEngineCore::EngineStart(HINSTANCE _Instance, std::string_view _DllName)
{
	UEngineDebug::LeakCheck();

	UEngineCore EngineCore;

	GEngine = &EngineCore;

	WindowInit(_Instance);

	LoadContents(_DllName);

	// ������ʹ� �����մϴ�.
	UEngineWindow::WindowMessageLoop(
		[]()
		{
			// ��򰡿��� �̰� ȣ���ϸ� �ܼ�â�� �߰� �� �ڷδ� std::cout �ϸ� �� �ܼ�â�� �޼����� ��̴ϴ�.
			// UEngineDebug::StartConsole();
			// ���� ����̽� �����
			GEngine->Device.CreateDeviceAndContext();
			// �ε��ϰ�
			GEngine->Core->EngineStart(GEngine->Data);
			// ������ �����Ҽ� �ִ�.
			GEngine->MainWindow.SetWindowPosAndScale(GEngine->Data.WindowPos, GEngine->Data.WindowSize);
			GEngine->Device.CreateBackBuffer(GEngine->MainWindow);
			// ����̽��� ��������� ������ ���ҽ� �ε嵵 �Ҽ��� ����.
			// ������� ���ҽ� �ε尡 �����ϴ�.
			
			UEngineGUI::Init();
		},
		[]()
		{
			EngineFrame();
			// ������ ���ư��� �ϰ� ������
		},
		[]()
		{
			// static���� ���ڰ� �߽��ϴ�.
			// �̶� ������ �� delete�� �Ǿ�� �Ѵ�.
			// ���۷��� ī��Ʈ�� �����Ǹ� �� ���۷��� ī��Ʈ�� ���� ���� �־��.
			EngineEnd();
		});


	// ���� ������ ���۵Ǿ���.
	// ������â�� ������ �˾Ƽ� ������ �ϰ�.

	// Window ������ �Ѵ�.

	
}

// ��� ��ȯ ������ ���� ���� �Լ��и�
std::shared_ptr<ULevel> UEngineCore::NewLevelCreate(std::string_view _Name)
{
	if (true == GEngine->LevelMap.contains(_Name.data()))
	{
		MSGASSERT("���� �̸��� ������ �� ������� �����ϴ�." + std::string( _Name.data()));
		return nullptr;
	}

	// ����⸸ �ϰ� ������ ���ϸ� �� �׳� �������ϴ�. <= 
	
	// ����� �ʿ� �־ ���۷��� ī��Ʈ�� ������ŵ�ϴ�.
	// UObject�� ����̾����ϴ�.
	std::shared_ptr<ULevel> Ptr = std::make_shared<ULevel>();
	Ptr->SetName(_Name);

	GEngine->LevelMap.insert({ _Name.data(), Ptr});

	std::cout << "NewLevelCreate" << std::endl;

	return Ptr;
}

void UEngineCore::OpenLevel(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == GEngine->LevelMap.contains(UpperString))
	{
		MSGASSERT("������ ���� ������ �����Ϸ��� �߽��ϴ�." + UpperString);
		return;
	}
	

	GEngine->NextLevel = GEngine->LevelMap[UpperString];
}

void UEngineCore::EngineFrame()
{
	if (nullptr != GEngine->NextLevel)
	{
		if (nullptr != GEngine->CurLevel)
		{
			GEngine->CurLevel->LevelChangeEnd();
		}

		GEngine->CurLevel = GEngine->NextLevel;

		GEngine->CurLevel->LevelChangeStart();
		GEngine->NextLevel = nullptr;
		GEngine->Timer.TimeStart();
	}

	GEngine->Timer.TimeCheck();
	float DeltaTime = GEngine->Timer.GetDeltaTime();
	if (true == GEngine->MainWindow.IsFocus())
	{
		UEngineInput::KeyCheck(DeltaTime);
	}
	else {
		UEngineInput::KeyReset();
	}
	
	GEngine->CurLevel->Tick(DeltaTime);
	GEngine->CurLevel->Render(DeltaTime);
	// GUI�������� ���� �������� �� ������ ���ִ°� ����.
	// ����Ʈ���μ���
	// �ݸ���
	GEngine->CurLevel->Collision(DeltaTime);


	GEngine->CurLevel->Release(DeltaTime);
}

void UEngineCore::EngineEnd()
{

	UEngineGUI::Release();

	// ���ҽ� ������ ���⼭ �Ұ̴ϴ�.
	GEngine->Device.Release();

	UEngineResources::Release();
	UEngineConstantBuffer::Release();

	GEngine->CurLevel = nullptr;
	GEngine->NextLevel = nullptr;
	GEngine->LevelMap.clear();

	UEngineDebug::EndConsole();

}