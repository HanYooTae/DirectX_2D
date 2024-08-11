#pragma once
// Vertex.h

struct D3D11_VertexColor final
{
	D3D11_VertexColor()
		: position(0.0f, 0.0f, 0.0f)
		, color(0.0f,0.0f,0.0f,0.0f)
	{}

	D3D11_VertexColor(const D3DXVECTOR3& position, const D3DXCOLOR& color)
		: position(position)
		, color(color)
	{}

	// D3DXVECTOR3 x, y, z를 float형으로 가지는 자료형
	// 위치를 의미하거나 방향을 의미할 수 있다.
	D3DXVECTOR3 position; // 정점의 위치
	D3DXCOLOR color;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const uint count = 2;
};

struct D3D11_VertexTexture final
{
	D3D11_VertexTexture()
		: position(0.0f, 0.0f, 0.0f)
		, uv(0.0f, 0.0f)
	{}

	D3D11_VertexTexture(const D3DXVECTOR3& position, const D3DXVECTOR2& uv)
		: position(position)
		, uv(uv)
	{}

	D3DXVECTOR3 position;
	/*
		uv
		- Texture Coordinate(텍스쳐 좌표계)이다.
		- 위치xy와 구분하기 위해 xy 대신 uv로 쓴다
		- 0~1 로 정규화된 좌표이다.

		DX기준 UV좌표계의 모습
		(u = 0, v = 0)
		ㅁ--------------------------ㅁ (u = 1, v = 0)
		|							|
		|							|
		|							|
		|							|
		|							|
		|							|
		|							|
		|							|
		|							|
		ㅁ--------------------------ㅁ (u = 1, v = 1)
		(u = 0, v = 1)
	*/
	D3DXVECTOR2 uv;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const uint count = 2;
};
