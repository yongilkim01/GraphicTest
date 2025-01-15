#include "PreCompile.h"
#include "EngineMath.h"

//DirectX::BoundingSphere Sphere; // ��
//XMFLOAT3 Center;            // Center of the sphere.
//float Radius;               // Radius of the sphere.

//DirectX::BoundingBox AABB;
//XMFLOAT3 Center;            // Center of the box.
//XMFLOAT3 Extents;           // ���ͱ��� ����ũ�⸦ �ǹ��մϴ�.

//DirectX::BoundingOrientedBox OBB; // 
//XMFLOAT3 Center;            // Center of the box.
//XMFLOAT3 Extents;           // Distance from the center to each side.
//XMFLOAT4 Orientation;       // Unit quaternion representing rotation (box -> world).

// DirectX::BoundingFrustum Cam; // ī�޶� 

template<>
FQuat TVector<float>::DegAngleToQuaternion()
{
	FQuat Result;
	Result.DirectVector = DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectVector);
	return Result;
}

// ��׸��� �������� �ٲٴ� ���� �ȴ�.



// const FVector FVector::BLUE = {0.0f, 0.0f, 1.0f, 1.0f};


const FIntPoint FIntPoint::LEFT = { -1, 0 };
const FIntPoint FIntPoint::RIGHT = { 1, 0 };
const FIntPoint FIntPoint::UP = { 0, -1 };
const FIntPoint FIntPoint::DOWN = { 0, 1 };




FIntPoint FVector::ConvertToPoint() const
{
	return { iX(), iY() };
}


std::function<bool(const FTransform&, const FTransform&)> FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Max)][static_cast<int>(ECollisionType::Max)];

class CollisionFunctionInit
{
public:
	CollisionFunctionInit()
	{
		// �ʿ��Ҷ����� �����е��� �����ֽø� �׶� ���鵵�� 
		// ������ ������ �ʱ�ȭ �ɶ� �ʱ�ȭ�ϴ� ���� �ڵ����� �����Ҽ� �ִ�.
		// ������ ������ ��������� �� �۾��� �ڵ����� ����ȴ�.
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Rect)][static_cast<int>(ECollisionType::Rect)] = FTransform::RectToRect;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::CirCle)][static_cast<int>(ECollisionType::CirCle)] = FTransform::CirCleToCirCle;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Rect)][static_cast<int>(ECollisionType::CirCle)] = FTransform::RectToCirCle;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::CirCle)][static_cast<int>(ECollisionType::Rect)] = FTransform::CirCleToRect;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB2D)][static_cast<int>(ECollisionType::OBB2D)] = FTransform::OBB2DToOBB2D;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB2D)][static_cast<int>(ECollisionType::Rect)] = FTransform::OBB2DToRect;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB2D)][static_cast<int>(ECollisionType::CirCle)] = FTransform::OBB2DToCirCle;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB2D)][static_cast<int>(ECollisionType::Point)] = FTransform::OBB2DToPoint;

		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB)][static_cast<int>(ECollisionType::Sphere)] = FTransform::OBBToSphere;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB)][static_cast<int>(ECollisionType::AABB)] = FTransform::OBBToAABB;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB)][static_cast<int>(ECollisionType::OBB)] = FTransform::OBBToOBB;

		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Sphere)][static_cast<int>(ECollisionType::Sphere)] = FTransform::SphereToSphere;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Sphere)][static_cast<int>(ECollisionType::AABB)] = FTransform::SphereToAABB;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Sphere)][static_cast<int>(ECollisionType::OBB)] = FTransform::SphereToOBB;

		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::AABB)][static_cast<int>(ECollisionType::Sphere)] = FTransform::AABBToSphere;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::AABB)][static_cast<int>(ECollisionType::AABB)] = FTransform::AABBToAABB;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::AABB)][static_cast<int>(ECollisionType::OBB)] = FTransform::AABBToOBB;

	}
};

// ������ ����
CollisionFunctionInit Inst = CollisionFunctionInit();


FVector FQuat::QuaternionToEulerDeg() const
{
	return QuaternionToEulerRad() * UEngineMath::R2D;
}

FVector FQuat::QuaternionToEulerRad() const
{
	FVector result;

	float sinrCosp = 2.0f * (W * Z + X * Y);
	float cosrCosp = 1.0f - 2.0f * (Z * Z + X * X);
	result.Z = atan2f(sinrCosp, cosrCosp);

	float pitchTest = W * X - Y * Z;
	float asinThreshold = 0.4999995f;
	float sinp = 2.0f * pitchTest;

	if (pitchTest < -asinThreshold)
	{
		result.X = -(0.5f * UEngineMath::PI);
	}
	else if (pitchTest > asinThreshold)
	{
		result.X = (0.5f * UEngineMath::PI);
	}
	else
	{
		result.X = asinf(sinp);
	}

	// �̰� ��������� 
	float sinyCosp = 2.0f * (W * Y + X * Z);
	float cosyCosp = 1.0f - 2.0f * (X * X + Y * Y);
	result.Y = atan2f(sinyCosp, cosyCosp);

	// 0, 45 0 => ���ʹϿ����� �ٲ۴�.
	// SetActorRoation(���ʹϿ�);
	// FQuat GetActorRoation();
	// FQuat => ���� -180 , 275, - 180

	return result;
}


FVector FVector::Transform(const FVector& _Vector, const class FMatrix& _Matrix)
{
	return _Vector * _Matrix;
}

FVector FVector::TransformCoord(const FVector& _Vector, const class FMatrix& _Matrix)
{
	FVector Copy = _Vector;
	Copy.W = 1.0f;
	return Copy * _Matrix;
}

FVector FVector::TransformNormal(const FVector& _Vector, const class FMatrix& _Matrix)
{
	FVector Copy = _Vector;
	Copy.W = 0.0f;
	return Copy * _Matrix;
}

template<>
FVector FVector::operator*(const class FMatrix& _Matrix) const
{
	FVector Result;
	// ������ �ϼ��ϰ� ���ؼ� ��� Ȯ���غ�����.

	// x y z w�� �� �������� �Ѵ�.
	Result.X = Arr2D[0][0] * _Matrix.Arr2D[0][0] + Arr2D[0][1] * _Matrix.Arr2D[1][0] + Arr2D[0][2] * _Matrix.Arr2D[2][0] + Arr2D[0][3] * _Matrix.Arr2D[3][0];
	Result.Y = Arr2D[0][0] * _Matrix.Arr2D[0][1] + Arr2D[0][1] * _Matrix.Arr2D[1][1] + Arr2D[0][2] * _Matrix.Arr2D[2][1] + Arr2D[0][3] * _Matrix.Arr2D[3][1];
	Result.Z = Arr2D[0][0] * _Matrix.Arr2D[0][2] + Arr2D[0][1] * _Matrix.Arr2D[1][2] + Arr2D[0][2] * _Matrix.Arr2D[2][2] + Arr2D[0][3] * _Matrix.Arr2D[3][2];
	Result.W = Arr2D[0][0] * _Matrix.Arr2D[0][3] + Arr2D[0][1] * _Matrix.Arr2D[1][3] + Arr2D[0][2] * _Matrix.Arr2D[2][3] + Arr2D[0][3] * _Matrix.Arr2D[3][3];


	//std::cout << "X : " << Arr2D[0][0] << "*" << _Matrix.Arr2D[0][0] << "+" << Arr2D[0][1] << "*" << _Matrix.Arr2D[1][0] << "+" << Arr2D[0][2] << "*" << _Matrix.Arr2D[2][0] << "+" << Arr2D[0][3] << "*" << _Matrix.Arr2D[3][0] << std::endl;
	//std::cout << "Y : " << Arr2D[0][0] << "*" << _Matrix.Arr2D[0][1] << "+" << Arr2D[0][1] << "*" << _Matrix.Arr2D[1][1] << "+" << Arr2D[0][2] << "*" << _Matrix.Arr2D[2][1] << "+" << Arr2D[0][3] << "*" << _Matrix.Arr2D[3][1] << std::endl;
	//std::cout << "Z : " << Arr2D[0][0] << "*" << _Matrix.Arr2D[0][2] << "+" << Arr2D[0][1] << "*" << _Matrix.Arr2D[1][2] << "+" << Arr2D[0][2] << "*" << _Matrix.Arr2D[2][2] << "+" << Arr2D[0][3] << "*" << _Matrix.Arr2D[3][2] << std::endl;
	//std::cout << "W : " << Arr2D[0][0] << "*" << _Matrix.Arr2D[0][3] << "+" << Arr2D[0][1] << "*" << _Matrix.Arr2D[1][3] << "+" << Arr2D[0][2] << "*" << _Matrix.Arr2D[2][3] << "+" << Arr2D[0][3] << "*" << _Matrix.Arr2D[3][3] << std::endl;

	//std::cout << Result.ToString() << std::endl;


	return Result;
}

FVector& FVector::operator*=(const FMatrix& _Matrix)
{
	DirectVector = DirectX::XMVector4Transform(DirectVector, _Matrix.DirectMatrix);
	return *this;
}

FMatrix FMatrix::operator*(const FMatrix& _Matrix)
{
	FMatrix Result;
	Result.DirectMatrix = DirectX::XMMatrixMultiply(DirectMatrix, _Matrix.DirectMatrix);
	return Result;

}

ENGINEAPI void FTransform::Decompose()
{
	World.Decompose(WorldScale, WorldQuat, WorldLocation);

	LocalWorld.Decompose(RelativeScale, RelativeQuat, RelativeLocation);

	//Scale = RelativeScale;
	//Quat = RelativeQuat;
	//Rotation = RelativeQuat.QuaternionToEulerDeg();
	//Location = RelativeLocation;
}

void FTransform::TransformUpdate(bool _IsAbsolut /*= false*/)
{
	// 10000 10000 10000

	// �����
	// 100 100 100


	// world���� local
	ScaleMat.Scale(Scale);
	RotationMat.RotationDeg(Rotation);
	LocationMat.Position(Location);

	// ���� ���� �� ���� �ȵ��� 
	FMatrix CheckWorld = ScaleMat * RotationMat * LocationMat;

	// ����� ���������ϴ� ���� ���� �Ǿ�� �Ѵ�.
	if (true == _IsAbsolut)
	{
		// 3000, 0
		// 100, 0
		// -2900

		// CheckWorld ���尡 �Ǿ�� �Ѵٰ� ����.
		World = CheckWorld;
		LocalWorld = CheckWorld * ParentMat.InverseReturn();
		// LocalWorld ���� ���ð��̶�� ��.
	}
	else
	{
		//      ũ         ��             ��            ��           ��
		LocalWorld = CheckWorld;
		World = CheckWorld * RevolveMat * ParentMat;
		// ���� ������ ������ �θ� ���� �Ȱ������� �θ� ���ؼ� ���� ���� ���� ã�Ƴ���.

		// World.ArrVector[3]; => 
	}

	// ũ���� ���� ������ �ٽ� �������� �ϴ� ��.
	Decompose();



}




bool FTransform::Collision(ECollisionType _LeftType, const FTransform& _Left, ECollisionType _RightType, const FTransform& _Right)
{
	if (nullptr == FTransform::AllCollisionFunction[static_cast<int>(_LeftType)][static_cast<int>(_RightType)])
	{
		MSGASSERT("���� �������� ���� �浹 Ÿ�԰��� �浹 üũ�� �Ϸ��� �߽��ϴ�.");
		return false;
	}

	return FTransform::AllCollisionFunction[static_cast<int>(_LeftType)][static_cast<int>(_RightType)](_Left, _Right);
}

bool FTransform::PointToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	FTransform LeftTrans = _Left;
	LeftTrans.Scale = FVector::ZERO;
	return CirCleToCirCle(LeftTrans, _Right);
}

// �� vs �簢��
bool FTransform::PointToRect(const FTransform& _Left, const FTransform& _Right)
{
	FTransform LeftTrans = _Left;
	LeftTrans.Scale = FVector::ZERO;
	return RectToRect(LeftTrans, _Right);
}

bool FTransform::CirCleToCirCle(const FTransform& _Left, const FTransform& _Right)
{

	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	return LeftCol.Sphere.Intersects(RightCol.Sphere);


	//FVector Len = _Left.Location - _Right.Location;

	//// Ʈ�������� ������ ������ �������� x�� ����ũ�⸦ ���������� ���ڽ��ϴ�.
	//// �ο��� �������� ���� ������ ���̺��� ũ�ٸ� 
	//if (Len.Length() < _Left.Scale.hX() + _Right.Scale.hX())
	//{
	//	return true;
	//}

	//return false;
}

bool FTransform::RectToRect(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	return LeftCol.AABB.Intersects(RightCol.AABB);


	//if (_Left.ZAxisCenterLeft() > _Right.ZAxisCenterRight())
	//{
	//	return false;
	//}

	//if (_Left.ZAxisCenterRight() < _Right.ZAxisCenterLeft())
	//{
	//	return false;
	//}

	//if (_Left.ZAxisCenterTop() > _Right.ZAxisCenterBottom())
	//{
	//	return false;
	//}

	//if (_Left.ZAxisCenterBottom() < _Right.ZAxisCenterTop())
	//{
	//	return false;
	//}
	// ���� ����� �ȴ�.
	// return true;
}

bool FTransform::RectToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	return CirCleToRect(_Right, _Left);
}



bool FTransform::CirCleToRect(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	return LeftCol.Sphere.Intersects(RightCol.AABB);


	//// �¿�� ������ Ȯ���� Ʈ������
	//FTransform WTransform = _Right;
	//WTransform.Scale.X += _Left.Scale.X;

	//// ���Ʒ��� ������ ��ŭ Ȯ���� Ʈ������
	//FTransform HTransform = _Right;
	//HTransform.Scale.Y += _Left.Scale.X;

	//if (true == PointToRect(_Left, WTransform) || true == PointToRect(_Left, HTransform))
	//{
	//	return true;
	//}

	//// ��� ������ ���ؼ� static���� ����� ����� �ִµ�.
	//// static FVector ArrPoint[4];
	//// �����忡���� ������.
	//FVector ArrPoint[4];

	//ArrPoint[0] = _Right.ZAxisCenterLeftTop();
	//ArrPoint[1] = _Right.ZAxisCenterLeftBottom();
	//ArrPoint[2] = _Right.ZAxisCenterRightTop();
	//ArrPoint[3] = _Right.ZAxisCenterRightBottom();

	//FTransform PointCirCle;
	//PointCirCle.Scale = _Left.Scale;
	//for (size_t i = 0; i < 4; i++)
	//{
	//	PointCirCle.Location = ArrPoint[i];
	//	if (true == PointToCirCle(_Left, PointCirCle))
	//	{
	//		return true;
	//	}
	//}

	//return false;
}

bool FTransform::OBB2DToOBB2D(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	return LeftCol.OBB.Intersects(RightCol.OBB);
}

bool FTransform::OBB2DToRect(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	return LeftCol.OBB.Intersects(RightCol.AABB);
}

bool FTransform::OBB2DToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	return LeftCol.OBB.Intersects(RightCol.Sphere);
}

bool FTransform::OBB2DToPoint(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Extents = { 0.0f, 0.0f, 0.0f };
	return LeftCol.OBB.Intersects(RightCol.AABB);
}

bool FTransform::OBBToSphere(const FTransform& _Left, const FTransform& _Right) 
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	return LeftCol.OBB.Intersects(RightCol.Sphere);
}
bool FTransform::OBBToOBB(const FTransform& _Left, const FTransform& _Right) 
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	return LeftCol.OBB.Intersects(RightCol.OBB);
}
bool FTransform::OBBToAABB(const FTransform& _Left, const FTransform& _Right) 
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	return LeftCol.OBB.Intersects(RightCol.AABB);
}


bool FTransform::SphereToSphere(const FTransform& _Left, const FTransform& _Right) 
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	return LeftCol.Sphere.Intersects(RightCol.Sphere);
}
bool FTransform::SphereToOBB(const FTransform & _Left, const FTransform & _Right) 
{ 
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	return LeftCol.Sphere.Intersects(RightCol.OBB);
}
bool FTransform::SphereToAABB(const FTransform & _Left, const FTransform & _Right) 
{ 
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	return LeftCol.Sphere.Intersects(RightCol.AABB);
}
bool FTransform::AABBToSphere(const FTransform & _Left, const FTransform & _Right) 
{ 
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	return LeftCol.AABB.Intersects(RightCol.Sphere);
}
bool FTransform::AABBToOBB(const FTransform & _Left, const FTransform & _Right) 
{ 
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	return LeftCol.AABB.Intersects(RightCol.OBB);
}
bool FTransform::AABBToAABB(const FTransform& _Left, const FTransform& _Right) 
{ 
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	return LeftCol.AABB.Intersects(RightCol.AABB);
}