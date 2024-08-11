struct VertexInput
{
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;
	float2 uv : TEXCOORD0;
};


cbuffer CameraBuffer : register(b0)
{
	matrix view;
	matrix proj;
}

cbuffer TransformBuffer : register(b1)
{
	matrix world;
}

PixelInput VS(VertexInput input)
{
	PixelInput output;

	output.position = mul(input.position, world); 
	output.position = mul(output.position, view); 
	output.position = mul(output.position, proj); 

	output.uv = input.uv;
	
	return output;
}

Texture2D source_texture1 : register(t0);
Texture2D source_texture2 : register(t1); // 0 ~ 127
SamplerState samp : register(s0); // 0 ~ 15

float4 PS(PixelInput input) : SV_Target
{
	// Sampling : ������ �����͸� �����ϴ� ��
	// Samp()	: �ؽ�ó���� ���� �κ��� ������(��)�� �����ؿ��� �Լ�

	//float4 color = source_texture.Sample(samp , input.uv);

	// clip() : ���ڷ� ���� �����Ͱ� 0���� ������ �ش� �����͸� ����Ѵ�.
	// ���İ��� 0.9 ������ ��� �ش� ���� ���
	//clip(color.a - 0.9f);

	//if (color.a < 0.1f) // ���İ� 0.1���� ������ ���
	//	discard; // discard : ȣ��Ǵ� ���� �ش� �����͸� ����Ѵ�.


	//float4 color = 0.0f;

	//if (input.uv.x < 1.0f)
	//	color = source_texture1.Sample(samp, input.uv);
	//else
	//	color = source_texture2.Sample(samp, float2((input.uv.x - 1.0f) / 2.0 , input.uv.y / 2.0));

	float4 color = source_texture1.Sample(samp, input.uv);



	return color;
}