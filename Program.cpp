// Program.cpp
#include"stdafx.h"
#include"Core/Window.h"
#include "Core/Engine.h"

int APIENTRY WinMain
(
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPSTR lpszCmdParam,
	int nCmdShow
)
{
	// 윈도우 생성
	Window::Create(hInstance, 1000, 1000);
	Window::Show();

	Settings::Get().SetWindowHandle(Window::global_handle);
	Settings::Get().SetWindowInstance(Window::global_instance);
	Settings::Get().SetWidth(static_cast<float>(Window::GetWidth()));
	Settings::Get().SetHeight(static_cast<float>(Window::GetHeight()));

	std::unique_ptr<Engine> engine = std::make_unique<Engine>();

	// 실행 부분 
	while (Window::Update())
	{
		engine->Update();
		engine->Render();
	}

	// 윈도우 파괴
	Window::Destroy();
}

// OpenGL
// DX SDK - DirectX
// GPU를 다루는데 사용하는 함수의 집합
// GPU - 그래픽 처리 장치

// CPU 코어 : 4, 8, 16, 32
// GPU 코어 : 3000, 4000