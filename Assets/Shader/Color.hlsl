// Color.hlsl


struct VertexInput
{
	/*
		: POSITION0, : COLOR0 -> Semantic
		Semantic
			- �������� ��ó�� ���ҿ����� �ǹ̸� �ο��ϱ� ���� Ű����
			- POSITION0���� POSITION�� �̸�, 0�� �ε����� �ǹ��Ѵ�.
	*/
	float4 position : POSITION0;
	float4 color : COLOR0;
};

struct PixelInput
{
	/*
		System Value(SV)
			- �ý��ۿ��� ����� ���̶�� ����� ��
			- SV_ ���λ縦 ���� �ش� ���� �ý��ۿ��� Ư���� �ǹ̰� �ִٰ� ǥ���� ���̴�.
			- SV�� ������ ������ ������ �ܰ迡 ���� �ǹ̰� �ٸ� �� �ִ�.
			- �ش� ���λ�� �ý����� ����� ���� �������ְų�, �ý��ۿ��� ��� �ǹ̸� ���� ���� ���� �� �ִ�.
			- SV_�� �ٴ� �پ��� Ű���尡 �ְ� �̰� �������� Ȯ��
			��ũ : https://docs.microsoft.com/ko-kr/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics?redirectedfrom=MSDN
			- VS�ܰ迡���� SV_POSITION�� ���� �ش� SV�� ���� �����͸� ÷���Ͽ� ����� ���,
			�ش� ���� RS�ܰ迡�� ������ �̹���ȭ�� �� �� �����, VS�ܰ迡�� ���� ��ȯ������ ���ļ�
			���� �����̶�� �ǹ̸� �ý��ۿ��� ������ �� �ִ�.
			- PS�ܰ迡���� SV_POSITION�� ���� VS�ܰ迡�� �ѱ� SV_POSTION�� ���� PS �ܰ谡 ���� ����
			RS�ܰ踦 ��ġ�鼭 �ϼ��� ������ �̹����� �ȼ� ��ǥ���� �ްڴٴ� �ǹ��̴�.

			�� ������ �̹��� : �ȼ��� �̹����� ǥ���ϴ� �ڷᱸ��, ��Ʈ���̶�� �θ���.
	*/
	float4 position : SV_POSITION0;
	float4 color : COLOR0;
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

cbuffer ColorBuffer : register(b2)
{
	float4 color;
}


PixelInput VS(VertexInput input)
{
	/*
		 ��� ����(��)�� Ư�� �������� �̵���Ű�� �ʹٸ� �ش� ���� �̵���ų ������ ����� �����ָ� �ȴ�.
		 
		 ����� ���Ҷ��� ���ϴ� ������ ���� ������ ���� ���ƾ� �Ѵ�.

		 1x4 * 4x4 -> 1x4
	*/

	PixelInput output;

	// ���� ������ ���� SV_���λ縦 �ٿ��ش� -> RS �ܰ迡�� ����� ������ ��������
	// mul : ����� ���� ���ϴ� �Լ�
	output.position = mul(input.position, world); // ���� ��ȯ
	output.position = mul(output.position, view); // �� ��ȯ
	output.position = mul(output.position, proj); // ����

	output.color = input.color;
	
	return output; // ��ȯ�� ���� RS �ܰ踦 ��ġ�� �ȴ�.
}

// SV_Target : ���� ���õ� ���� ���� Ÿ���� ����϶�
float4 PS(PixelInput input) : SV_Target
{

	// 1 0 1 1
	// 1 0 0 1 *
	// 1 0 0 1
	return input.color;
}