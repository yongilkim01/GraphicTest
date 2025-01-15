#include "PreCompile.h"
#include "EngineRenderTarget.h"

UEngineRenderTarget::UEngineRenderTarget()
{
    TargetUnit.SetMesh("FullRect");
    TargetUnit.SetMaterial("TargetMerge");
}

UEngineRenderTarget::~UEngineRenderTarget()
{
}


void UEngineRenderTarget::CreateTarget(float4 _Scale, float4 _ClearColor /*= float4::NONE*/, DXGI_FORMAT _Format /*= DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT*/)
{
    ClearColor = _ClearColor;

    std::shared_ptr<class UEngineTexture> NewTarget = std::make_shared<UEngineTexture>();

    // ����Ÿ���̶�� ������ ����� �̴ϴ�. 
    D3D11_TEXTURE2D_DESC Desc = { 0 };
    Desc.ArraySize = 1;
    Desc.Width = _Scale.iX();
    Desc.Height = _Scale.iY();
    // 3����Ʈ �Ǽ� 1����Ʈ ������ ���Ľ� ���̶�� �մϴ�.
    Desc.Format = _Format;

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    // �̷��� ����� �༮����

    Desc.MipLevels = 1;
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.CPUAccessFlags = 0;
    // ���⿡ �׷������� �ְ� 
    // �׷��� �� ���̴����� �ÿ��Ҽ� �� �ִ�.
    Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

    NewTarget->ResCreate(Desc);
    NewTarget->Size = _Scale;

    ArrRTVs.push_back(NewTarget->GetRTV());
    ArrTexture.push_back(NewTarget);
}


void UEngineRenderTarget::CreateTarget(Microsoft::WRL::ComPtr<ID3D11Texture2D> _Texture2D)
{
	std::shared_ptr<class UEngineTexture> NewTarget = std::make_shared<UEngineTexture>();
	NewTarget->ResCreate(_Texture2D);


    if (nullptr == NewTarget->GetRTV())
    {
        MSGASSERT("����Ÿ�� �䰡 �������� �ʴ� ����Ÿ�� �Դϴ�.");
    }
    ;

    ArrRTVs.push_back(NewTarget->GetRTV());

    ArrTexture.push_back(NewTarget);
}

// ���° Ÿ���� ������� ���̹��۸� ����ų� �Դϴ�.
void UEngineRenderTarget::CreateDepth(int _Index)
{
    if (ArrTexture.size() <= _Index)
    {
        MSGASSERT("�������� �ʴ� �ؽ�ó �ε����� ���̹��۸� ������� �߽��ϴ�.");
    }

    FVector Size = ArrTexture[_Index]->GetTextureSize();

    // ����Ÿ���̶�� ������ ����� �̴ϴ�. 
    D3D11_TEXTURE2D_DESC Desc = { 0 };
    Desc.ArraySize = 1;
    Desc.Width = Size.iX();
    Desc.Height = Size.iY();
    // 3����Ʈ �Ǽ� 1����Ʈ ������ ���Ľ� ���̶�� �մϴ�.
    Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    Desc.MipLevels = 1;
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.CPUAccessFlags = 0;
    Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

    DepthTexture = std::make_shared<UEngineTexture>();
    DepthTexture->ResCreate(Desc);

}

void UEngineRenderTarget::Clear()
{
    for (size_t i = 0; i < ArrRTVs.size(); i++)
    {
        UEngineCore::GetDevice().GetContext()->ClearRenderTargetView(ArrRTVs[i], ClearColor.Arr1D);
    }
    UEngineCore::GetDevice().GetContext()->ClearDepthStencilView(DepthTexture->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void UEngineRenderTarget::Setting()
{
    //ID3D11RenderTargetView* RTV = UEngineCore::GetDevice().GetRTV();
    //ID3D11RenderTargetView* ArrRtv[16] = { 0 };
    // ArrRtv[0] = RTV; // SV_Target0
    UEngineCore::GetDevice().GetContext()->OMSetRenderTargets(1, &ArrRTVs[0], DepthTexture->GetDSV());
}

// ��� ����Ÿ���� ��Ҹ� �ٸ� ����Ÿ�ٿ� �����Ѵ�.
// ����� �����Ѵ�.
void UEngineRenderTarget::CopyTo(std::shared_ptr<UEngineRenderTarget> _Target)
{
    _Target->Clear();
    MergeTo(_Target);
}

void UEngineRenderTarget::MergeTo(std::shared_ptr<UEngineRenderTarget> _Target)
{
    _Target->Setting();
    TargetUnit.SetTexture("MergeTex", ArrTexture[0]);
    TargetUnit.Render(nullptr, 0.0f);
}