#include "PreCompile.h"
#include "SpriteRenderer.h"
#include "EngineCamera.h"

USpriteRenderer::USpriteRenderer()
{
	CreateRenderUnit();
	SetMesh("Rect");
	SetMaterial("SpriteMaterial");

	GetRenderUnit().ConstantBufferLinkData("ResultColor", ColorData);
	GetRenderUnit().ConstantBufferLinkData("FSpriteData", SpriteData);
	GetRenderUnit().ConstantBufferLinkData("FUVValue", UVValue);


	UVValue.PlusUVValue = {0.0f, 0.0f, 0.0f, 0.0f};
	SpriteData.CuttingPos = {0.0f, 0.0f};
	SpriteData.CuttingSize = { 1.0f, 1.0f };
	SpriteData.Pivot = { 0.5f, 0.5f };

	ColorData.PlusColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
}

USpriteRenderer::~USpriteRenderer()
{
}

void USpriteRenderer::SetSprite(std::string_view _Name, UINT _Index)
{
	Sprite = UEngineSprite::Find<UEngineSprite>(_Name).get();

	GetRenderUnit().SetTexture("ImageTexture", Sprite->GetTexture(_Index)->GetName());
	SpriteData = Sprite->GetSpriteData(_Index);

	CurIndex = _Index;
}

void USpriteRenderer::SetTexture(std::string_view _Name, bool AutoScale /*= false*/, float _Ratio /*= 1.0f*/)
{
	std::shared_ptr<UEngineTexture> Texture = UEngineTexture::Find<UEngineTexture>(_Name);

	if (nullptr == Texture)
	{
		MSGASSERT("�ε����� ���� �ؽ�ó�� ����Ϸ��� �߽��ϴ�.");
	}

	GetRenderUnit().SetTexture("ImageTexture", _Name);

	if (true == AutoScale)
	{
		SetRelativeScale3D(Texture->GetTextureSize() * _Ratio);
	}
}

void USpriteRenderer::BeginPlay()
{
	URenderer::BeginPlay();

}

USpriteRenderer::FrameAnimation* USpriteRenderer::FindAnimation(std::string_view _AnimationName)
{
	std::string UpperString = UEngineString::ToUpper(_AnimationName);

	if (false == FrameAnimations.contains(UpperString))
	{
		return nullptr;
	}

	return &FrameAnimations[UpperString];
}

void USpriteRenderer::Render(UEngineCamera* _Camera, float _DeltaTime)
{
	if (nullptr != CurAnimation)
	{
		Sprite = CurAnimation->Sprite;

		GetRenderUnit().SetTexture("ImageTexture", Sprite->GetTexture(CurIndex)->GetName());
		SpriteData = Sprite->GetSpriteData(CurIndex);
	}

	if (true == IsAutoScale && nullptr != Sprite)
	{
		FVector Scale = Sprite->GetSpriteScaleToReal(CurIndex);
		Scale.Z = 1.0f;
		SetRelativeScale3D(Scale * AutoScaleRatio);
	}

	URenderer::Render(_Camera, _DeltaTime);

	if (true == IsBillboard)
	{
		Transform.WVP;
	}
}

void USpriteRenderer::CameraTransUpdate(UEngineCamera* _Camera)
{
	FTransform& CameraTrans = _Camera->GetTransformRef();
	FTransform& RendererTrans = GetTransformRef();
	//	// �������� ���� �� ������Ʈ�� �� ���ù޾Ұ�
	// RendererTrans.View = CameraTrans.View;

	RendererTrans.View = CameraTrans.View;
	FMatrix CurWorld = RendererTrans.World;

	if (true == IsBillboard)
	{
		// world���� �̵���Ų ��ġ�� ȸ���� ���ؼ� ��ġ�� ��߳���.
		// ȸ������� ����ع����ϱ� 
		//RendererTrans.View.ArrVector[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
		//RendererTrans.View.ArrVector[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
		//RendererTrans.View.ArrVector[2] = { 0.0f, 0.0f, 1.0f, 0.0f };

		// �� �̾߱�� ������ ����� ����������� view����� ��ҽ�Ű��
		// ȸ������� ������ķ� ���� �Ѵ�.
		// ��������� ����
		// ����(ũ�� * ���� * �̵� * ���� * �θ�) => �� => �������� => ����Ʈ
		// ���� => ����� ȸ�� �Ӽ��� ������� �Ǿ�� �մϴ�.
		// ȸ������� ������� transpose

		// ���������
		// ����(ũ�� * ���� * �̵� * ���� * �θ�) => �� => �������� => ����Ʈ 

		// ī�޶������ �Ǿ���.
		FMatrix Bill = CameraTrans.View;
		// ������� �̵��κ��� �ʱ�ȭ
		Bill.ArrVector[3] = FVector(0.0f, 0.0f, 0.0f, 1.0f);
		// ����ķ� ���ص� ���������� �ִ� ȸ���Ӽ��� ������� ��ġ����̱� ������ �̷��� ó�����ָ� �ȴ�.
		Bill.Transpose();

		// ������ ����� Ʈ�������� �� ���� ���� ������ ����
		CurWorld = RendererTrans.ScaleMat * Bill * RendererTrans.LocationMat * RendererTrans.RevolveMat * RendererTrans.ParentMat;
	}

	RendererTrans.Projection = CameraTrans.Projection;
	RendererTrans.WVP = CurWorld * RendererTrans.View * RendererTrans.Projection;
}

void USpriteRenderer::ComponentTick(float _DeltaTime)
{
	URenderer::ComponentTick(_DeltaTime);

	// �ִϸ��̼� �����Ű�� �ڵ带 ComponentTick���� �Ű��. 
	if (nullptr != CurAnimation)
	{
		FrameAnimation* EventAnimation = nullptr;
		int EventFrame = -1;

		CurAnimation->IsEnd = false;
		std::vector<int>& Indexs = CurAnimation->FrameIndex;
		std::vector<float>& Times = CurAnimation->FrameTime;

		Sprite = CurAnimation->Sprite;


		CurAnimation->CurTime += _DeltaTime * CurAnimationSpeed;

		float CurFrameTime = Times[CurAnimation->CurIndex];

		//                           0.1 0.1 0.1
		if (CurAnimation->CurTime > CurFrameTime)
		{

			CurAnimation->CurTime -= CurFrameTime;
			++CurAnimation->CurIndex;

			if (CurAnimation->Events.contains(CurIndex))
			{
				EventAnimation = CurAnimation;
				EventFrame = CurIndex;
				// CurAnimation->Events[CurIndex]();
			}

			// �ִϸ��̼� �ص�
			if (CurAnimation->CurIndex >= Indexs.size())
			{
				CurAnimation->IsEnd = true;
			}
			else {
				CurAnimation->IsEnd = false;
			}


			if (CurAnimation->CurIndex >= Indexs.size())
			{
				if (true == CurAnimation->Loop)
				{
					CurAnimation->CurIndex = 0;

					if (CurAnimation->Events.contains(CurIndex))
					{
						EventAnimation = CurAnimation;
						EventFrame = CurIndex;
						// CurAnimation->Events[CurIndex]();
					}
				}
				else
				{
					CurAnimation->IsEnd = true;
					--CurAnimation->CurIndex;
				}
			}

		}


		CurIndex = Indexs[CurAnimation->CurIndex];

		if (nullptr != EventAnimation)
		{
			if (EventAnimation->Events.contains(CurIndex))
			{
				EventAnimation->Events[CurIndex]();
			}
		}
	}


}



void USpriteRenderer::CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, int _Start, int _End, float Time /*= 0.1f*/, bool _Loop /*= true*/)
{
	int Inter = 0;

	std::vector<int> Indexs;
	std::vector<float> Times;

	if (_Start < _End)
	{
		Inter = (_End - _Start) + 1;
		for (size_t i = 0; i < Inter; i++)
		{
			Indexs.push_back(_Start);
			Times.push_back(Time);
			++_Start;
		}

	}
	else
	{
		Inter = (_Start - _End) + 1;
		for (size_t i = 0; i < Inter; i++)
		{
			Indexs.push_back(_End);
			Times.push_back(Time);
			++_End;
		}
	}


	CreateAnimation(_AnimationName, _SpriteName, Indexs, Times, _Loop);
}


void USpriteRenderer::CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, std::vector<int> _Indexs, float _Frame, bool _Loop /*= true*/)
{
	std::vector<float> Times;

	for (size_t i = 0; i < _Indexs.size(); i++)
	{
		Times.push_back(_Frame);
	}

	CreateAnimation(_AnimationName, _SpriteName, _Indexs, Times, _Loop);
}

void USpriteRenderer::CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, std::vector<int> _Indexs, std::vector<float> _Frame, bool _Loop /*= true*/)
{
	std::string UpperName = UEngineString::ToUpper(_AnimationName);

	if (_Frame.size() != _Indexs.size())
	{
		MSGASSERT(UpperName + "�� ����� ������ �����ϴ� �����Ӱ� Ÿ���� ī��Ʈ�� ���� �ٸ��ϴ�");
		return;
	}

	if (FrameAnimations.contains(UpperName))
	{
		return;
	}

	std::shared_ptr<UEngineSprite> FindSprite = UEngineSprite::Find<UEngineSprite>(_SpriteName);

	if (nullptr == FindSprite)
	{
		MSGASSERT("�ε����� ���� ��������Ʈ�� �ִϸ��̼� ������ ����Ϸ��� �߽��ϴ�" + std::string(UpperName));
		return;
	}

	FrameAnimation NewAnimation;
	NewAnimation.Sprite = FindSprite.get();
	NewAnimation.FrameIndex = _Indexs;
	NewAnimation.FrameTime = _Frame;
	NewAnimation.Loop = _Loop;
	NewAnimation.Reset();

	FrameAnimations.insert({ UpperName ,NewAnimation });

}

void USpriteRenderer::ChangeAnimation(std::string_view _AnimationName, bool _Force /*= false*/)
{
	std::string UpperName = UEngineString::ToUpper(_AnimationName);

	if (false == FrameAnimations.contains(UpperName))
	{
		MSGASSERT("�������� ���� �ִϸ��̼����� �����Ϸ��� �߽��ϴ�. = " + UpperName);
		return;
	}

	FrameAnimation* ChangeAnimation = &FrameAnimations[UpperName];

	if (CurAnimation == ChangeAnimation && false == _Force)
	{
		return;
	}

	CurAnimation = &FrameAnimations[UpperName];
	CurAnimation->Reset();
	CurIndex = CurAnimation->FrameIndex[CurAnimation->CurIndex];

	if (CurAnimation->Events.contains(CurAnimation->CurIndex))
	{
		CurAnimation->Events[CurAnimation->CurIndex]();
	}

	if (true == IsAutoScale)
	{
		FVector Scale = CurAnimation->Sprite->GetSpriteScaleToReal(CurIndex);
		Scale.Z = 1.0f;
		SetRelativeScale3D(Scale * AutoScaleRatio);
	}
}


void USpriteRenderer::SetAnimationEvent(std::string_view _AnimationName, int _Frame, std::function<void()> _Function)
{
	std::string UpperName = UEngineString::ToUpper(_AnimationName);

	if (false == FrameAnimations.contains(UpperName))
	{
		MSGASSERT("�������� ���� �ִϸ��̼����� �����Ϸ��� �߽��ϴ�. = " + UpperName);
		return;
	}

	FrameAnimation* ChangeAnimation = &FrameAnimations[UpperName];

	bool Check = false;

	for (size_t i = 0; i < ChangeAnimation->FrameIndex.size(); i++)
	{
		if (_Frame == ChangeAnimation->FrameIndex[i])
		{
			Check = true;
			break;
		}
	}

	if (false == Check)
	{
		MSGASSERT("�������� �ʴ� �����ӿ� �̺�Ʈ�� �����Ϸ��� �߽��ϴ�" + std::string(_AnimationName));
		return;
	}

	ChangeAnimation->Events[_Frame] += _Function;

}



void USpriteRenderer::SetSprite(UEngineSprite* _Sprite)
{
	Sprite = _Sprite;

	if (nullptr == Sprite)
	{
		MSGASSERT("�������� �ʴ� ��������Ʈ�� ����Ϸ��� �߽��ϴ�.");
	}
}
