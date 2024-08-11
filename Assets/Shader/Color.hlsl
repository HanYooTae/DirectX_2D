// Color.hlsl


struct VertexInput
{
	/*
		: POSITION0, : COLOR0 -> Semantic
		Semantic
			- 데이터의 출처와 역할에대한 의미를 부여하기 위한 키워드
			- POSITION0에서 POSITION은 이름, 0은 인덱스를 의미한다.
	*/
	float4 position : POSITION0;
	float4 color : COLOR0;
};

struct PixelInput
{
	/*
		System Value(SV)
			- 시스템에서 사용할 값이라고 명시한 것
			- SV_ 접두사를 통해 해당 값이 시스템에서 특별한 의미가 있다고 표현한 것이다.
			- SV는 랜더링 파이프 라인의 단계에 따라 의미가 다를 수 있다.
			- 해당 접두사로 시스템이 사용할 값을 지정해주거나, 시스템에서 어떠한 의미를 가진 값을 받을 수 있다.
			- SV_가 붙는 다양한 키워드가 있고 이건 문서에서 확인
			링크 : https://docs.microsoft.com/ko-kr/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics?redirectedfrom=MSDN
			- VS단계에서의 SV_POSITION의 경우는 해당 SV를 가진 데이터를 첨부하여 출력할 경우,
			해당 값은 RS단계에서 레스터 이미지화를 할 때 사용할, VS단계에서 최종 변환과정을 거쳐서
			나온 정점이라는 의미를 시스템에게 전달할 수 있다.
			- PS단계에서의 SV_POSITION의 경우는 VS단계에서 넘긴 SV_POSTION의 값을 PS 단계가 오기 전인
			RS단계를 거치면서 완성된 래스터 이미지의 픽셀 좌표들을 받겠다는 의미이다.

			ㅁ 래스터 이미지 : 픽셀로 이미지를 표현하는 자료구조, 비트맵이라고도 부른다.
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
		 어떠한 공간(점)을 특정 공간으로 이동시키고 싶다면 해당 점과 이동시킬 공간의 행렬을 곱해주면 된다.
		 
		 행렬을 곱할때는 곱하는 좌항의 열과 우항의 행이 같아야 한다.

		 1x4 * 4x4 -> 1x4
	*/

	PixelInput output;

	// 받은 포지션 값에 SV_접두사를 붙여준다 -> RS 단계에서 사용할 정점을 지정해줌
	// mul : 행렬의 곱을 구하는 함수
	output.position = mul(input.position, world); // 월드 변환
	output.position = mul(output.position, view); // 뷰 변환
	output.position = mul(output.position, proj); // 투영

	output.color = input.color;
	
	return output; // 반환된 값은 RS 단계를 거치게 된다.
}

// SV_Target : 현재 세팅된 메인 렌더 타겟을 사용하라
float4 PS(PixelInput input) : SV_Target
{

	// 1 0 1 1
	// 1 0 0 1 *
	// 1 0 0 1
	return input.color;
}