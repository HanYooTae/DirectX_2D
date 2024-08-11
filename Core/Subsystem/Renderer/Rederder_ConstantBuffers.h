#pragma once
#include "stdafx.h"

struct CAMERA_DATA
{
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

struct TRANSFORM_DATA
{
	D3DXMATRIX world;
};

/*
   cbuffer : Constant Buffer�� ����
   register : ���̴��� �����͸� ���� �����ش�.
   b : buffer
   0 : slot, ������ �������� ������ ��ȣ, ���۴� �ִ� 14�� ���尡�� -> 0 ~ 13 ������ ������.

   ��� ���۴� 16byte ������ �����ؾ� �Ѵ�.
   �ִ� 4096 * 16byte ��ŭ ���� �� �ִ�.
*/

// shader������ ������۸� ���� �� 16byte ������ ����
struct ANIMATION_DATA
{
	D3DXVECTOR2 sprite_offet;
	D3DXVECTOR2 sprite_size;
	D3DXVECTOR2 texture_size;
	float is_animated;
	float padding;
};