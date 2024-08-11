#pragma once
#include "IComponent.h"
class TestComponent final : public IComponent
{
public:
	using IComponent::IComponent;

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

private:

};

