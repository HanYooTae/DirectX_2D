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

	// D3DXVECTOR3 x, y, z�� float������ ������ �ڷ���
	// ��ġ�� �ǹ��ϰų� ������ �ǹ��� �� �ִ�.
	D3DXVECTOR3 position; // ������ ��ġ
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
		- Texture Coordinate(�ؽ��� ��ǥ��)�̴�.
		- ��ġxy�� �����ϱ� ���� xy ��� uv�� ����
		- 0~1 �� ����ȭ�� ��ǥ�̴�.

		DX���� UV��ǥ���� ���
		(u = 0, v = 0)
		��--------------------------�� (u = 1, v = 0)
		|							|
		|							|
		|							|
		|							|
		|							|
		|							|
		|							|
		|							|
		|							|
		��--------------------------�� (u = 1, v = 1)
		(u = 0, v = 1)
	*/
	D3DXVECTOR2 uv;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const uint count = 2;
};
