#include "PreCompile.h"
#include "TransformObject.h"
#include "EngineCamera.h"

UTransformObject::UTransformObject()
{
}

UTransformObject::~UTransformObject()
{
}

void UTransformObject::SetupAttachment(std::shared_ptr<UTransformObject> _Parent)
{
	SetupAttachment(_Parent.get());
}

void UTransformObject::ParentMatrixCheck()
{
	if (nullptr != Parent)
	{
		Transform.ParentMat = Parent->Transform.World;
	}
}

void UTransformObject::SetupAttachment(UTransformObject* _Parent)
{
	Parent = _Parent;
	Parent->Childs.push_back(this);

	TransformUpdate();
}

void UTransformObject::TransformUpdate()
{
	ParentMatrixCheck();
	// ���� Ʈ������ ������Ʈ�� �ϴ� �Ѵ�.
	Transform.TransformUpdate(IsAbsolute);

	for (UTransformObject* Child : Childs)
	{
		Child->TransformUpdate();
	}

	IsAbsolute = false;
}

void UTransformObject::CameraTransUpdate(UEngineCamera* _Camera)
{
	// ���� ���ϸ� Ʈ������ 
	// Ʈ�������� �������� ������ �ֽ��ϴ�.
	FTransform& CameraTrans = _Camera->GetTransformRef();
	FTransform& RendererTrans = GetTransformRef();
	//	// �������� ���� �� ������Ʈ�� �� ���ù޾Ұ�
	RendererTrans.View = CameraTrans.View;
	RendererTrans.Projection = CameraTrans.Projection;
	RendererTrans.WVP = RendererTrans.World * RendererTrans.View * RendererTrans.Projection;
}
