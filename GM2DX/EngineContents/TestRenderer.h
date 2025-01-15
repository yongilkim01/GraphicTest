#pragma once
#include <EngineCore/Renderer.h>

struct FMyColor
{
	float4 Albedo;
};


/**
 *	����
 */
class TestRenderer : public URenderer
{
public:
	/** ������, �Ҹ��� */
	TestRenderer();
	~TestRenderer();

	/** ��ü �� ���� ���� */
	TestRenderer(const TestRenderer& _Other) = delete;
	TestRenderer(TestRenderer&& _Other) noexcept = delete;
	TestRenderer& operator=(const TestRenderer& _Other) = delete;
	TestRenderer& operator=(TestRenderer&& _Other) noexcept = delete;

protected:

private:
	FMyColor MyColor;

};

