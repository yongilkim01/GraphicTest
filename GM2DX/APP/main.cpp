// ���־� �ڵ� ����.
#include <EngineBase/EngineMath.h>
#include <EngineCore/EngineCore.h>

// ������Ʈ �� �������
// ���� ���� �������� �ϰڴ�.
#pragma comment(lib, "EngineBase.lib")
#pragma comment(lib, "EnginePlatform.lib")
#pragma comment(lib, "EngineCore.lib")

// �̳༮�� �����ε����� ������� �ʰڴ�.
// #pragma comment(lib, "EngineContents.lib")6

// �������� Level���� �����
// �������� ���� ������ Create�ϴ� ������ ����.

int APIENTRY wWinMain(_In_ HINSTANCE _hInstance,
	_In_opt_ HINSTANCE _hPrevInstance,
	_In_ LPWSTR    _lpCmdLine,
	_In_ int       _nCmdShow)
{
	// ���� ������ ���� "EngineContents" dll�̸��� �־���� �Ѵ�.
	UEngineCore::EngineStart(_hInstance, "EngineContents.dll");
	return 0;
}