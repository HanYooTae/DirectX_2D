#pragma once
#include "stdafx.h"

enum class RenderableType : uint
{
	Qpaque,		// 타입이 불투명할 때
	Camera
};

enum class RasterizerStateType : uint
{
	Cull_Front_Fill_Solid,
	Cull_Back_Fill_Solid,
	Cull_None_Fill_Solid,
	Cull_Front_Fill_Wireframe,
	Cull_Back_Fill_Wireframe,
	Cull_None_Fill_Wireframe
};

enum class BlendStateType : uint
{
	Disabled,		// 블렌드 옵션X
	Alpha			// 블렌드 옵션O
};