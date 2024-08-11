#pragma once
#include "IComponent.h"
class TestComponent final : public IComponent
{
public:
	using IComponent::IComponent;

	// IComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

private:

};

