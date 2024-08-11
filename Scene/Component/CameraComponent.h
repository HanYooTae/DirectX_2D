#pragma once
#include "IComponent.h"

// View, Projection°ü¸®
class CameraComponent final : public IComponent
{
public:
	using IComponent::IComponent;
	~CameraComponent() = default;
	
	// Inherited via IComponent
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	const D3DXMATRIX& GetViewMatrix() const { return view; }
	const D3DXMATRIX& GetProjectionMatrix() const { return projection; }

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

