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

	// ���� �ڱ� ��Ʈ 
	UEngineFont::Load("�ü�", "�ü�");
}

void UEngineGraphicDevice::DepthStencilInit()
{
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// ���̰��� �� ������ �������
		Desc.DepthFunc = D3D11_COMPARISON_LESS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		UEngineDepthStencilState::Create("BaseDepth", Desc);
	}

	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = false;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// ���̰��� �� ������ �������
		Desc.DepthFunc = D3D11_COMPARISON_LESS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		UEngineDepthStencilState::Create("UIDepth", Desc);
	}


	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// ���̰��� �� ������ �������
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		UEngineDepthStencilState::Create("CollisionDebugDepth", Desc);
	}

	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// ���̰��� �� ������ �������
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		UEngineDepthStencilState::Create("TargetMerge", Desc);
	}

}

void UEngineGraphicDevice::TextureInit()
{

		D3D11_SAMPLER_DESC SampInfo = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT };
		SampInfo.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP; // 0~1���̸� ��ȿ
		SampInfo.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP; // y
		SampInfo.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP; // z // 3�� 

		SampInfo.BorderColor[0] = 0.0f;
		SampInfo.BorderColor[1] = 0.0f;
		SampInfo.BorderColor[2] = 0.0f;
		SampInfo.BorderColor[3] = 0.0f;

		// SampInfo.ComparisonFunc = D3D11_COMPARISON_NEVER;
		// Lod��� �Ҹ��� ���� z���� �󸶳� �־����� �󸶳� ���� ���� ���ðų�. 
		// SampInfo.MaxLOD = 0.0f;
		// SampInfo.MinLOD = 0.0f;

		UEngineSampler::Create("WRAPSampler", SampInfo);


	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("EngineShader"))
		{
			MSGASSERT("EngineShader ������ ã�� ���߽��ϴ�.");
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
		// FullRect ����Ʈ���μ��̿� ȭ�� ��üũ�� ���� �Ž��� ����.
		UMesh::Create("FullRect", "FullRect", "Rect");
	}

	// Triangle 1 (������)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // �ﰢ�� 1�� �� �������� ������ ����
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, 0.5f), {0.5f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f} }; // ������
    Vertexs[1] = FEngineVertex{ FVector(0.0f, -1.0f, 0.0f), {0.5f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} }; // ������
    Vertexs[2] = FEngineVertex{ FVector(0.5f, 0.0f, 0.0f), {1.0f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} }; // ������

    UEngineVertexBuffer::Create("Triangle1", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // ��� ������
    Indices.push_back(1); // �߽ɼ� ����
    Indices.push_back(2); // �߽ɼ� ������

    UEngineIndexBuffer::Create("Triangle1", Indices);
    UMesh::Create("Triangle1");
}

// Triangle 2 (��ũ��)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // �ﰢ�� 2�� �� �������� ��ũ�� ����
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, 0.5f), {0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} }; // ��ũ��
    Vertexs[1] = FEngineVertex{ FVector(0.5f, 0.0f, 0.0f), {1.0f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }; // ��ũ��
    Vertexs[2] = FEngineVertex{ FVector(0.0f, 1.0f, 0.0f), {0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }; // ��ũ��

    UEngineVertexBuffer::Create("Triangle2", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // ��� ������
    Indices.push_back(2); // �߽ɼ� ����
    Indices.push_back(1); // �߽ɼ� ������

    UEngineIndexBuffer::Create("Triangle2", Indices);
    UMesh::Create("Triangle2");
}

// Triangle 3 (���)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // �ﰢ�� 3�� �� �������� ��� ����
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, 0.5f), {0.5f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} }; // ���
    Vertexs[1] = FEngineVertex{ FVector(0.0f, 1.0f, 0.0f), {0.5f, 0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} }; // ���
    Vertexs[2] = FEngineVertex{ FVector(-0.5f, 0.0f, 0.0f), {0.0f, 0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} }; // ���

    UEngineVertexBuffer::Create("Triangle3", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // ��� ������
    Indices.push_back(1); // �߽ɼ� ����
    Indices.push_back(2); // �߽ɼ� ������

    UEngineIndexBuffer::Create("Triangle3", Indices);
    UMesh::Create("Triangle3");
}

// Triangle 4 (�����)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // �ﰢ�� 4�� �� �������� ����� ����
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, 0.5f), {0.5f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} }; // �����
    Vertexs[1] = FEngineVertex{ FVector(-0.5f, 0.0f, 0.0f), {0.0f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }; // �����
    Vertexs[2] = FEngineVertex{ FVector(0.0f, -1.0f, 0.0f), {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }; // �����

    UEngineVertexBuffer::Create("Triangle4", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // ��� ������
    Indices.push_back(1); // �߽ɼ� ����
    Indices.push_back(2); // �߽ɼ� ������

    UEngineIndexBuffer::Create("Triangle4", Indices);
    UMesh::Create("Triangle4");
}

// Triangle 5 (������)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // �ﰢ�� 5�� �� �������� ������ ����
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, -0.5f), {0.5f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} }; // ������
    Vertexs[1] = FEngineVertex{ FVector(0.0f, -1.0f, 0.0f), {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }; // ������
    Vertexs[2] = FEngineVertex{ FVector(0.5f, 0.0f, 0.0f), {1.0f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }; // ������

    UEngineVertexBuffer::Create("Triangle5", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // �ϴ� ������
    Indices.push_back(1); // �߽ɼ� ����
    Indices.push_back(2); // �߽ɼ� ������

    UEngineIndexBuffer::Create("Triangle5", Indices);
    UMesh::Create("Triangle5");
}

// Triangle 6 (��ũ��)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // �ﰢ�� 6�� �� �������� ��ũ�� ����
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, -0.5f), {0.5f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} }; // ��ũ��
    Vertexs[1] = FEngineVertex{ FVector(0.5f, 0.0f, 0.0f), {1.0f, 0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} }; // ��ũ��
    Vertexs[2] = FEngineVertex{ FVector(0.0f, 1.0f, 0.0f), {0.5f, 0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} }; // ��ũ��

    UEngineVertexBuffer::Create("Triangle6", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // �ϴ� ������
    Indices.push_back(2); // �߽ɼ� ����
    Indices.push_back(1); // �߽ɼ� ������

    UEngineIndexBuffer::Create("Triangle6", Indices);
    UMesh::Create("Triangle6");
}

// Triangle 7 (���)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // �ﰢ�� 7�� �� �������� ��� ����
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, -0.5f), {0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} }; // ���
    Vertexs[1] = FEngineVertex{ FVector(0.0f, 1.0f, 0.0f), {0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }; // ���
    Vertexs[2] = FEngineVertex{ FVector(-0.5f, 0.0f, 0.0f), {0.0f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }; // ���

    UEngineVertexBuffer::Create("Triangle7", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // �ϴ� ������
    Indices.push_back(1); // �߽ɼ� ����
    Indices.push_back(2); // �߽ɼ� ������

    UEngineIndexBuffer::Create("Triangle7", Indices);
    UMesh::Create("Triangle7");
}

// Triangle 8 (�����)
{
    std::vector<FEngineVertex> Vertexs;
    Vertexs.resize(3);

    // �ﰢ�� 8�� �� �������� ����� ����
    Vertexs[0] = FEngineVertex{ FVector(0.0f, 0.0f, -0.5f), {0.5f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f} }; // �����
    Vertexs[1] = FEngineVertex{ FVector(-0.5f, 0.0f, 0.0f), {0.0f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} }; // �����
    Vertexs[2] = FEngineVertex{ FVector(0.0f, -1.0f, 0.0f), {0.5f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} }; // �����

    UEngineVertexBuffer::Create("Triangle8", Vertexs);

    std::vector<uint32_t> Indices;
    Indices.push_back(0); // �ϴ� ������
    Indices.push_back(2); // �߽ɼ� ����
    Indices.push_back(1); // �߽ɼ� ������

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

		// ����
		UEngineVertexBuffer::Create("Diamond", Vertexs);

		// 6���� ���ؽ��� ������ �� �ε��� ���� ����
		std::vector<uint32_t> Indices;

		// ��� �ﰢ�� 4��
		Indices.push_back(0); // ��� ������
		Indices.push_back(1); // �߽ɼ� ����
		Indices.push_back(2); // �߽ɼ� ������

		Indices.push_back(0); // ��� ������
		Indices.push_back(2); // �߽ɼ� ������
		Indices.push_back(3); // �߽ɼ� ����

		Indices.push_back(0); // ��� ������
		Indices.push_back(3); // �߽ɼ� ����
		Indices.push_back(4); // �߽ɼ� ����

		Indices.push_back(0); // ��� ������
		Indices.push_back(4); // �߽ɼ� ����
		Indices.push_back(1); // �߽ɼ� ����

		// �ϴ� �ﰢ�� 4��
		Indices.push_back(5); // �ϴ� ������
		Indices.push_back(1); // �߽ɼ� ����
		Indices.push_back(2); // �߽ɼ� ������

		Indices.push_back(5); // �ϴ� ������
		Indices.push_back(2); // �߽ɼ� ������
		Indices.push_back(3); // �߽ɼ� ����

		Indices.push_back(5); // �ϴ� ������
		Indices.push_back(3); // �߽ɼ� ����
		Indices.push_back(4); // �߽ɼ� ����

		Indices.push_back(5); // �ϴ� ������
		Indices.push_back(4); // �߽ɼ� ����
		Indices.push_back(1); // �߽ɼ� ����

		// ����
		UEngineIndexBuffer::Create("Diamond", Indices);

		UMesh::Create("Diamond");
	}
}

void UEngineGraphicDevice::BlendInit()
{
	// ��Ƽ�����̳� �̷������� �� ���� ������ �����Ѵ�.
	// �÷� ����� �ٸ� ����� �ް����� �ȵ˴ϴ�.
	// ����� ���� ������ ��Ī�Ǵ� ���� �ְ�
	// transparent ��� �ܾ�


	D3D11_BLEND_DESC Desc = {0};

	// �ڵ����� ���ĺκ��� 
	// ���İ� 0.0f ����κ��� �˾Ƽ� �ȱ׸��� �����ִ� ���
	// ������ ���� �������� �׳� ���� �� ó���ϴ°� �� ������.

	Desc.AlphaToCoverageEnable = false;

	//BOOL AlphaToCoverageEnable;
	//BOOL IndependentBlendEnable;
	//D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[8];
	// 0�� �������� 8���� �� �մϴ�.
	Desc.IndependentBlendEnable = true;
	Desc.RenderTarget[0].BlendEnable = true;
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	// �˺����÷� RGB A
	// �˺����÷� SrcColor �ɼ� SrcFactor  BlendOp  DestColor  �ɼ� DestFactor  
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	// ���ĺ����� �⺻ ����

	// SrcColor 1.0, 0.0f, 0.0f, 0.8f; * 0.8f 0.8f 0.8f 0.8f
	
	// SrcColor 0.0, 0.0f, 1.0f, 1.0f; * 1 - 0.8f,  1 - 0.8f, 1 - 0.8f, 1 - 0.8f

	// �˺����÷� SrcColor �ɼ� SrcFactor  BlendOp  DestColor  �ɼ� DestFactor  
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	// ���İ� �� ���ڱ� �̻��������.
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
		// ������ ȭ�鿡 ������ ������ �̳༮�� ���� ������ ����.
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