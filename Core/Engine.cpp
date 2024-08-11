#include "stdafx.h"
#include "Engine.h"

Engine::Engine()
{
	//scene_manager = std::make_shared<SceneManager>();
	//scene_manager->Initialize();

	context = new Context();

	// 가장 먼저 업데이트되는게 좋음, 같은 시간으로 적용시켜주기 위해
	context->RegisterSubsystem<Timer>();
	context->RegisterSubsystem<InputManager>();

	context->RegisterSubsystem<Graphics>();		// 그래픽스는 어디에 있든 상관없음
	context->RegisterSubsystem<ResourceManager>();
	context->RegisterSubsystem<SceneManager>();


	// 렌더가 되는 것에는 문제없지만, Update함수보다 아래로 빼주고 싶다면 새롭게 함수를 만든 후에 Render함수에 넣어주면 됨
	// 가장 마지막에 업데이트(렌더) 되도록 제일 마지막에 생성
	context->RegisterSubsystem<Renderer>();

	context->InitializeSubsystems();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(Settings::Get().GetWindowHandle());
	auto graphics = context->GetSubsystem<Graphics>();
	ImGui_ImplDX11_Init(graphics->GetDevice(), graphics->GetDeviceContext());

	// std::to_string() : 들어오는 데이터를 string형태로 바꿔줌
	//Logsystem::SetLog("INFO", __FUNCTION__, __FILE__, std::to_string(__LINE__), "시스템 세팅 완료!");

	// 위의 코드를 편안하게 세팅해주기 위하여 매크로로 작성함
	LOG_INFO("시스템 세팅 완료!");
	LOG_WARNING("시스템 세팅 완료!");
	LOG_ERROR("시스템 세팅 완료!");
}

Engine::~Engine()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SAFE_DELETE(context);
}

void Engine::Update()
{
	// Start the Dear ImGui frame
	// imgui창을 만들기 위한 RenderTargetView
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	context->UpdateSubsystems();
}

// context->RegisterSubsystem<Renderer>();만 있다면 Render함수가 굳이 있을 필요가 없음
void Engine::Render()
{
	Renderer* renderer = context->GetSubsystem<Renderer>();
	//context->UpdateSubsystems();가 있기 때문에 Update가 2번일어남
	renderer->Update();
}
