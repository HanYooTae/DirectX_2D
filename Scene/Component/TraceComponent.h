#pragma once
#include "IComponent.h"

enum State
{
	Trace,
	Avoid
};

class TraceComponent : public IComponent
{
public:
	using IComponent::IComponent;
	~TraceComponent() = default;

	// Inherited via IComponent
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void SetTarget(const std::shared_ptr<const class Actor> target)
	{
		// �����Ͱ� ���ٸ� ����
		if (actor == target.get()) return;

		// �ƴ� ���, ���� �������
		this->target = target;
	}

private:
	// �ּҴ� ����������(weak_ptr) Actor�� ����ȵ�
	std::weak_ptr<const class Actor> target;

	Stopwatch timer;
	int speed = 40;

	enum State current_state = State::Trace;
};

