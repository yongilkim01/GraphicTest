#include "PreCompile.h"
#include "EngineCamera.h"
#include "Renderer.h"
#include "EngineRenderTarget.h"

UEngineCamera::UEngineCamera()
{
}

void UEngineCamera::BeginPlay()
{
	USceneComponent::BeginPlay();

	FVector Scale = UEngineCore::GetScreenScale();

	ProjectionScale = Scale;

	ViewPortInfo.Width = UEngineCore::GetScreenScale().X;
	ViewPortInfo.Height = UEngineCore::GetScreenScale().Y;
	ViewPortInfo.TopLeftX = 0.0f;
	ViewPortInfo.TopLeftY = 0.0f;
	ViewPortInfo.MinDepth = 0.0f;
	ViewPortInfo.MaxDepth = 1.0f;

	// ī�޶� ���� Ÿ���� ������ �ɰ��̹Ƿ� �������� �̵��մϴ�.
	CameraTarget = std::make_shared<UEngineRenderTarget>();
	CameraTarget->CreateTarget(UEngineCore::GetScreenScale());
	CameraTarget->CreateDepth();

	// ī�޶� ������ ����� �׸��� ������ŭ�� �ٸ� �ָ� ���� �������� �ִµ�.
}

UEngineCamera::~UEngineCamera()
{
	Renderers.clear();
}

void UEngineCamera::Tick(float _DetlaTime)
{
	USceneComponent::ComponentTick(_DetlaTime);
	// ī�޶�� ƽ���� �ڽ��� ��� ������Ʈ�� ����Ѵ��� �������鿡�� �������� �̴ϴ�.
	Transform.View;
	Transform.Projection;
}

void UEngineCamera::Render(float _DetlaTime)
{
	// ������ �����ϱ� ���� �ѹ� ����Ʈ �����ϰ� 
	UEngineCore::GetDevice().GetContext()->RSSetViewports(1, &ViewPortInfo);
	CameraTarget->Clear();
	CameraTarget->Setting();

	//// Ranged for�� �������� ���簡 �Ͼ�Ƿ�
	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RenderGroup : Renderers)
	{
		std::list<std::shared_ptr<URenderer>>& RenderList = RenderGroup.second;

		if (true == RendererZSort[RenderGroup.first])
		{
			// ���� z���� ������ ������������ ���� ����.
			// ũ�� ���ؼ� ũ�Ⱑ �� �������� �������� �ű��.
			RenderList.sort([](std::shared_ptr<URenderer>& _Left, std::shared_ptr<URenderer>& _Right)
				{
					return _Left->GetTransformRef().WorldLocation.Z > _Right->GetTransformRef().WorldLocation.Z;
				});
		}


		for (std::shared_ptr<URenderer> Renderer : RenderList)
		{
			if (false == Renderer->IsActive())
			{
				continue;
			}

			Renderer->Render(this, _DetlaTime);
		}
	}
}

void UEngineCamera::Release(float _DeltaTime)
{

	//// Ranged for�� �������� ���簡 �Ͼ�Ƿ�
	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RenderGroup : Renderers)
	{
		std::list<std::shared_ptr<URenderer>>& RenderList = RenderGroup.second;
		std::list<std::shared_ptr<URenderer>>::iterator StartIter = RenderList.begin();
		std::list<std::shared_ptr<URenderer>>::iterator EndIter = RenderList.end();

		// �𸮾��� �߰��� �����Ҽ� ����.
		for (; StartIter != EndIter; )
		{
			if (false == (*StartIter)->IsDestroy())
			{
				++StartIter;
				continue;
			}

			// �������� ���� �ʿ䰡 �����ϴ�.
			// (*RenderStartIter) ���� ���� ������ ��������.
			// ������Ʈ�� �޸𸮸� �����Ҽ� ������ ������ ���͸� ������ �ִ�.
			StartIter = RenderList.erase(StartIter);
		}
	}
}

void UEngineCamera::SetZSort(int _Order, bool _Value)
{
	RendererZSort[_Order] = _Value;
}

void UEngineCamera::ChangeRenderGroup(int _PrevGroupOrder, std::shared_ptr<URenderer> _Renderer)
{
	Renderers[_PrevGroupOrder].remove(_Renderer);
	Renderers[_Renderer->GetOrder()].push_back(_Renderer);
}

void UEngineCamera::CalculateViewAndProjection()
{
	FTransform& Trans = GetTransformRef();

	Trans.View.View(Trans.World.ArrVector[3], Trans.World.GetFoward(), Trans.World.GetUp());

	switch (ProjectionType)
	{
	case EProjectionType::Perspective:
		Trans.Projection.PerspectiveFovDeg(FOV, ProjectionScale.X, ProjectionScale.Y, Near, Far);
		break;
	case EProjectionType::Orthographic:
		Trans.Projection.OrthographicLH(ProjectionScale.X, ProjectionScale.Y, Near, Far);
		break;
	default:
		break;
	}

	int a = 0;
}