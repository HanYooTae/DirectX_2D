#include "stdafx.h"
#include "Renderer.h"
#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Scene/Component/MeshRendererComponent.h"
#include "Scene/Component/CameraComponent.h"

Renderer::~Renderer()
{
}

bool Renderer::Initialize()
{
	graphics = context->GetSubsystem<Graphics>();
	graphics->CreateBackBuffer(
		static_cast<uint>(Settings::Get().GetWidth()),
		static_cast<uint>(Settings::Get().GetHeight())
	);

	pipeline = std::make_shared<D3D11_Pipeline>(graphics);

	// ������ ó���� ���� ������ �� �����͵��̱� ������ �ʱ�ȭ����
	CreateConstantBuffers();
	CreateRasterizeStates();
	CreateBlendStates();

	return true;
}

void Renderer::Update()
{
	if (camera == nullptr) return;

	D3DXMatrixTranspose(&cpu_camera_buffer.view, &camera->GetViewMatrix());
	D3DXMatrixTranspose(&cpu_camera_buffer.projection, &camera->GetProjectionMatrix());
	UpdateCameraBuffer();

	graphics->Begin();
	// ������ ����������, ��ο� ��ɳ����ִ� ��
	PassMain();
	graphics->End();
}

void Renderer::UpdateRenderables(Scene* const scene)
{
	auto actors = scene->GetActors();

	// unsigned int�� �ʱ�ȭ�� �ȵǳ�?
	//for(unsigned int i = RenderableType::Min, i < RenderableType::Count; i++)
	
	// ������Ʈ ���� ������� ��������� ��
	renderables[RenderableType::Qpaque].clear();
	renderables[RenderableType::Camera].clear();

	if (actors.empty() == true) return;

	for (const auto& actor : actors)
	{
		if (auto camera_component = actor->GetComponent<CameraComponent>())
		{
			renderables[RenderableType::Camera].emplace_back(actor.get());
			camera = camera_component.get();
		}

		if (auto camera_component = actor->GetComponent<MeshRendererComponent>())
		{
			renderables[RenderableType::Qpaque].emplace_back(actor.get());
		}

	}
}

void Renderer::UpdateCameraBuffer()
{
	auto buffer = gpu_camera_buffer->Map<CAMERA_DATA>();		// gpu���� ����
	*buffer = cpu_camera_buffer;		// cpu���ۿ��� gpu���۸� �����ͼ� �����̵�
	gpu_camera_buffer->Unmap();			// gpu���� �����
}

void Renderer::UpdateObjectBuffer()
{
	auto buffer = gpu_object_buffer->Map<TRANSFORM_DATA>();
	*buffer = cpu_object_buffer;
	gpu_object_buffer->Unmap();
}

void Renderer::UpdateAnimationBuffer()
{
	auto buffer = gpu_animation_buffer->Map<ANIMATION_DATA>();
	*buffer = cpu_animation_buffer;
	gpu_animation_buffer->Unmap();
}
