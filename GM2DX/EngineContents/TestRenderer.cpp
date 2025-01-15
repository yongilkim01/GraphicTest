#include "PreCompile.h"
#include "TestRenderer.h"

TestRenderer::TestRenderer()
{
	CreateRenderUnit();
	SetMesh("Diamond");
	SetMaterial("MyMaterial");

	MyColor.Albedo = float4(1.0f, 1.0f, 1.0f, 1.0f);
	GetRenderUnit().ConstantBufferLinkData("FMyColor", MyColor);
}

TestRenderer::~TestRenderer()
{
}

