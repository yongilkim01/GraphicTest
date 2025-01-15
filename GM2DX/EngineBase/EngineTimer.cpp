#include "PreCompile.h"
#include "EngineTimer.h"

UEngineTimer::UEngineTimer()
{
	// ���� 1�ʿ� �󸶸� ���� �ִ� �ֿ���.
	QueryPerformanceFrequency(&Count);

	// ������ ������ �ϱ� ���ؼ� 8����Ʈ 
	TimeCounter = static_cast<double>(Count.QuadPart);

	// ������� �̸�ŭ �����
	QueryPerformanceCounter(&PrevTime);
	QueryPerformanceCounter(&CurTime);

	int a = 0;
}

UEngineTimer::~UEngineTimer()
{
}

void UEngineTimer::TimeStart()
{
	QueryPerformanceCounter(&PrevTime);
}

float UEngineTimer::End()
{
	TimeCheck();
	return GetDeltaTime();
}

double UEngineTimer::DEnd()
{
	TimeCheck();
	return GetDoubleDeltaTime();
}

void UEngineTimer::TimeCheck()
{
	// QueryPerformanceCounter(&PrevTime); ������ ����� �ð��� �������� ���̴�.

	QueryPerformanceCounter(&CurTime);

	// ���̰����� ���ɴϴ�.
	double Tick = static_cast<double>(CurTime.QuadPart - PrevTime.QuadPart);
	DeltaTime = Tick / TimeCounter * TimeScale;
	fDeltaTime = static_cast<float>(DeltaTime);
	PrevTime.QuadPart = CurTime.QuadPart;
}
