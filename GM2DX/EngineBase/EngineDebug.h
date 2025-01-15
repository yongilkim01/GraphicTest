#pragma once
#include "EngineDefine.h"
#include <Windows.h>
#include <assert.h>


#define MSGASSERT(VALUE) std::string ErrorText = VALUE; MessageBoxA(nullptr, ErrorText.c_str(), "ġ���� ����", MB_OK); assert(false);

namespace UEngineDebug
{
	ENGINEAPI void LeakCheck();

	// �и��ؾ� �ϰڳ׿�
	ENGINEAPI void OutPutString(const std::string& Text);


	ENGINEAPI void StartConsole();

	// AllocConsole();
	// ���� �ܼ�â�� ����� �Է� ��� �ڵ��� ���� ������ ������ �ٲ��.
	// �׷��� �츮�� ���� ��Ƽ����Ʈ�� ó���ߴ�.
	// �Ʒ��Լ� �Ἥ �ܼ� ������� ��������� �ϴµ�.
	// _tfreopen_s �Լ��� ����Ǿ��µ� ������ ���� �������� ����.
	// ���ͳ� �˻��� ���ؼ� �˾Ƴ��� �Ұ� ����.
	// �Ʒ� �Լ��� ����ϴ¹��� �� �����ϰ� �;��Ұ� ����.
	//_tfreopen(_T("CONOUT$"), _T("w"), stdout);
	//_tfreopen(_T("CONIN$"), _T("r"), stdin);
	//_tfreopen(_T("CONERR$"), _T("w"), stderr);
	ENGINEAPI void EndConsole();
	// FreeConsole();

}

