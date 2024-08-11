#pragma once

// ** �̸� �����ϵ� ����� Ư�� ��ο� �־��ָ� �ȵ� **

// stdafx.h
#define SHADER_5_0
#define DIRECTINPUT_VERSION 0x0800
#define NOMINMAX

#ifdef _DEBUG

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#endif // _DEBUG

#pragma warning(disable : 4506)		// ���� ���� : no definition for inline function 'ComponentType IComponent::DeduceComponentType<T>(void)'

//Windows
#include<Windows.h>
#include<assert.h>

//STD
#include<vector>
#include<map>
#include<iostream>
#include<unordered_map>
#include<sstream>
#include<random>
#include<chrono>
#include<functional>
#include<memory>
#include<mutex>
#include<fstream>		// file stream

//DirectX
#include<D3Dcompiler.h>
#include<D3D11.h>
#include<D3DX10math.h>
#include<D3DX11async.h>
#include <dinput.h>		//DirectX input

// pragma comment(lib, lib���� �̸�)
// -> ������Ʈ �Ӽ� -> ��Ŀ -> �Է� -> �߰� ���Ӽ��� ���̺귯���� �߰��� �Ͱ� �Ȱ���
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using uint = unsigned int;

#define SAFE_RELEASE(p) {if(p) { (p)->Release(); (p) = nullptr; }}
#define SAFE_DELETE(p) {if(p) { delete (p); (p) = nullptr; }}
#define SAFE_DELETE_ARRAY(p) {if(p) { delete[] (p); (p) = nullptr; }}

//Math
#include"Math/Math.h"

//Utility
#include"Utility/Geometry_Generator.h"
#include"Utility/GUID_Generator.h"
#include "Utility/Xml.h"
#include "Utility/Imgui/imgui.h"
#include "Utility/Imgui/imgui_impl_win32.h"
#include "Utility/Imgui/imgui_impl_dx11.h"
#include "Utility/Logsystem.h"

//Framework
#include"Core/Settings.h"
#include"Core/Object.h"

// Main System
#include"Core/Context.h"

// Sub System
#include "Core/Subsystem/Graphics.h"
#include "Core/Subsystem/SceneManager.h"
#include "Core/Subsystem/Renderer/Renderer.h"
#include "Core/Subsystem/Timer.h"
#include "Core/Subsystem/ResourceManager.h"
#include "Core/Subsystem/InputManager.h"


#include"Core/D3D11/Vertex/D3D11_Vertex.h"
#include"Core/D3D11/Vertex/D3D11_Geometry.h"

#include"Core/D3D11/InputAssembler/D3D11_VertexBuffer.h"
#include"Core/D3D11/InputAssembler/D3D11_IndexBuffer.h"
#include"Core/D3D11/InputAssembler/D3D11_InputLayout.h"

#include"Core/D3D11/Shader/D3D11_Shader.h"
#include"Core/D3D11/Shader/D3D11_ConstantBuffer.h"
#include"Core/D3D11/Shader/D3D11_Texture.h"
#include"Core/D3D11/Shader/D3D11_SamplerState.h"

#include"Core/D3D11/Rasterizer/D3D11_RasterizerState.h"
#include"Core/D3D11/Rasterizer/D3D11_Viewport.h"

#include"Core/D3D11/OutputMerger/D3D11_BlendState.h"

#include"Core/D3D11/D3D11_Pipeline.h"

// Resources
#include "Resource/Animation.h"