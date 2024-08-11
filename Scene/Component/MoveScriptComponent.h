#pragma once
#include "IComponent.h"
class MoveScriptComponent : public IComponent
{
public:
	// 부모의 생성자를 그대로 상속받음
	using IComponent::IComponent;
	~MoveScriptComponent() = default;

	// Inherited via IComponent
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;
};

