#include "PreCompile.h"
#include "EngineDebug.h"


namespace UEngineDebug
{
	void LeakCheck()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	// �и��ؾ� �ϰڳ׿�
	void OutPutString(const std::string& Text)
	{
		std::string ResultText = Text + "\n";
		OutputDebugStringA(ResultText.c_str());
	}

	ENGINEAPI void StartConsole()
	{
		AllocConsole();
		FILE* pCout = nullptr;
		FILE* pErr = nullptr;
		FILE* pCin = nullptr;
		errno_t Err;

		// �ܼ�â�� �⺻ ��¹��۴� stdout�̶�� ������ ������ �޶�
		// &pCout ���� ���� ����Ʈ ����� ���۴� pCout�� �����.
		// �Է� �� ��¹��۴� FILE*�� ǥ���մϴ�.
		// &pCout
		// __acrt_iob_func(1) ���� ����ϰ� �ִ� ���α׷��� �Է¹���(FILE*)�� �������� �Լ��ΰ� �����ϴ�.
		// OS ��α׿��� �ý��ϴ�.
		Err = freopen_s(&pCout, "CONOUT$", "w", stdout);
		if (pCout != stdout)
		{
			fclose(pCout);
		}
		Err = freopen_s(&pCin, "CONIN$", "r", stdin);
		if (pCin != stdin)
		{
			fclose(pCin);
		}
		Err = freopen_s(&pErr, "CONERR$", "w", stderr);
		if (pErr != stderr)
		{
			fclose(pErr);
		}

		// fclose(pCout);
		// fclose(pCin);
		// fclose(pErr);
		// std::cout << "Console Log Start" << std::endl;
	}

	ENGINEAPI void EndConsole()
	{
		FreeConsole();
	}
}