#pragma once
#include "../ISubsystem.h"	// "Core/Subsystem/ISubsystem.h"
#include "Rederder_ConstantBuffers.h"
#include "Renderer_Types.h"

class Renderer final : public ISubsystem
{
public:
	using ISubsystem::ISubsystem;
	~Renderer();

	// ISubsystem을(를) 통해 상속됨
	virtual bool Initialize() override;
	virtual void Update() override;

	void UpdateRenderables(class Scene* const scene);

private:
	void CreateConstantBuffers();
	void CreateRasterizeStates();
	void CreateBlendStates();

	void UpdateCameraBuffer();
	void UpdateObjectBuffer();
	void UpdateAnimationBuffer();

	// Rendering 처리
	void PassMain();

private:
	class Graphics* graphics = nullptr;
	class CameraComponent* camera = nullptr;
	std::shared_ptr<class D3D11_Pipeline> pipeline;
	
	CAMERA_DATA cpu_camera_buffer;
	std::shared_ptr<class D3D11_ConstantBuffer> gpu_camera_buffer;

	TRANSFORM_DATA cpu_object_buffer;
	std::shared_ptr<class D3D11_ConstantBuffer> gpu_object_buffer;

	ANIMATION_DATA cpu_animation_buffer;
	std::shared_ptr<class D3D11_ConstantBuffer> gpu_animation_buffer;

private:
	std::map<RasterizerStateType, std::shared_ptr<class D3D11_RasterizerState>> rasterizer_state;
	std::map<BlendStateType, std::shared_ptr<class D3D11_BlendState>> blend_state;

	std::map<RenderableType, std::vector<class Actor*>> renderables;
};

