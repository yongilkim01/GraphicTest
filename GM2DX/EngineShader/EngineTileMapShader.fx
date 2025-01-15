struct EngineVertex
{
	float4 POSITION : POSITION;
	float4 UV : TEXCOORD;
	float4 COLOR : COLOR;
};

// ���ؽ� ���̴��� ������ ���ϰ��� �־�� �մϴ�.
// ��ǲ�����2�� �ѱ� ���� ����������ϴµ�.
// �̶��� ��Ģ�� �ֽ��ϴ�.

struct VertexShaderOutPut
{
	float4 SVPOSITION : SV_POSITION; // ����Ʈ����� �������� �������Դϴ�.
	float4 UV : TEXCOORD; // 
	float4 COLOR : COLOR;
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

// ������۴� �ƹ��͵� ���������� ������ �⺻���� 0���� ä�����ϴ�.
cbuffer FSpriteData : register(b1)
{
	float4 CuttingPos;
	float4 CuttingSize;
	float4 Pivot; // 0.5 0.0f
};

// ���ؽ����̴��� �� �������.
VertexShaderOutPut VertexToWorld_VS(EngineVertex _Vertex)
{
	// CPU���� ����� ���� ���̴����� �ѱ�� ����� �˾ƾ� �ϴµ�
	// ������۶�� �θ��ϴ�.
	// ���߿��� ���� �⺻���� ���� ������۸� 
	
	// float4x4 WVP;
	
	VertexShaderOutPut OutPut;
	
	
	_Vertex.POSITION.x += (1.0f - Pivot.x) - 0.5f;
	_Vertex.POSITION.y += (1.0f - Pivot.y) - 0.5f;
	
	OutPut.SVPOSITION = mul(_Vertex.POSITION, WVP);
	
	OutPut.UV = _Vertex.UV;
	OutPut.UV.x = (_Vertex.UV.x * CuttingSize.x) + CuttingPos.x;
	OutPut.UV.y = (_Vertex.UV.y * CuttingSize.y) + CuttingPos.y;
	
	OutPut.COLOR = _Vertex.COLOR;
	return OutPut;
}


Texture2D TileMapTex : register(t0);
// ���÷� 1���� �ʿ��մϴ�.
SamplerState ImageSampler : register(s0);

// ���̴������� ������� �ε��� ���ĵ� ��� ����.
// ex) ���ؽ����̴����� 0�� ������۸� ��
// ex) �ȼ����̴������� 0�� �� ���� �ִ�.
cbuffer ResultColor : register(b0)
{
	float4 PlusColor;
	float4 MulColor;
};

// �̹����� ���ø��ؼ� �̹����� ���̰� �����
float4 PixelToWorld_PS(VertexShaderOutPut _Vertex) : SV_Target0
{
	float4 Color = TileMapTex.Sample(ImageSampler, _Vertex.UV.xy);
	
	if (0.0f >= Color.a)
	{
		// �ȼ����̴����� �ƿ�ǲ ������ �ѱ��� �ʴ´�.
		clip(-1);
	}
	
	Color += PlusColor;
	Color *= MulColor;
	return Color;
};
