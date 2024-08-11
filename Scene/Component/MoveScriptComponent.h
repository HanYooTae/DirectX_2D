#pragma once
#include "IComponent.h"
class MoveScriptComponent : public IComponent
{
public:
	// �θ��� �����ڸ� �״�� ��ӹ���
	using IComponent::IComponent;
	~MoveScriptComponent() = default;

	// Inherited via IComponent
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;
};

