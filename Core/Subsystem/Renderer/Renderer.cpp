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

	// 어차피 처음에 전부 만들어야 할 데이터들이기 때문에 초기화해줌
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
	// 렌더링 파이프라인, 드로우 명령내려주는 곳
	PassMain();
	graphics->End();
}

void Renderer::UpdateRenderables(Scene* const scene)
{
	auto actors = scene->GetActors();

	// unsigned int로 초기화가 안되나?
	//for(unsigned int i = RenderableType::Min, i < RenderableType::Count; i++)
	
	// 업데이트 전에 내용들을 정리해줘야 함
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
	auto buffer = gpu_camera_buffer->Map<CAMERA_DATA>();		// gpu버퍼 매핑
	*buffer = cpu_camera_buffer;		// cpu버퍼에서 gpu버퍼를 가져와서 업데이드
	gpu_camera_buffer->Unmap();			// gpu버퍼 언매핑
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
