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
	// Sampling : 임의의 데이터를 추출하는 것
	// Samp()	: 텍스처에서 일정 부분의 데이터(색)을 추출해오는 함수

	//float4 color = source_texture.Sample(samp , input.uv);

	// clip() : 인자로 받은 데이터가 0보다 작으면 해당 데이터를 폐기한다.
	// 알파값이 0.9 이하인 경우 해당 색을 폐기
	//clip(color.a - 0.9f);

	//if (color.a < 0.1f) // 알파가 0.1보다 작으면 폐기
	//	discard; // discard : 호출되는 순간 해당 데이터를 폐기한다.


	//float4 color = 0.0f;

	//if (input.uv.x < 1.0f)
	//	color = source_texture1.Sample(samp, input.uv);
	//else
	//	color = source_texture2.Sample(samp, float2((input.uv.x - 1.0f) / 2.0 , input.uv.y / 2.0));

	float4 color = source_texture1.Sample(samp, input.uv);



	return color;
}