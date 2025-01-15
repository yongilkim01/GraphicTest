#include "PreCompile.h"
#include "EngineGraphicDevice.h"
#include "EngineVertex.h"
#include "EngineVertexBuffer.h"
#include "EngineIndexBuffer.h"
#include "Mesh.h"
#include "EngineBlend.h"
#include "EngineShader.h"
#include "EngineMaterial.h"
#include "EngineTexture.h"
#include "EngineFont.h"
#include "EngineDepthStencilState.h"

void UEngineGraphicDevice::DefaultResourcesInit()
{
	DepthStencilInit();
	TextureInit();
	MeshInit();
	BlendInit();
	RasterizerStateInit();
	ShaderInit();
	MaterialInit();

	// 직접 자기 폰트 
	UEngineFont::Load("궁서", "궁서");
}

void UEngineGraphicDevice::DepthStencilInit()
{
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// 깊이값이 더 작으면 통과시켜
		Desc.DepthFunc = D3D11_COMPARISON_LESS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		UEngineDepthStencilState::Create("BaseDepth", Desc);
	}

	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = false;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// 깊이값이 더 작으면 통과시켜
		Desc.DepthFunc = D3D11_COMPARISON_LESS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		UEngineDepthStencilState::Create("UIDepth", Desc);
	}


	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// 깊이값이 더 작으면 통과시켜
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		UEngineDepthStencilState::Create("CollisionDebugDepth", Desc);
	}

	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// 깊이값이 더 작으면 통과시켜
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		UEngineDepthStencilState::Create("TargetMerge", Desc);
	}

}

void UEngineGraphicDevice::TextureInit()
{

		D3D11_SAMPLER_DESC SampInfo = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT };
		SampInfo.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP; // 0~1사이만 유효
		SampInfo.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP; // y
		SampInfo.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP; // z // 3중 

		SampInfo.BorderColor[0] = 0.0f;
		SampInfo.BorderColor[1] = 0.0f;
		SampInfo.BorderColor[2] = 0.0f;
		SampInfo.BorderColor[3] = 0.0f;

		// SampInfo.ComparisonFunc = D3D11_COMPARISON_NEVER;
		// Lod라고 불리는 것은 z값이 얼마나 멀어지면 얼마나 대충 색깔 빼올거냐. 
		// SampInfo.MaxLOD = 0.0f;
		// SampInfo.MinLOD = 0.0f;

		UEngineSampler::Create("WRAPSampler", SampInfo);


	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("EngineShader"))
		{
			MSGASSERT("EngineShader 폴더를 찾지 못했습니다.");
			return;
		}
		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineTexture::Load(FilePath);
		}
	}
}

void UEngineGraphicDevice::ShaderInit()
{
	UEngineDirectory CurDir;
	CurDir.MoveParentToDirectory("EngineShader");

	std::vector<UEngineFile> ShaderFiles = CurDir.GetAllFile(true, {".fx", ".hlsl"});

	for (size_t i = 0; i < ShaderFiles.size(); i++)
	{
		UEngineShader::ReflectionCompile(ShaderFiles[i]);
	}
}



void UEngineGraphicDevice::MeshInit()
{
	int a = 0;

	{
		std::vector<FEngineVertex> Vertexs;
		Vertexs.resize(4);
		Vertexs[0] = FEngineVertex{ FVector(-0.5f, 0.5f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = FEngineVertex{ FVector(0.5f, 0.5f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = FEngineVertex{ FVector(-0.5f, -0.5f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
		Vertexs[3] = FEngineVertex{ FVector(0.5f, -0.5f, 0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };

		UEngineVertexBuffer::Create("Rect", Vertexs);
	}

	{
		std::vector<unsigned int> Indexs;

		Indexs.push_back(0);
		Indexs.push_back(1);
		Indexs.push_back(2);

		Indexs.push_back(1);
		Indexs.push_back(3);
		Indexs.push_back(2);
		UEngineIndexBuffer::Create("Rect", Indexs);
	}

	{
		std::vector<FEngineVertex> Vertexs;
		Vertexs.resize(4);
		Vertexs[0] = FEngineVertex{ FVector(-1.0f, 1.0f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = FEngineVertex{ FVector(1.0f, 1.0f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = FEngineVertex{ FVector(-1.0f, -1.0f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
		Vertexs[3] = FEngineVertex{ FVector(1.0f, -1.0f, 0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };

		UEngineVertexBuffer::Create("FullRect", Vertexs);
	}

	{
		UMesh::Create("Rect");
		// FullRect 포스트프로세싱용 화면 전체크기 만한 매쉬를 제작.
		UMesh::Create("FullRect", "FullRect", "Rect");
	}

	// Triangle 1 (빨간색)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // 삼각형 1의 각 꼭짓점에 빨간색 적용
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, 0.5f), {0.5f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f} }; // 빨간색
    Vertexs[1] = FEngineVertex{ FVector(0.0f, -1.0f, 0.0f), {0.5f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} }; // 빨간색
    Vertexs[2] = FEngineVertex{ FVector(0.5f, 0.0f, 0.0f), {1.0f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} }; // 빨간색

    UEngineVertexBuffer::Create("Triangle1", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // 상단 꼭짓점
    Indices.push_back(1); // 중심선 앞쪽
    Indices.push_back(2); // 중심선 오른쪽

    UEngineIndexBuffer::Create("Triangle1", Indices);
    UMesh::Create("Triangle1");
}

// Triangle 2 (핑크색)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // 삼각형 2의 각 꼭짓점에 핑크색 적용
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, 0.5f), {0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} }; // 핑크색
    Vertexs[1] = FEngineVertex{ FVector(0.5f, 0.0f, 0.0f), {1.0f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }; // 핑크색
    Vertexs[2] = FEngineVertex{ FVector(0.0f, 1.0f, 0.0f), {0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }; // 핑크색

    UEngineVertexBuffer::Create("Triangle2", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // 상단 꼭짓점
    Indices.push_back(2); // 중심선 앞쪽
    Indices.push_back(1); // 중심선 오른쪽

    UEngineIndexBuffer::Create("Triangle2", Indices);
    UMesh::Create("Triangle2");
}

// Triangle 3 (녹색)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // 삼각형 3의 각 꼭짓점에 녹색 적용
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, 0.5f), {0.5f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} }; // 녹색
    Vertexs[1] = FEngineVertex{ FVector(0.0f, 1.0f, 0.0f), {0.5f, 0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} }; // 녹색
    Vertexs[2] = FEngineVertex{ FVector(-0.5f, 0.0f, 0.0f), {0.0f, 0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} }; // 녹색

    UEngineVertexBuffer::Create("Triangle3", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // 상단 꼭짓점
    Indices.push_back(1); // 중심선 앞쪽
    Indices.push_back(2); // 중심선 오른쪽

    UEngineIndexBuffer::Create("Triangle3", Indices);
    UMesh::Create("Triangle3");
}

// Triangle 4 (노란색)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // 삼각형 4의 각 꼭짓점에 노란색 적용
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, 0.5f), {0.5f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} }; // 노란색
    Vertexs[1] = FEngineVertex{ FVector(-0.5f, 0.0f, 0.0f), {0.0f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }; // 노란색
    Vertexs[2] = FEngineVertex{ FVector(0.0f, -1.0f, 0.0f), {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }; // 노란색

    UEngineVertexBuffer::Create("Triangle4", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // 상단 꼭짓점
    Indices.push_back(1); // 중심선 앞쪽
    Indices.push_back(2); // 중심선 오른쪽

    UEngineIndexBuffer::Create("Triangle4", Indices);
    UMesh::Create("Triangle4");
}

// Triangle 5 (빨간색)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // 삼각형 5의 각 꼭짓점에 빨간색 적용
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, -0.5f), {0.5f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} }; // 빨간색
    Vertexs[1] = FEngineVertex{ FVector(0.0f, -1.0f, 0.0f), {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }; // 빨간색
    Vertexs[2] = FEngineVertex{ FVector(0.5f, 0.0f, 0.0f), {1.0f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }; // 빨간색

    UEngineVertexBuffer::Create("Triangle5", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // 하단 꼭짓점
    Indices.push_back(1); // 중심선 앞쪽
    Indices.push_back(2); // 중심선 오른쪽

    UEngineIndexBuffer::Create("Triangle5", Indices);
    UMesh::Create("Triangle5");
}

// Triangle 6 (핑크색)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // 삼각형 6의 각 꼭짓점에 핑크색 적용
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, -0.5f), {0.5f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} }; // 핑크색
    Vertexs[1] = FEngineVertex{ FVector(0.5f, 0.0f, 0.0f), {1.0f, 0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} }; // 핑크색
    Vertexs[2] = FEngineVertex{ FVector(0.0f, 1.0f, 0.0f), {0.5f, 0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} }; // 핑크색

    UEngineVertexBuffer::Create("Triangle6", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // 하단 꼭짓점
    Indices.push_back(2); // 중심선 앞쪽
    Indices.push_back(1); // 중심선 오른쪽

    UEngineIndexBuffer::Create("Triangle6", Indices);
    UMesh::Create("Triangle6");
}

// Triangle 7 (녹색)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // 삼각형 7의 각 꼭짓점에 녹색 적용
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, -0.5f), {0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} }; // 녹색
    Vertexs[1] = FEngineVertex{ FVector(0.0f, 1.0f, 0.0f), {0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }; // 녹색
    Vertexs[2] = FEngineVertex{ FVector(-0.5f, 0.0f, 0.0f), {0.0f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }; // 녹색

    UEngineVertexBuffer::Create("Triangle7", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // 하단 꼭짓점
    Indices.push_back(1); // 중심선 앞쪽
    Indices.push_back(2); // 중심선 오른쪽

    UEngineIndexBuffer::Create("Triangle7", Indices);
    UMesh::Create("Triangle7");
}

// Triangle 8 (노란색)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // 삼각형 8의 각 꼭짓점에 노란색 적용
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, -0.5f), {0.5f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f} }; // 노란색
    Vertexs[1] = FEngineVertex{ FVector(-0.5f, 0.0f, 0.0f), {0.0f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} }; // 노란색
    Vertexs[2] = FEngineVertex{ FVector(0.0f, -1.0f, 0.0f), {0.5f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} }; // 노란색

    UEngineVertexBuffer::Create("Triangle8", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // 하단 꼭짓점
    Indices.push_back(2); // 중심선 앞쪽
    Indices.push_back(1); // 중심선 오른쪽

    UEngineIndexBuffer::Create("Triangle8", Indices);
    UMesh::Create("Triangle8");
}



	{
		std::vector<FEngineVertex> Vertexs;
		Vertexs.resize(6);

		Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, 1.0f), {0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = FEngineVertex{ FVector(0.0f, -0.5f, 0.0f), {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = FEngineVertex{ FVector(0.5f, 0.0f, 0.0f), {1.0f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} };
		Vertexs[3] = FEngineVertex{ FVector(0.0f, 0.5f, 0.0f), {0.5f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[4] = FEngineVertex{ FVector(-0.5f, 0.0f, 0.0f), {0.0f, 0.5f}, {0.0f, 1.0f, 1.0f, 1.0f} };
		Vertexs[5] = FEngineVertex{ FVector(0.0f, 0.0f, -1.0f), {0.5f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f} };

		// 생성
		UEngineVertexBuffer::Create("Diamond", Vertexs);

		// 6개의 버텍스를 정의한 후 인덱스 버퍼 생성
		std::vector<uint32_t> Indices;

		// 상단 삼각형 4개
		Indices.push_back(0); // 상단 꼭짓점
		Indices.push_back(1); // 중심선 앞쪽
		Indices.push_back(2); // 중심선 오른쪽

		Indices.push_back(0); // 상단 꼭짓점
		Indices.push_back(2); // 중심선 오른쪽
		Indices.push_back(3); // 중심선 뒤쪽

		Indices.push_back(0); // 상단 꼭짓점
		Indices.push_back(3); // 중심선 뒤쪽
		Indices.push_back(4); // 중심선 왼쪽

		Indices.push_back(0); // 상단 꼭짓점
		Indices.push_back(4); // 중심선 왼쪽
		Indices.push_back(1); // 중심선 앞쪽

		// 하단 삼각형 4개
		Indices.push_back(5); // 하단 꼭짓점
		Indices.push_back(1); // 중심선 앞쪽
		Indices.push_back(2); // 중심선 오른쪽

		Indices.push_back(5); // 하단 꼭짓점
		Indices.push_back(2); // 중심선 오른쪽
		Indices.push_back(3); // 중심선 뒤쪽

		Indices.push_back(5); // 하단 꼭짓점
		Indices.push_back(3); // 중심선 뒤쪽
		Indices.push_back(4); // 중심선 왼쪽

		Indices.push_back(5); // 하단 꼭짓점
		Indices.push_back(4); // 중심선 왼쪽
		Indices.push_back(1); // 중심선 앞쪽

		// 생성
		UEngineIndexBuffer::Create("Diamond", Indices);

		UMesh::Create("Diamond");
	}
}

void UEngineGraphicDevice::BlendInit()
{
	// 머티리얼이나 이런곳에서 이 블랜드 세팅이 존재한다.
	// 컬러 블랜드랑 다른 블랜드랑 햇갈리면 안됩니다.
	// 대놓고 알파 블랜드라고 명칭되는 곳이 있고
	// transparent 라는 단어


	D3D11_BLEND_DESC Desc = {0};

	// 자동으로 알파부분을 
	// 알파가 0.0f 색상부분을 알아서 안그리게 도와주는 기능
	// 굉장히 많이 느려져서 그냥 내가 다 처리하는게 더 빨랐다.

	Desc.AlphaToCoverageEnable = false;

	//BOOL AlphaToCoverageEnable;
	//BOOL IndependentBlendEnable;
	//D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[8];
	// 0번 세팅으로 8개를 다 합니다.
	Desc.IndependentBlendEnable = true;
	Desc.RenderTarget[0].BlendEnable = true;
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	// 알베도컬러 RGB A
	// 알베도컬러 SrcColor 옵션 SrcFactor  BlendOp  DestColor  옵션 DestFactor  
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	// 알파블랜드의 기본 공식

	// SrcColor 1.0, 0.0f, 0.0f, 0.8f; * 0.8f 0.8f 0.8f 0.8f
	
	// SrcColor 0.0, 0.0f, 1.0f, 1.0f; * 1 - 0.8f,  1 - 0.8f, 1 - 0.8f, 1 - 0.8f

	// 알베도컬러 SrcColor 옵션 SrcFactor  BlendOp  DestColor  옵션 DestFactor  
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	// 알파값 저 갑자기 이상해졌어요.
	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	UEngineBlend::Create("AlphaBlend", Desc);
}

void UEngineGraphicDevice::RasterizerStateInit()
{
	{
		D3D11_RASTERIZER_DESC Desc = {};
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

		UEngineRasterizerState::Create("EngineBase", Desc);
	}

	{
		D3D11_RASTERIZER_DESC Desc = {};
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

		UEngineRasterizerState::Create("CollisionDebugRas", Desc);
	}
}

void UEngineGraphicDevice::MaterialInit()
{
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("SpriteMaterial");
		Mat->SetVertexShader("EngineSpriteShader.fx");
		Mat->SetPixelShader("EngineSpriteShader.fx");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("WidgetMaterial");
		Mat->SetVertexShader("EngineSpriteShader.fx");
		Mat->SetPixelShader("EngineSpriteShader.fx");
		Mat->SetDepthStencilState("UIDepth");
	}


	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("CollisionDebugMaterial");
		Mat->SetVertexShader("EngineDebugCollisionShader.fx");
		Mat->SetPixelShader("EngineDebugCollisionShader.fx");
		// 언제나 화면에 나오는 누구도 이녀석의 앞을 가릴수 없어.
		Mat->SetDepthStencilState("CollisionDebugDepth");
		Mat->SetRasterizerState("CollisionDebugRas");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("TileMap");
		Mat->SetVertexShader("EngineTileMapShader.fx");
		Mat->SetPixelShader("EngineTileMapShader.fx");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("TargetMerge");
		Mat->SetVertexShader("EngineTargetMergeShader.fx");
		Mat->SetPixelShader("EngineTargetMergeShader.fx");
		Mat->SetDepthStencilState("TargetMerge");
	}



}