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

cbuffer AnimationBuffer : register(b2)
{
    float2 sprite_offset;	// key frame의 offset
    float2 sprite_size;		// key frame의 size
    float2 texture_size;	// texture의 size
	
	// 선언 부분이 PS
    float is_animated; // 애니매이션이 있는지 없는지를 체크	Rederder_ConstantBuffers.h
}

PixelInput VS(VertexInput input)
{
	PixelInput output;

    //output.position = float4(input.position.xy * sprite_size, 0.0f, 1.0f);
	//output.position = mul(output.position, world); 
	
	output.position = mul(input.position, world); 
	output.position = mul(output.position, view); 
	output.position = mul(output.position, proj); 

	output.uv = input.uv;
	
    output.uv *= sprite_size / texture_size;
    output.uv += sprite_offset / texture_size;
	
	return output;
}

Texture2D source_texture : register(t0); // 0 ~ 127
SamplerState samp : register(s0); // 0 ~ 15

float4 PS(PixelInput input) : SV_Target
{
	if (is_animated == 1.0f)
		return source_texture.Sample(samp, input.uv);
    else
		return float4(1, 0, 0, 1);
}