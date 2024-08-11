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
   cbuffer : Constant Buffer의 약자
   register : 세이더에 데이터를 연결 시켜준다.
   b : buffer
   0 : slot, 저장할 레지스터 슬롯의 번호, 버퍼는 최대 14개 저장가능 -> 0 ~ 13 범위를 가진다.

   상수 버퍼는 16byte 단위로 정렬해야 한다.
   최대 4096 * 16byte 만큼 받을 수 있다.
*/

// shader에서는 상수버퍼를 받을 때 16byte 단위로 받음
struct ANIMATION_DATA
{
	D3DXVECTOR2 sprite_offet;
	D3DXVECTOR2 sprite_size;
	D3DXVECTOR2 texture_size;
	float is_animated;
	float padding;
};