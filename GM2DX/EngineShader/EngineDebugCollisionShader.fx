struct EngineVertex
{
	float4 POSITION : POSITION;
};

// ���ؽ� ���̴��� ������ ���ϰ��� �־�� �մϴ�.
// ��ǲ�����2�� �ѱ� ���� ����������ϴµ�.
// �̶��� ��Ģ�� �ֽ��ϴ�.

struct VertexShaderOutPut
{
	float4 SVPOSITION : SV_POSITION; // ����Ʈ����� �������� �������Դϴ�.
};

// ������۸� ����ϰڴ�.
cbuffer FTransform : register(b0)
{
	// transformupdate�� 
	// �Ʒ��� ������ �� �����ؼ�
	// WVP�� ������ �Լ��̴�.
	// ��ȯ�� ����
	float4 Scale;
	float4 Rotation;
	float4 Qut;
	float4 Location;

	// ������Ƽ�� ����
	float4 RelativeScale;
	float4 RelativeRotation;
	float4 RelativeQut;
	float4 RelativeLocation;

	// ����
	float4 WorldScale;
	float4 WorldRotation;
	float4 WorldQuat;
	float4 WorldLocation;

	float4x4 ScaleMat;
	float4x4 RotationMat;
	float4x4 LocationMat;
	float4x4 RevolveMat;
	float4x4 ParentMat;
	float4x4 LocalWorld;
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
	float4x4 WVP;
};

VertexShaderOutPut Debug_VS(EngineVertex _Vertex)
{
	VertexShaderOutPut OutPut;
	OutPut.SVPOSITION = mul(_Vertex.POSITION, WVP);
	return OutPut;
}


// ex) �ȼ����̴������� 0�� �� ���� �ִ�.
cbuffer OutColor : register(b0)
{
	float4 Color;
};

// �̹����� ���ø��ؼ� �̹����� ���̰� �����
float4 PixelToWorld_PS(VertexShaderOutPut _Vertex) : SV_Target0
{
	return Color;
};
