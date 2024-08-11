#pragma once
#include "IComponent.h"

class AIScriptComponent : public IComponent
{
public:
	// �θ��� �����ڸ� �״�� ����
	using IComponent::IComponent;
	~AIScriptComponent() = default;

	// Inherited via IComponent
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

private:
	enum class Direction
	{
		Up,	   // 0
		Right, // 1 
		Down,  // 2
		Left   // 3
	};

	Stopwatch stopwatch;
	Direction direction = Direction::Right;
};
