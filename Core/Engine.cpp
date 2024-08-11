#include "stdafx.h"
#include "Engine.h"

Engine::Engine()
{
	//scene_manager = std::make_shared<SceneManager>();
	//scene_manager->Initialize();

	context = new Context();

	// ���� ���� ������Ʈ�Ǵ°� ����, ���� �ð����� ��������ֱ� ����
	context->RegisterSubsystem<Timer>();
	context->RegisterSubsystem<InputManager>();

	context->RegisterSubsystem<Graphics>();		// �׷��Ƚ��� ��� �ֵ� �������
	context->RegisterSubsystem<ResourceManager>();
	context->RegisterSubsystem<SceneManager>();


	// ������ �Ǵ� �Ϳ��� ����������, Update�Լ����� �Ʒ��� ���ְ� �ʹٸ� ���Ӱ� �Լ��� ���� �Ŀ� Render�Լ��� �־��ָ� ��
	// ���� �������� ������Ʈ(����) �ǵ��� ���� �������� ����
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

	// std::to_string() : ������ �����͸� string���·� �ٲ���
	//Logsystem::SetLog("INFO", __FUNCTION__, __FILE__, std::to_string(__LINE__), "�ý��� ���� �Ϸ�!");

	// ���� �ڵ带 ����ϰ� �������ֱ� ���Ͽ� ��ũ�η� �ۼ���
	LOG_INFO("�ý��� ���� �Ϸ�!");
	LOG_WARNING("�ý��� ���� �Ϸ�!");
	LOG_ERROR("�ý��� ���� �Ϸ�!");
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
	// imguiâ�� ����� ���� RenderTargetView
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	context->UpdateSubsystems();
}

// context->RegisterSubsystem<Renderer>();�� �ִٸ� Render�Լ��� ���� ���� �ʿ䰡 ����
void Engine::Render()
{
	Renderer* renderer = context->GetSubsystem<Renderer>();
	//context->UpdateSubsystems();�� �ֱ� ������ Update�� 2���Ͼ
	renderer->Update();
}
