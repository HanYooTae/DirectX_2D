#include "stdafx.h"
#include "Renderer.h"
#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/MeshRendererComponent.h"
#include "Scene/Component/TransformComponent.h"
#include "Scene/Component/AnimatorComponent.h"

// Render하는 과정
void Renderer::PassMain()
{
	std::vector<Actor*> actors = renderables[RenderableType::Qpaque];

	if (actors.empty() == true) return;

	// MeshRendererComponent에 있는 정보들을 세팅해준 후 draw해줌
	for (const auto& actor : actors)
	{
		// imgui
		if (actor->IsActive() == false) continue;

		auto renderable = actor->GetComponent<MeshRendererComponent>();

		if (renderable == nullptr) continue;
		// imgui
		if (renderable->IsEnabled() == false) continue;

		// 어떤 transform이든 actor의 transform으로 고정됨
		auto transform = actor->GetTransform_Raw();
		if (transform == nullptr) continue;

		D3D11_PipelineState pipeline_state;
		pipeline_state.input_layout = renderable->GetInputLayout().get();
		pipeline_state.primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		pipeline_state.vertex_shader = renderable->GetVertexShader().get();
		pipeline_state.pixel_shader = renderable->GetPixelShader().get();
		pipeline_state.rasterizer_state = rasterizer_state[RasterizerStateType::Cull_Back_Fill_Solid].get();
		pipeline_state.blend_state = blend_state[BlendStateType::Alpha].get();

		if (pipeline->Begin(pipeline_state))
		{
			pipeline->SetVertexBuffer(renderable->GetVertexBuffer().get());
			pipeline->SetIndexBuffer(renderable->GetIndexBuffer().get());

			D3DXMatrixTranspose(&cpu_object_buffer.world, &transform->GetWorldMatrix());
			UpdateObjectBuffer();

			pipeline->SetConstantBuffer(0, ShaderScope_VS, gpu_camera_buffer.get());

			// camera정보 업데이트는 Renderer.cpp의 Update함수
			// camera는 한번만 업데이트되면 되기 때문에, for문 밖 또는 Renderer.cpp에 넣어도 상관없음
			pipeline->SetConstantBuffer(1, ShaderScope_VS, gpu_object_buffer.get());

			if (auto animator = actor->GetComponent<AnimatorComponent>())
			{
				auto current_keyframe = animator->GetCurrentKeyframe();
				cpu_animation_buffer.sprite_offet = current_keyframe->offset;
				cpu_animation_buffer.sprite_size = current_keyframe->size;
				cpu_animation_buffer.texture_size = animator->GetCurrentAnimation()->GetSpriteTextureSize();
				cpu_animation_buffer.is_animated = 1.0f;		// true
				UpdateAnimationBuffer();

				pipeline->SetConstantBuffer(2, ShaderScope_VS | ShaderScope_PS, gpu_animation_buffer.get());
				pipeline->SetShaderResource(0, ShaderScope_PS, animator->GetCurrentAnimation()->GetSpriteTexture().get());
			}
			else
			{
				cpu_animation_buffer.sprite_offet = D3DXVECTOR2(0, 0);
				cpu_animation_buffer.sprite_size = D3DXVECTOR2(1, 1);
				cpu_animation_buffer.texture_size = D3DXVECTOR2(1, 1);
				cpu_animation_buffer.is_animated = 0.0f;		// false
				UpdateAnimationBuffer();

				pipeline->SetConstantBuffer_nullptr(2, ShaderScope_VS | ShaderScope_PS);
				pipeline->SetShaderResource_nullptr(0, ShaderScope_PS);
			}

			pipeline->DrawIndexed(
				renderable->GetIndexBuffer()->GetCount(),
				renderable->GetIndexBuffer()->GetOffset(),
				renderable->GetVertexBuffer()->GetOffset()
			);

			pipeline->End();
		}
	}

	ImGui::ShowDemoWindow();

	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
