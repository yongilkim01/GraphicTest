#pragma once
// �⺻������ �������ֱ� ����
// windowsdk�� ���ԵǾ� �ְ� windowsdk ������ �����е� ������ ���丮�� �ڵ����ԵǾ� �ֽ��ϴ�.
#include <wrl.h>
#include <d3d11_4.h> // directx 11 ����4�� ���
#include <d3dcompiler.h> // ���̴� �����Ϸ��� �������̽� ���̴��� ���� ����
#include <EnginePlatform/EngineWindow.h>
#include <memory>


// ���̺귯����
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler") 
#pragma comment(lib, "dxguid")

// GetHighPerFormanceAdapter ����� ���ؼ��� ����� ������ �׷���ī�� ������ ���;��ϴµ�
// �̳༮�� �� �Լ����� ������ �ִ�.
#pragma comment(lib, "DXGI") 

// ���� 4�� ������ 
// ù��° ��ǥ ��罺ũ��(�ܻ���ũ��)
// ���̷�Ʈ x��������� ������ �ϼ��ϸ� 
// ���̷�Ʈ x���� ��������� �� ��������� �׸������� �˷��޶�� �մϴ�. 


// ��¥ ��Ƽ�÷����� �Ƿ���
// ���⿡�� directx�� ���ϳ��� ���̸� �ȵǰ� ¥�� �մϴ�.

// ���� :
class UEngineGraphicDevice
{
public:
	// constrcuter destructer
	ENGINEAPI UEngineGraphicDevice();
	ENGINEAPI ~UEngineGraphicDevice();

	// delete Function
	UEngineGraphicDevice(const UEngineGraphicDevice& _Other) = delete;
	UEngineGraphicDevice(UEngineGraphicDevice&& _Other) noexcept = delete;
	UEngineGraphicDevice& operator=(const UEngineGraphicDevice& _Other) = delete;
	UEngineGraphicDevice& operator=(UEngineGraphicDevice&& _Other) noexcept = delete;

	void CreateDeviceAndContext();

	void CreateBackBuffer(const UEngineWindow& _Window);

	// Adapter �׷���ī���� ������ ������ �ִ� �������̽�
	// ���̷�Ʈ x ����̽��� ����.
	IDXGIAdapter* GetHighPerFormanceAdapter();

	ENGINEAPI void Release();

	void RenderStart();

	void RenderEnd();

	ENGINEAPI ID3D11Device* GetDevice()
	{
		return Device.Get();
	}

	ENGINEAPI ID3D11DeviceContext* GetContext()
	{
		return Context.Get();
	}

	ENGINEAPI std::shared_ptr<class UEngineRenderTarget> GetBackBufferTarget()
	{
		return BackBufferTarget;
	}

protected:

private:

	// �׷��ȵ���̽�
	// 9������ �׷���� �ε��ض� ���ߴ�.

	// 11�� ���鼭 �������̽��� 2���� �η��� �з��ߴ�.

	// ���� ������ �Ŵϱ�.
	
	// �޸� �ε��ض� ����
	// �׷���ī�忡���� �׸� ������ �ش޶�� �Ҽ� �ֽ��ϴ�.
	Microsoft::WRL::ComPtr<ID3D11Device> Device = nullptr;

	// ������ �׷��� ����
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context = nullptr;

	// ���̷�Ʈ x������ ����۸� ���� ü���̶�� �θ���
	// ���� ��ü�� 
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain = nullptr;

	Microsoft::WRL::ComPtr<IDXGIAdapter> MainAdapter = nullptr;

	std::shared_ptr<class UEngineRenderTarget> BackBufferTarget;

	//FVector ClearColor = FVector::BLUE;
	ENGINEAPI void DefaultResourcesInit();

	ENGINEAPI void MeshInit();
	ENGINEAPI void BlendInit();
	ENGINEAPI void ShaderInit();
	ENGINEAPI void MaterialInit();
	ENGINEAPI void RasterizerStateInit();
	ENGINEAPI void TextureInit();
	ENGINEAPI void DepthStencilInit();
};

