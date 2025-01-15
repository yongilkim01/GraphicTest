#pragma once
#include <EngineCore/Renderer.h>

/**
 *	설명
 */
class TestRenderer : public URenderer
{
public:
	/** 생성자, 소멸자 */
	TestRenderer();
	~TestRenderer();

	/** 객체 값 복사 방지 */
	TestRenderer(const TestRenderer& _Other) = delete;
	TestRenderer(TestRenderer&& _Other) noexcept = delete;
	TestRenderer& operator=(const TestRenderer& _Other) = delete;
	TestRenderer& operator=(TestRenderer&& _Other) noexcept = delete;

protected:

private:

};

