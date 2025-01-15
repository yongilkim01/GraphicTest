#include "PreCompile.h"
#include "TestRenderer.h"

TestRenderer::TestRenderer()
{
	for (int i = 0; i < 8; i++)
	{
		CreateRenderUnit();
		SetMesh("Triangle" + std::to_string(i + 1), i);
		SetMaterial("MyMaterial", i);
	}
}

TestRenderer::~TestRenderer()
{
}

